#include "folders_window.h"
#include "ui_folders_window.h"
#include "create_folder_widnow.h"
#include "folders_widget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "open_folder_window.h"

folders_window::folders_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::folders_window)
{
    ui->setupUi(this);

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
    layout = new QVBoxLayout(container);
    scrollArea->setWidget(container);
    scrollArea->setWidgetResizable(true);
    ui->mainLY->addWidget(scrollArea);  // Додаємо QScrollArea до головного вікна

    // шлях до нотаток
    QString directoryPath = "Folders/";
    watcher = new QFileSystemWatcher(this);
    watcher->addPath(directoryPath);

    // Підключаємо сигнал до слота для оновлення списку нотаток
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &folders_window::displayFolders);

    displayFolders();  // Початкове заповнення списку
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
                QString countTitle = noteObject["Count"].toString();

                folders_widget *foldersWidget = new folders_widget(fileName, countTitle, this);
                layout->addWidget(foldersWidget);

                // видалити нотатку
                connect(foldersWidget, &folders_widget::openFolder, this, [this, filePath,fileName]() {
                    open_folder_window *folder = new open_folder_window(fileName);
                    folder->setAttribute(Qt::WA_DeleteOnClose);  //автоматично видаляється при закритті
                    folder->show();
                });

            }
        }
    }
}

