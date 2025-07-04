#include "mainwindow.h"

#include <QFontDatabase>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDir>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    // Завантаження шрифту
    int id = QFontDatabase::addApplicationFont(":/fonts/SFProText-Regular.ttf");
    if (id != -1) {
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont font(family);
        font.setPointSize(14);  // або інший розмір
        a.setFont(font);
    }

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "QTtasks_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();


    // створення потрібних папок проєкта
    QDir dir("Notes");
    if (!dir.exists())
        dir.mkpath(".");
    QDir dirD("Basket");
    if (!dirD.exists())
        dirD.mkpath(".");
    QDir dirC("Statistics");
    if (!dirC.exists())
        dirC.mkpath(".");
    QDir dirS("Settings");
    if (!dirS.exists())
        dirS.mkpath(".");
    QDir dirF("Folders");
    if (!dirF.exists())
        dirF.mkpath(".");



    // створення файлу налаштувань
    QString filePath = "Settings/Settings.json";
    QFile fileSettings(filePath);

    if (!QFile::exists(filePath))
    {
        // створення JSON обєкта для збереження даних
        QJsonObject settingObject;
        settingObject["Notification"] = 1;
        settingObject["Clear_notification_list"] = 0;
        settingObject["Clear_basket_list"] = 0;

        // запис JSON у файл
        if (fileSettings.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(settingObject);
            fileSettings.write(doc.toJson());
            fileSettings.close();
        }
    }


    // створення файлу статистики
    QString filePathCount = "Settings/Count_notes.json";
    QFile fileCount(filePathCount);

    if (!QFile::exists(filePathCount))
    {
        // створення JSON обєкта для збереження даних
        QJsonObject countObject;
        countObject["Complited_notes"] = 0;

        // запис JSON у файл
        if (fileCount.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(countObject);
            fileCount.write(doc.toJson());
            fileCount.close();
        }
    }


    // відповідність налаштуванням програми
    if (fileSettings.open(QIODevice::ReadOnly))
    {
        QByteArray fileData = fileSettings.readAll();
        fileSettings.close();

        QJsonDocument doc = QJsonDocument::fromJson(fileData);
        // читання налаштувань
        QString settNotification;
        QString settNotificationList;
        QString settBasketList;
        if (!doc.isNull() && doc.isObject()) {
            QJsonObject settObject = doc.object();
            settNotification = QString::number(settObject["Notification"].toInt());
            settNotificationList = QString::number(settObject["Clear_notification_list"].toInt());
            settBasketList = QString::number(settObject["Clear_basket_list"].toInt());
        }

        // очистка кошику
        if (settBasketList == "1")
        {
            QString directoryBasket = "Basket/";
            QDir dirBasket(directoryBasket);
            QStringList jsonFiles = dirBasket.entryList(QStringList() << "*.json", QDir::Files);
            for (const QString &fileName : jsonFiles) {
                QString filePath = dirBasket.filePath(fileName);
                QFile::remove(filePath);
            }
        }

        // очистка статистики
        if (settNotificationList == "1")
        {
            QString directoryStatistic = "Statistics/";
            QDir dirStatistic(directoryStatistic);
            QStringList jsonFiles = dirStatistic.entryList(QStringList() << "*.json", QDir::Files);
            for (const QString &fileName : jsonFiles) {
                QString filePath = dirStatistic.filePath(fileName);
                QFile::remove(filePath);
            }
        }
    }

    return a.exec();
}
