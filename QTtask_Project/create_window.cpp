#include "create_window.h"
#include "ui_create_window.h"
#include "generate_name.h"
#include <QDate>
#include <QDir>
#include <QString>
#include <QLabel>
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
    ui->nameFile->setPlaceholderText(tr("Назва нотатки"));
    ui->textFile->setPlaceholderText(tr("Завдання..."));

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
        QDialog dialogW(this);
        dialogW.setWindowTitle(tr("Помилка"));
        QLabel *label = new QLabel(tr("Ви не ввели назву нотатки!"));
        QPushButton *okBtn = new QPushButton(tr("Окей"));
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
            allow = true;
            this->close();                      // якщо файл успішно збережений, то закриття вікна
        } else {
            QMessageBox::critical(this, "Помилка", "Не вдалося створити файл!");
        }
    }
}


//попередження перед закриттям
void create_window::closeEvent(QCloseEvent *event)
{
    QString text = ui->textFile->toPlainText();
    if (text == "")
        allow = true;

    if (!allow)
    {
        QDialog dialog(this);
        dialog.setWindowTitle(tr("Вихід"));
        QLabel *label = new QLabel(tr("Ви впевнені, що хочете закрити вікно створення нотатки?"));
        QPushButton *okBtn = new QPushButton(tr("Закрити"));
        QPushButton *cancelBtn = new QPushButton(tr("Скасувати"));
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
            event->accept();
        }
        else
            event->ignore();
        }
}

