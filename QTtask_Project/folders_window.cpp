#include "folders_window.h"
#include "ui_folders_window.h"
#include "create_folder_widnow.h"
#include "folders_widget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "open_folder_window.h"

folders_window::folders_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::folders_window)
{
    ui->setupUi(this);

    //для відображення плиток
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
    ui->mainLY->addWidget(scrollArea);

    // шлях до папок
    QString directoryPath = "Folders/";
    watcher = new QFileSystemWatcher(this);
    watcher->addPath(directoryPath);

    //оновлення списку папок
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &folders_window::displayFolders);

    displayFolders();
}

folders_window::~folders_window()
{
    delete ui;
}


//кнопка створення папки
void folders_window::on_createBtn_clicked()
{
    create_folder_widnow folder;
    folder.exec();
}


//відображення всіх папок
void folders_window::displayFolders()
{
    QString directoryPath = "Folders/";
    QDir dir(directoryPath);
    QStringList jsonFiles = dir.entryList(QStringList() << "*.json", QDir::Files);

    //oчищення віджетів
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // перевірка на пустоту
    if (jsonFiles.isEmpty())
    {
        QLabel *emptyLbl = new QLabel(tr("Поки папок немає"), this);
        emptyLbl->setStyleSheet("color: white; padding: 2px; font-size: 12pt");
        emptyLbl->setAlignment(Qt::AlignCenter);
        layout->addWidget(emptyLbl);
    }
    else
    {
        //пошук папок
        int count = 0;
        for (const QString &fileName : jsonFiles) {

            //підрахунок списку нотаток до папки
            QString filePath = directoryPath + fileName;
            QFile file(filePath);
            QJsonArray notesArray;

            //зчитування даних про папку
            if (file.open(QIODevice::ReadOnly)) {
                QByteArray fileData = file.readAll();
                file.close();

                QJsonDocument doc = QJsonDocument::fromJson(fileData);
                if (!doc.isNull() && doc.isObject()) {
                    QJsonObject noteObject = doc.object();

                    //рахується кількість
                    if (noteObject.contains("Notes") && noteObject["Notes"].isArray()) {
                        notesArray = noteObject["Notes"].toArray();

                        for (const QJsonValue &val : notesArray) {
                            if (val.isString()) {
                                count++;
                            }
                        }
                    }
                }
            }

            QString countTitle = QString::number(count);
            folders_widget *foldersWidget = new folders_widget(fileName, countTitle, this);
            layout->addWidget(foldersWidget);

            // відкрити папку
            connect(foldersWidget, &folders_widget::openFolder, this, [this, filePath,fileName]() {
                open_folder_window *folder = new open_folder_window(fileName, this);
                folder->setWindowFlag(Qt::Window);
                folder->setAttribute(Qt::WA_DeleteOnClose);  //автоматично видаляється при закритті
                folder->show();
            });
        }
    }
}


void folders_window::on_closeBtn_clicked()
{
    this->close();
}

