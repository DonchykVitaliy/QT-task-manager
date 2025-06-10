#include "create_window.h"
#include "ui_create_window.h"
#include "generate_name.h"
#include <QDate>
#include <QDir>
#include <QString>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QCloseEvent>

create_window::create_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::create_window)
{
    ui->setupUi(this);
    // початкові налаштування вікна
    ui->nameFile->setPlaceholderText("Назва нотатки");
    ui->textFile->setPlaceholderText("Завдання...");

    QDate trueDate = QDate::currentDate();
    ui->dataFile->setDate(trueDate);

}

create_window::~create_window()
{
    delete ui;
}

void create_window::on_closeBtn_clicked()
{
    this->close();
}


// кнопка створення нотатки
void create_window::on_createBtn_clicked()
{
    QString noteName = ui->nameFile->text();
    QString noteText = ui->textFile->toPlainText();
    QString noteDate = ui->dataFile->date().toString("yyyy-MM-dd");
    bool noteNotf = ui->notificationRd->isChecked();


    if (noteName=="")
    {
        QMessageBox::warning(this, "Помилка створення", "Ви не ввели назву нотатки!");
    }
    else
    {
        QString directoryPath = "Notes/";
        QString fileName = generateFileName(directoryPath);  // генерація назви файлу

        // створення JSON обєкта для збереження даних
        QJsonObject noteObject;
        noteObject["Name"] = noteName;
            noteObject["Date"] = noteDate;
                noteObject["Text"] = noteText;
                    noteObject["Notf"] = noteNotf;

                    // запис JSON у файл
                    QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(noteObject);
            file.write(doc.toJson());
            file.close();
            this->close();                      // якщо файл успішно збережений, то закриття вікна
        } else {
            QMessageBox::critical(this, "Помилка", "Не вдалося створити файл!");
        }

    }



}


//попередження перед закриттям
void create_window::closeEvent(QCloseEvent *event)
{
    QMessageBox msg(this);
    msg.setWindowTitle("Вихід");
    msg.setText("Ви впевнені, що хочете закрити вікно створення нотатки?");
    QPushButton *yes = msg.addButton(tr("Так"), QMessageBox::YesRole);
    QPushButton *no = msg.addButton(tr("Ні"), QMessageBox::NoRole);
    msg.setDefaultButton(no);

    msg.exec();

    if (msg.clickedButton() == yes) {
        event->accept(); //дозволити закриття
    } else {
        event->ignore(); //скасувати
    }
}

