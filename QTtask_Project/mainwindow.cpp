#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "create_window.h"
#include "task_widget.h"
#include "basket_window.h"
#include "stat_window.h"
#include "settings_window.h"
#include "folders_window.h"
#include "generate_name.h"
#include "count_complited_note.h"
#include "edit_task_window.h"
#include "calendar_window.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileSystemWatcher>
#include <QListWidgetItem>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->searchText->setPlaceholderText("Пошук нотатки...");
    ui->clearBut->hide();

    // Ініціалізуємо QScrollArea для відображення плиток
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
    scrollArea->setWidgetResizable(true);
    ui->verticalLayout->addWidget(scrollArea);  // Додаємо QScrollArea до головного вікна

    // шлях до нотаток
    QString directoryPath = "Notes/";
    watcher = new QFileSystemWatcher(this);
    watcher->addPath(directoryPath);

    // Підключаємо сигнал до слота для оновлення списку нотаток
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &MainWindow::updateNoteList);

    updateNoteList();  // Початкове заповнення списку
}

MainWindow::~MainWindow(){delete ui;}


//кнопка створення нотатки
void MainWindow::on_createBut_clicked()
{
    create_window create;
    create.exec();

    //оновлення списку нотатків
    updateNoteList();
}


//відтворення нотаток на головному вікні
void MainWindow::updateNoteList()
{
    QString directoryPath = "Notes/";
    QDir dir(directoryPath);
    QStringList jsonFiles = dir.entryList(QStringList() << "*.json", QDir::Files);

    //очищення layout
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

                //створення TaskWidget і додавання в нього layout
                task_widget *taskWidget = new task_widget(noteTitle, noteDate, noteText, noteNotf, this);
                layout->addWidget(taskWidget);


                // виконати нотатку
                connect(taskWidget, &task_widget::taskCompleted, this, [this, filePath,fileName]() {
                    QString fileDir = "Notes/" + fileName;
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
                    updateNoteList();
                });

                // видалити нотатку
                connect(taskWidget, &task_widget::taskDeleted, this, [this, filePath,fileName]() {
                    QString fileDir = "Notes/" + fileName;
                    QString directoryPath = "Basket/";

                    //generation name file
                    QString newName = generateFileName(directoryPath);

                    if (QFile::rename(fileDir, newName)) {
                        qDebug() << "Файл переміщено до корзини: " << newName;
                        QFile::remove(filePath);
                    } else {
                        qDebug() << "Не вдалося перемістити файл!";
                    }
                    updateNoteList();
                });

                // редагувати нотатку
                connect(taskWidget, &task_widget::taskEdit, this, [this, filePath,fileName]() {
                    edit_task_window edit(fileName);
                    edit.exec();

                    updateNoteList();
                });

                // вимкнути нагадування
                connect(taskWidget, &task_widget::taskNotf, this, [this, filePath,fileName]() {
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

                    updateNoteList();
                });
            }
        }
    }
}



//кнопка кошику
void MainWindow::on_basketBut_clicked()
{
    basket_window basket;
    basket.exec();
}


//кнопка статистики
void MainWindow::on_statBut_clicked()
{
    stat_window stat;
    stat.exec();
}


//кнопка налаштувань
void MainWindow::on_setingsBut_clicked()
{
    settings_window settings;
    settings.exec();
}


//пошуковий рядок
void MainWindow::on_searchText_textChanged()
{
    if (ui->searchText->toPlainText().isEmpty())    //якщо пошук пустий
    {
        ui->clearBut->hide();
        updateNoteList();
    }
    else                                            //якщо не пустий
    {
        ui->clearBut->show();

        QString search = ui->searchText->toPlainText();

        QDir dir("Notes/");
        QStringList jsonFiles = dir.entryList(QStringList() << "*.json", QDir::Files);

        //очищення layout
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

                    if (noteTitle.contains(search))     //перевірка назви нотатки з пошуком
                    {
                        QString noteDate = noteObject["Date"].toString();
                        QString noteText = noteObject["Text"].toString();
                        bool noteNotf = noteObject["Notf"].toBool();

                        //створення TaskWidget і додавання в нього layout
                        task_widget *taskWidget = new task_widget(noteTitle, noteDate, noteText, noteNotf, this);
                        layout->addWidget(taskWidget);


                        // виконати нотатку
                        connect(taskWidget, &task_widget::taskCompleted, this, [this, filePath,fileName]() {
                            QString fileDir = "Notes/" + fileName;
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
                            updateNoteList();
                        });

                        // видалити нотатку
                        connect(taskWidget, &task_widget::taskDeleted, this, [this, filePath,fileName]() {
                            QString fileDir = "Notes/" + fileName;
                            QString directoryPath = "Basket/";

                            //generation name file
                            QString newName = generateFileName(directoryPath);

                            if (QFile::rename(fileDir, newName)) {
                                qDebug() << "Файл переміщено до корзини: " << newName;
                                QFile::remove(filePath);
                            } else {
                                qDebug() << "Не вдалося перемістити файл!";
                            }
                            updateNoteList();
                        });
                    }
                }
            }
        }
    }
}


//кнопка очистки пошуку
void MainWindow::on_clearBut_clicked()
{
    ui->searchText->clear();
    ui->searchText->setFocus();
    updateNoteList();
}


//кнопка календара
void MainWindow::on_calendarBut_clicked()
{
    calendar_window calendar;
    calendar.exec();
}


//кнопка папок
void MainWindow::on_foldersBut_clicked()
{
    folders_window *folders = new folders_window(this);
    folders->show();
}

