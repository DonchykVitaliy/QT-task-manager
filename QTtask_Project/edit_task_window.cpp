#include "edit_task_window.h"
#include "ui_edit_task_window.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

edit_task_window::edit_task_window(const QString &nameFile, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_task_window),
    nameFile(nameFile)              //збереження у полі класу для всіх методів
{
    ui->setupUi(this);
    ui->textFile->setPlaceholderText("Завдання...");

    QString filePath = "Notes/" + nameFile;
    QFile file(filePath);
    QString noteTitle, noteDate, noteText;

    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray fileData = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(fileData);
        if (!doc.isNull() && doc.isObject())
        {
            QJsonObject noteObject = doc.object();
            noteTitle = noteObject["Name"].toString();
            noteDate = noteObject["Date"].toString();
            noteText = noteObject["Text"].toString();
        }
    }

    ui->nameFile->setText(noteTitle);
    ui->textFile->setText(noteText);
    ui->dataFile->setDate(QDate::fromString(noteDate, "yyyy-MM-dd"));
    this->setWindowTitle(noteTitle);
}

edit_task_window::~edit_task_window()
{
    delete ui;
}

void edit_task_window::on_pushButton_clicked()
{
    this->close();
}


void edit_task_window::on_doneBtn_clicked()
{

    QString filePath = "Notes/" + nameFile;
    QFile file(filePath);

    file.open(QIODevice::ReadOnly);
    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    QJsonObject noteObject = doc.object();
    noteObject["Name"] = ui->nameFile->text();
    noteObject["Text"] = ui->textFile->toPlainText();
    noteObject["Date"] = ui->dataFile->date().toString("yyyy-MM-dd");

    // запис JSON у файл
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::critical(this, "Помилка", "Не вдалося відкрити файл для запису!");
        return;
    }

    doc.setObject(noteObject);
    file.write(doc.toJson());
    file.close();
    this->close();
}

