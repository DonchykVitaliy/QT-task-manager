#include "open_folder_window.h"
#include "ui_open_folder_window.h"
#include "folder_widget.h"
#include "generate_name.h"
#include "edit_task_window.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDialog>
#include <QFile>
#include <QLabel>
#include <QDebug>
#include <QFileSystemWatcher>
#include <QListWidgetItem>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QMessageBox>
#include <QInputDialog>

open_folder_window::open_folder_window(const QString &fileName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::open_folder_window),
    fileName(fileName)              //збереження у полі класу для всіх методів
{
    ui->setupUi(this);
    this->setWindowTitle(fileName);

    // Ініціалізуємо QScrollArea для відображення плиток
    scrollArea = new QScrollArea(this);
    container = new QWidget;
    layout = new QVBoxLayout(container);
    scrollArea->setWidget(container);
    scrollArea->setWidgetResizable(true);
    ui->mainLY->addWidget(scrollArea);  // Додаємо QScrollArea до головного вікна

    loadNotes();  // Початкове заповнення списку
}

open_folder_window::~open_folder_window()
{
    delete ui;
}


//відображення нотаток
void open_folder_window::loadNotes()
{
    QString filePath = "Folders/" + fileName;
    QFile file(filePath);
    QJsonArray notesArray;

    //зчитування даних про папку
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fileData = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(fileData);
        if (!doc.isNull() && doc.isObject()) {
            QJsonObject noteObject = doc.object();

            //запісь в масив списку всіх назв нотаток
            QStringList notesList;
            if (noteObject.contains("Notes") && noteObject["Notes"].isArray()) {
                notesArray = noteObject["Notes"].toArray();

                for (const QJsonValue &val : notesArray) {
                    if (val.isString()) {
                        notesList.append(val.toString());
                    }
                }
            }
        }
    }

    //qDebug() << notesArray;

    // Очищення списку віджетів
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    //відображення нотаток у вікні
    for (const QJsonValue &value : qAsConst(notesArray)) {
        if (value.isString()) {
            QString noteName = value.toString();
            QString filePath = "Notes/" + noteName;
            QFile file(filePath);

            if (file.open(QIODevice::ReadOnly)) {
                QByteArray fileData = file.readAll();
                file.close();

                QJsonDocument doc = QJsonDocument::fromJson(fileData);
                if (!doc.isNull() && doc.isObject()) {
                    QJsonObject noteObject = doc.object();
                    QString noteTitle = noteObject["Name"].toString();
                    QString noteDate = noteObject["Date"].toString();
                    QString noteText = noteObject["Text"].toString();
                    bool noteNotf = noteObject["Notf"].toBool();

                    folder_widget *folderWidget = new folder_widget(noteTitle, noteDate, noteText, noteNotf, this);
                    layout->addWidget(folderWidget);


                    // видалити нотатку
                    connect(folderWidget, &folder_widget::taskDeleted, this, [this, filePath, noteName]() {
                        QString fileDir = "Notes/" + noteName;
                        QString directoryPath = "Basket/";

                        //generation name file
                        QString newName = generateFileName(directoryPath);

                        if (QFile::rename(fileDir, newName)) {
                            qDebug() << "Файл переміщено до корзини: " << newName;
                            QFile::remove(filePath);
                            loadNotes();
                        } else {
                            qDebug() << "Не вдалося перемістити файл!";
                        }
                    });


                    // виконати нотатку
                    connect(folderWidget, &folder_widget::taskCompleted, this, [this, filePath,noteName]() {
                        QString fileDir = "Notes/" + noteName;
                        QString directoryPath = "Statistics/";

                        //generation name file
                        QString newName = generateFileName(directoryPath);

                        if (QFile::rename(fileDir, newName)) {
                            qDebug() << "Файл переміщено " << newName;
                            QFile::remove(filePath);  // Видалення файлу
                            loadNotes();
                        } else {
                            qDebug() << "Не вдалося перемістити файл!";
                        }
                    });


                    // редагувати нотатку
                    connect(folderWidget, &folder_widget::taskEdit, this, [this, filePath,noteName]() {
                        edit_task_window edit(noteName);
                        edit.exec();

                        loadNotes();
                    });

                    // вимкнути нагадування
                    connect(folderWidget, &folder_widget::taskNotf, this, [this, filePath,noteName]() {
                        QFile file(filePath);
                        if (!file.open(QIODevice::ReadOnly)) {
                            qWarning() << "Не вдалося відкрити файл для читання:" << filePath;
                            return;
                        }

                        QByteArray fileData = file.readAll();
                        file.close();

                        QJsonDocument doc = QJsonDocument::fromJson(fileData);
                        QJsonObject settingsObj = doc.object();

                        //зміна параметру
                        if (settingsObj.contains("Notf")) {
                            settingsObj["Notf"] = false;
                        }

                        //запис у файл
                        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                            QJsonDocument newDoc(settingsObj);
                            file.write(newDoc.toJson(QJsonDocument::Indented)); // красиво
                            file.close();
                        }

                        loadNotes();
                    });
                }
            }
        }
    }

}



//rename
void open_folder_window::on_renameBtn_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Зміна назви папки");
    QLabel *label = new QLabel("Вкажіть нову назву для цієї папки:");
    QLineEdit *line = new QLineEdit;
    QPushButton *okBtn = new QPushButton("Перейменувати");
    QPushButton *cancelBtn = new QPushButton("Скасувати");
    line->setStyleSheet(R"(
QLineEdit
{
    border-style: outset;
    border-width: 2px;
    border-radius: 10px;


    background-color: rgb(235, 235, 235);
    border-color: rgb(196, 196, 196);
    color: rgb(119, 120, 115);
}
)");
    label->setStyleSheet("color:white;");
    okBtn->setStyleSheet(
        "QPushButton {"
        "   font-size: 12pt;"
        "   border-style: outset;"
        "   border-width: 2px;"
        "   border-radius: 10px;"
        "   background-color: rgb(152, 133, 248);"
        "   color: white;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(132, 113, 228);"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgb(112, 93, 208);"
        "}"
        );
    cancelBtn->setStyleSheet(
        "QPushButton {"
        "   font-size: 12pt;"
        "   border-style: outset;"
        "   border-width: 2px;"
        "   border-radius: 10px;"
        "   background-color: rgb(152, 133, 248);"
        "   color: white;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(132, 113, 228);"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgb(112, 93, 208);"
        "}"
        );
    cancelBtn->setMinimumHeight(30);
    okBtn->setMinimumHeight(30);
    cancelBtn->setMinimumWidth(135);
    okBtn->setMinimumWidth(150);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    layout->addWidget(label);
    layout->addWidget(line);
    layout->addLayout(btnLayout);

    QString newName;
    connect(okBtn, &QPushButton::clicked, [&] ()
            {
        newName = line->text();
        if (newName != ""){
            dialog.accept();
        }
        else
            {
            QDialog dialogW(this);
            dialogW.setWindowTitle("Помилка");
            QLabel *label = new QLabel("Ви не ввели нову назву папки!");
            QPushButton *okBtn = new QPushButton("Окей");
            label->setStyleSheet("color:white;");
            okBtn->setStyleSheet(
                "QPushButton {"
                "   font-size: 12pt;"
                "   border-style: outset;"
                "   border-width: 2px;"
                "   border-radius: 10px;"
                "   background-color: rgb(152, 133, 248);"
                "   color: white;"
                "   border: none;"
                "}"
                "QPushButton:hover {"
                "   background-color: rgb(132, 113, 228);"
                "}"
                "QPushButton:pressed {"
                "   background-color: rgb(112, 93, 208);"
                "}"
                );
            okBtn->setMinimumHeight(30);

            QVBoxLayout *layout = new QVBoxLayout(&dialogW);
            QHBoxLayout *btnLayout = new QHBoxLayout;
            btnLayout->addWidget(okBtn);

            layout->addWidget(label);
            layout->addLayout(btnLayout);

            connect(okBtn, &QPushButton::clicked, [&] ()
                    {
                        dialogW.accept();
                    });

            dialogW.exec();
        }
            });
    connect(cancelBtn,&QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();

    if (QDialog::Accepted && newName != "")
    {
        QString filePath = "Folders/" + fileName;
        QString newFilePath = "Folders/" + newName + ".json";

        QFile::rename(filePath,newFilePath);
        this->setWindowTitle(newName);
    }
}


//delete
void open_folder_window::on_deleteBtn_clicked()
{
QDialog dialog(this);
    dialog.setWindowTitle("Підтвердження");
    QLabel *label = new QLabel("Ви впевнені,що хочете видалити папку?\nВсі нотатки залишуться не тронутими?");
    QPushButton *okBtn = new QPushButton("Так");
    QPushButton *cancelBtn = new QPushButton("Ні");
    label->setStyleSheet("color:white;");
    okBtn->setStyleSheet(
        "QPushButton {"
        "   font-size: 12pt;"
        "   border-style: outset;"
        "   border-width: 2px;"
        "   border-radius: 10px;"
        "   background-color: rgb(152, 133, 248);"
        "   color: white;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(132, 113, 228);"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgb(112, 93, 208);"
        "}"
        );
    cancelBtn->setStyleSheet(
        "QPushButton {"
        "   font-size: 12pt;"
        "   border-style: outset;"
        "   border-width: 2px;"
        "   border-radius: 10px;"
        "   background-color: rgb(152, 133, 248);"
        "   color: white;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(132, 113, 228);"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgb(112, 93, 208);"
        "}"
        );
    cancelBtn->setMinimumHeight(30);
    okBtn->setMinimumHeight(30);
    cancelBtn->setMinimumWidth(135);
    okBtn->setMinimumWidth(125);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    layout->addWidget(label);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, [&] ()
            {
                dialog.accept();
            });
    connect(cancelBtn, &QPushButton::clicked, [&] ()
            {
                dialog.reject();
            });

    int result = dialog.exec();

    if (result == QDialog::Accepted)
    {
        QString directoryPath = "Folders/" + fileName;
        QFile::remove(directoryPath);
        this->close();
    }

}

