#include "stat_window.h"
#include "ui_stat_window.h"
#include "generate_name.h"
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
    container = new QWidget;
    layout = new QVBoxLayout(container);
    scrollArea->setWidget(container);
    QVBoxLayout *layout = new QVBoxLayout(this);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);
    setLayout(layout);
    ui->rightLY->addWidget(scrollArea);

    updateStatList();       //оновлення списку

    //вивід статистики користувача
    updateDoneLbl();    //кількість виконаних
    updateNoteLbl();    //кількість не виконаних
    updateBasketLbl();  //кількість в кошику

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
                if (noteNotf) {
                    statWidget->setStyleSheet("QWidget { border: 2px solid red; }");
                }
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
    QMessageBox miniBox;
    miniBox.setWindowTitle("Підтвердження");
    miniBox.setText("Ви впевненні, що хочете очистити список?");
    QPushButton *yesBut = miniBox.addButton("Так", QMessageBox::YesRole);
    QPushButton *noBut = miniBox.addButton("Ні", QMessageBox::NoRole);
    miniBox.setDefaultButton(noBut);
    miniBox.exec();

    if (miniBox.clickedButton() == yesBut)
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
    else{}

}



//кнопка очистки підрахунока виконаних нотаток
void stat_window::on_doneBtn_clicked()
{
    QMessageBox miniBox;
    miniBox.setWindowTitle("Підтвердження");
    miniBox.setText("Ви впевненні, що хочете очистити історію виконаних нотаток?");
    QPushButton *yesBut = miniBox.addButton("Так", QMessageBox::YesRole);
    QPushButton *noBut = miniBox.addButton("Ні", QMessageBox::NoRole);
    miniBox.setDefaultButton(noBut);
    miniBox.exec();

    if (miniBox.clickedButton() == yesBut)
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
    else{}
}


//кнопка очистки кошику
void stat_window::on_basketBtn_clicked()
{
    QMessageBox miniBox;
    miniBox.setWindowTitle("Підтвердження");
    miniBox.setText("Ви впевненні, що хочете очистити кошик?");
    QPushButton *yesBut = miniBox.addButton("Так", QMessageBox::YesRole);
    QPushButton *noBut = miniBox.addButton("Ні", QMessageBox::NoRole);
    miniBox.setDefaultButton(noBut);
    miniBox.exec();

    if (miniBox.clickedButton() == yesBut)
    {
        QString directoryPath = "Basket/";
        QDir dir(directoryPath);
        QStringList jsonFiles = dir.entryList(QStringList() << "*.json", QDir::Files);
        for (const QString &fileName : jsonFiles) {
            QString filePath = dir.filePath(fileName);
            QFile::remove(filePath);
        }
    }
    else{}

    updateBasketLbl();
}

