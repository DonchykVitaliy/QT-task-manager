#include "stat_window.h"
#include "ui_stat_window.h"
#include "generate_name.h"
#include "stat_line_widget.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <QFileSystemWatcher>
#include <QListWidgetItem>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QMessageBox>
#include <QString>

stat_window::stat_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::stat_window)
{
    ui->setupUi(this);

    //віджет для відображення виконаних нотаток
    scrollArea = new QScrollArea(this);
    scrollArea->setStyleSheet(R"(
    QScrollBar:vertical {
        background: #e0e0e0;
        width: 12px;
        margin: 2px 0 2px 0;
        border-radius: 6px;
    }
    QScrollBar::handle:vertical {
        background: #9c9c9c;
        border-radius: 6px;
        min-height: 20px;
    }
    QScrollBar::handle:vertical:hover {
        background: #666;
    }
    QScrollBar::add-line:vertical,
    QScrollBar::sub-line:vertical {
        height: 0;
    }
    QScrollBar::add-page:vertical,
    QScrollBar::sub-page:vertical {
        background: none;
    })");
    container = new QWidget;
    container->setStyleSheet("background-color: #0a0910;");
    layout = new QVBoxLayout(container);
    layout->setAlignment(Qt::AlignTop);
    scrollArea->setWidget(container);
    QVBoxLayout *layout = new QVBoxLayout(this);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);
    setLayout(layout);
    ui->rightLY->addWidget(scrollArea);

    updateStatList();       //оновлення списку

    //вивід статистики користувача
    int done = 0, note = 0, basket = 0;
    updateDoneLbl();    //кількість виконаних
    updateNoteLbl();    //кількість не виконаних
    updateBasketLbl();  //кількість в кошику

    done = ui->doneLbl->text().toInt();
    note = ui->noteLbl->text().toInt();
    basket = ui->basketLbl->text().toInt();

    stat_line_widget *statusWidget = new stat_line_widget(this);
    statusWidget->setCounts(note, done, basket);
    statusWidget->setStyleSheet("border: 2px solid gray; border-radius: 5px;");
    ui->statLayout->addWidget(statusWidget);
}

stat_window::~stat_window()
{
    delete ui;
}

void stat_window::updateStatList()
{
    QString directoryPath = "Statistics/";
    QDir dir(directoryPath);
    QStringList jsonFiles = dir.entryList(QStringList() << "*.json", QDir::Files);

    // Очищення списку віджетів
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // перевірка на пустоту
    if (jsonFiles.isEmpty())
    {
        ui->pushButton->setEnabled(false);

        QLabel *emptyLbl = new QLabel(tr("Історія нотаток пуста"), this);
        emptyLbl->setStyleSheet("color: white; padding: 2px; font-size: 12pt");
        emptyLbl->setAlignment(Qt::AlignCenter);
        layout->addWidget(emptyLbl);
    }
    else
    {
        ui->pushButton->setEnabled(true);
        for (const QString &fileName : jsonFiles) {
            QString filePath = dir.filePath(fileName);
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

                    stat_widget *statWidget = new stat_widget(noteTitle, noteDate, noteText, noteNotf, this);
                    layout->addWidget(statWidget);

                    // видалити нотатку
                    connect(statWidget, &stat_widget::taskDeleted, this, [this, filePath,fileName]() {
                        QFile::remove(filePath);  //видалення файлу
                        updateStatList();
                    });

                    // відновити нотатку
                    connect(statWidget, &stat_widget::taskResume, this, [this, filePath,fileName]() {
                        QString fileDir = "Statistics/" + fileName;
                        QString directoryPath = "Notes/";

                        //generation name file
                        QString newName = generateFileName(directoryPath);

                        if (QFile::rename(fileDir, newName)) {
                            qDebug() << "Файл переміщено до нотаток: " << newName;
                            QFile::remove(filePath);  //видалення файлу
                            updateStatList();         //оновлення списку
                        } else {
                            qDebug() << "Не вдалося перемістити файл!";
                        }
                    });
                }
            }
        }
    }
}


//оновлення лейблу про виконані
void stat_window::updateDoneLbl()
{
    //file
    QString filePath = "Settings/Count_notes.json";
    QFile file(filePath);
    QByteArray fileData;
    if (file.open(QIODevice::ReadOnly))
    {
        fileData = file.readAll();
        file.close();
    }

    //read
    QJsonDocument doc = QJsonDocument::fromJson(fileData);  // json -> info
    QJsonObject countObj = doc.object();
    QString key = "Complited_notes";
    QString valueCompl = "0";

    if (countObj.contains(key)) {
        valueCompl = QString::number(countObj[key].toInt());
    } else {
        qDebug() << "Ключ не знайдено";
        return;
    }

    //update
    ui->doneLbl->setText(valueCompl);   //запис кількості виконаних нотатків у лейбл
}


//оновлення лейблу про нотатки
void stat_window::updateNoteLbl()
{
    QString path = "Notes";
    QDir dir(path);

    //перерахунок файлів у директорії
    QStringList countStr = dir.entryList(QDir::Files);      //only files
    int count = countStr.count();

    //update
    ui->noteLbl->setText(QString::number(count));
}


//оновлення лейблу про кошик
void stat_window::updateBasketLbl()
{
    QString path = "Basket";
    QDir dir(path);

    //перерахунок файлів у директорії
    QStringList countStr = dir.entryList(QDir::Files);      //only files
    int count = countStr.count();

    //update
    ui->basketLbl->setText(QString::number(count));
}


void stat_window::on_pushButton_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Підтвердження"));
    QLabel *label = new QLabel(tr("Ви впевнені,що хочете очистити список?"));
    QPushButton *okBtn = new QPushButton(tr("Так"));
    QPushButton *cancelBtn = new QPushButton(tr("Ні"));
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
        QString directoryPath = "Statistics/";
        QDir dir(directoryPath);
        QStringList jsonFiles = dir.entryList(QStringList() << "*.json", QDir::Files);
        for (const QString &fileName : jsonFiles) {
            QString filePath = dir.filePath(fileName);
            QFile::remove(filePath);
        }
        updateStatList();
    }

}



//кнопка очистки підрахунока виконаних нотаток
void stat_window::on_doneBtn_clicked()
{
QDialog dialog(this);
    dialog.setWindowTitle(tr("Підтвердження"));
QLabel *label = new QLabel(tr("Ви впевнені,що хочете очистити історію виконаних нотаток?"));
    QPushButton *okBtn = new QPushButton(tr("Так"));
    QPushButton *cancelBtn = new QPushButton(tr("Ні"));
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
        QString filePath = "Settings/Count_notes.json";
        QFile file(filePath);
        QJsonObject countObj;

        QString key = "Complited_notes";
        int valueCompl = 0;

        //записуємо обєкт в файл
        countObj[key] = valueCompl;
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QJsonDocument newDoc(countObj);
            file.write(newDoc.toJson(QJsonDocument::Indented));
            file.close();
        }

        updateDoneLbl();
    }
}


//кнопка очистки кошику
void stat_window::on_basketBtn_clicked()
{
QDialog dialog(this);
    dialog.setWindowTitle(tr("Підтвердження"));
    QLabel *label = new QLabel(tr("Ви впевнені,що хочете очистити кошик?"));
    QPushButton *okBtn = new QPushButton(tr("Так"));
    QPushButton *cancelBtn = new QPushButton(tr("Ні"));
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
        QString directoryPath = "Basket/";
        QDir dir(directoryPath);
        QStringList jsonFiles = dir.entryList(QStringList() << "*.json", QDir::Files);
        for (const QString &fileName : jsonFiles) {
            QString filePath = dir.filePath(fileName);
            QFile::remove(filePath);
        }
    }

    updateBasketLbl();
}

