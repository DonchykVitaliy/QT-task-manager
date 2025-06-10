#include "create_folder_widnow.h"
#include "ui_create_folder_widnow.h"
#include <QDir>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QJsonArray>

create_folder_widnow::create_folder_widnow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::create_folder_widnow)
{
    ui->setupUi(this);

    // початкові налаштування вікна
    ui->nameFile->setPlaceholderText("Назва папки");

    //список всіх нотаток
    QDir dir("Notes/");
    QStringList fileList = dir.entryList(QStringList() << "*.json", QDir::Files);
    //список всіх папок
    QDir dirF("Folders/");
    QStringList foldersList = dirF.entryList(QStringList() << "*.json", QDir::Files);

    //список чекбоксів нотаток
    loadList(fileList, dir);

}

create_folder_widnow::~create_folder_widnow()
{
    delete ui;
}

void create_folder_widnow::loadList(QStringList fileList, QDir dir)
{
    m_fileList = fileList;
    m_dir = dir;
    QVBoxLayout *layout = new QVBoxLayout;

    for (const QString &fileName : fileList) {
        QString filePath = dir.absoluteFilePath(fileName);
        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Не вдалося відкрити файл:" << filePath;
            continue;
        }

        QByteArray fileData = file.readAll();
        file.close();
        QJsonDocument doc = QJsonDocument::fromJson(fileData);

        QJsonObject obj = doc.object();
        QString title = obj.value("Name").toString();  // зчитування назви

        QCheckBox *checkBox = new QCheckBox(title);
        layout->addWidget(checkBox);
    }

    ui->groupBox->setLayout(layout);
}


//close
void create_folder_widnow::on_closeBtn_clicked()
{
    this->close();
}


//create
void create_folder_widnow::on_createBtn_clicked()
{
    //
    QString folderName = ui->nameFile->text();
    int count = 0;

    //списки
    QStringList notesBox;
    QList<QCheckBox*> checkBoxes = ui->groupBox->findChildren<QCheckBox*>();

    //перебір елементів чекбокс
    for(QCheckBox *CheckBox : checkBoxes)
    {
        if (CheckBox->isChecked())
        {
            //перебір файлів з директорії
            for (const QString &fileName : m_fileList) {
                QString filePath = m_dir.absoluteFilePath(fileName);
                QFile file(filePath);

                if (!file.open(QIODevice::ReadOnly)) {
                    qDebug() << "Не вдалося відкрити файл:" << filePath;
                    continue;
                }

                QByteArray fileData = file.readAll();
                file.close();
                QJsonDocument doc = QJsonDocument::fromJson(fileData);

                QJsonObject obj = doc.object();
                QString title = obj.value("Name").toString();  // зчитування назви

                //перевірка чи назва нотатки співпадає з чекбоксом та додаємо в список
                if (title == CheckBox->text())
                {
                    notesBox << fileName;
                    count++;
                }
            }
        }
    }

    qDebug() << notesBox;

    if (folderName=="")
    {
        QMessageBox::warning(this, "Помилка створення", "Ви не ввели назву папки!");
    }
    else
    {
        QString fileFolder = "Folders/" + folderName + ".json";

        // Конвертація в QJsonArray
        QJsonArray jsonArray;
        for (const QString &str : notesBox) {
            jsonArray.append(str);
        }
        //кількість нотаток в папці
        QString countNotes = QString::number(count);
        // створення JSON обєкта для збереження даних
        QJsonObject noteObject;
        noteObject["Notes"] = jsonArray;
        noteObject["Count"] = countNotes;

        // запис JSON у файл
        QFile file(fileFolder);
        if (file.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(noteObject);
            file.write(doc.toJson());
            file.close();
            this->close();                      // якщо файл успішно збережений, то закриття вікна
        } else {
            QMessageBox::critical(this, "Помилка", "Не вдалося створити папку!");
        }

    }
}

