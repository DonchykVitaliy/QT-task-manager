#include "open_folder_window.h"
#include "ui_open_folder_window.h"
#include "folder_widget.h"
#include "generate_name.h"
#include "edit_task_window.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
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
                }
            }
        }
    }

}



//rename
void open_folder_window::on_renameBtn_clicked()
{
    QString newName = QInputDialog::getText(
        this,
        "Зміна назви папки",
        "Вкажіть нову назву для цієї папки:"
        );

    if (!newName.isEmpty())
    {
        QString filePath = "Folders/" + fileName;
        QString newFilePath = "Folders/" + newName + ".json";

        QFile::rename(filePath,newFilePath);
        this->setWindowTitle(newName);
    }
    else{}
}


//delete
void open_folder_window::on_deleteBtn_clicked()
{
    QMessageBox miniBox;
    miniBox.setWindowTitle("Підтвердження");
    miniBox.setText("Ви впевненні, що хочете видалити папку?\nВсі нотатки залишуться не тронутими");
    QPushButton *yesBut = miniBox.addButton("Так", QMessageBox::YesRole);
    QPushButton *noBut = miniBox.addButton("Ні", QMessageBox::NoRole);
    miniBox.setDefaultButton(noBut);
    miniBox.exec();

    if (miniBox.clickedButton() == yesBut)
    {
        QString directoryPath = "Folders/" + fileName;
        QFile::remove(directoryPath);
        this->close();
    }
    else{}
}

