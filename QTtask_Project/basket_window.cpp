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

    // oчищення списку віджетів
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // перевірка на пустоту
    if (jsonFiles.isEmpty())
    {
        ui->pushButton->setEnabled(false);

        QLabel *emptyLbl = new QLabel("Кошик пустий", this);
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
}

void basket_window::on_pushButton_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Підтвердження");
    QLabel *label = new QLabel("Ви впевнені,що хочете очистити кошик?");
    QPushButton *okBtn = new QPushButton("Так");
    QPushButton *cancelBtn = new QPushButton("Ні");
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

}

