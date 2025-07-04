#include "open_note.h"
#include "ui_open_note.h"
#include "generate_name.h"
#include "count_complited_note.h"
#include <QByteArray>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>

open_note::open_note(const QString &fileName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::open_note),
    fileName(fileName)
{
    ui->setupUi(this);

    QFile file("Notes/" + fileName);

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

            this->setWindowTitle(noteTitle);
            ui->dateLbl->setText(noteDate);
            ui->nameLbl->setText(noteTitle);

            QString formattedText = noteText.toHtmlEscaped().replace("\n", "<br>");
            ui->textEdit->setText(formattedText);

            if (!noteNotf)
                ui->notfBtn->hide();
        }
    }

}

open_note::~open_note()
{
    delete ui;
}

void open_note::on_notfBtn_clicked()
{
    ui->notfBtn->hide();

    QFile file("Notes/" + fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не вдалося відкрити файл для читання:" << fileName;
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
}


void open_note::on_deleteBtn_clicked()
{
    QString fileDir = "Notes/" + fileName;
    QString filePath = fileDir;
    QString directoryPath = "Basket/";

    //generation name file
    QString newName = generateFileName(directoryPath);

    if (QFile::rename(fileDir, newName)) {
        qDebug() << "Файл переміщено до корзини: " << newName;
        QFile::remove(filePath);
    } else {
        qDebug() << "Не вдалося перемістити файл!";
    }

    this->close();
}


void open_note::on_doneBtn_clicked()
{
    QString fileDir = "Notes/" + fileName;
    QString filePath = fileDir;
    QString directoryPath = "Statistics/";

    //generation name file
    QString newName = generateFileName(directoryPath);

    if (QFile::rename(fileDir, newName)) {
        qDebug() << "Файл переміщено до статистики: " << newName;
        QFile::remove(filePath);
        count_complited_note();
    } else {
        qDebug() << "Не вдалося перемістити файл!";
    }
}


