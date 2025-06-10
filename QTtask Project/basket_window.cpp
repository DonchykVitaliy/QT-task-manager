#include "basket_window.h"
#include "ui_basket_window.h"
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

basket_window::basket_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::basket_window)
{
    ui->setupUi(this);

    scrollArea = new QScrollArea(this);
    container = new QWidget;
    layout = new QVBoxLayout(container);
    scrollArea->setWidget(container);
    scrollArea->setWidgetResizable(true);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(scrollArea);
    setLayout(layout);
    ui->mainLY->addWidget(scrollArea);

    // Стеження за папкою "Basket"
    QString directoryPath = "Basket/";
    watcher = new QFileSystemWatcher(this);
    watcher->addPath(directoryPath);
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &basket_window::updateBasketList);

    updateBasketList();
}

basket_window::~basket_window()
{
    delete ui;
}

void basket_window::updateBasketList()
{
    QString directoryPath = "Basket/";
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

                basket_widget *basketWidget = new basket_widget(noteTitle, noteDate, noteText, noteNotf, this);
                if (noteNotf) {
                    basketWidget->setStyleSheet("QWidget { border: 2px solid red; }");
                }
                layout->addWidget(basketWidget);

                // видалити нотатку
                connect(basketWidget, &basket_widget::taskDeleted, this, [this, filePath,fileName]() {
                    QFile::remove(filePath);  // Видалення файлу після завершення
                    updateBasketList();         // Оновлення списку
                });

                // відновити нотатку
                connect(basketWidget, &basket_widget::taskResume, this, [this, filePath,fileName]() {
                    QString fileDir = "Basket/" + fileName;
                    QString directoryPath = "Notes/";

                    //generation name file
                    QString newName = generateFileName(directoryPath);

                    if (QFile::rename(fileDir, newName)) {
                        qDebug() << "Файл переміщено до нотаток: " << newName;
                    } else {
                        qDebug() << "Не вдалося перемістити файл!";
                    }
                    QFile::remove(filePath);  // Видалення файлу
                    updateBasketList();         // Оновлення списку
                });
            }
        }
    }
}

void basket_window::on_pushButton_clicked()
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

}

