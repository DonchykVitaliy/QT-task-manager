#include "settings_window.h"
#include "ui_settings_window.h"
#include <QDir>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

settings_window::settings_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings_window)
{
    ui->setupUi(this);

    QString filePath = "Settings/Settings.json";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Не вдалося відкрити файл для читання!";
        return;
    }

    // збір інфи з файлу
    QByteArray fileData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(fileData);  // json -> info
    QJsonObject settingsObj = doc.object();

    // початкові стани чек боксів
    if (settingsObj["Clear_basket_list"] == 1) {
        ui->basketCB->setCheckState(Qt::Checked);
    } else {
        ui->basketCB->setCheckState(Qt::Unchecked);
    }
    if (settingsObj["Clear_notification_list"] == 1) {
        ui->historyCB->setCheckState(Qt::Checked);
    } else {
        ui->historyCB->setCheckState(Qt::Unchecked);
    }
    if (settingsObj["Notification"] == 1) {
        ui->notificationCB->setCheckState(Qt::Checked);
    } else {
        ui->notificationCB->setCheckState(Qt::Unchecked);
    }

}

settings_window::~settings_window()
{
    delete ui;
}

void settings_window::on_closeBut_clicked()
{
    this->close();
}


// чек бокс кошику
void settings_window::on_basketCB_stateChanged(int arg1)
{
    const QString key = "Clear_basket_list";
    int newValue;

    // перевірка стану чек бокса
    if (arg1 == Qt::Checked) {
        newValue = 1;
    } else {
        newValue = 0;
    }

    updateSettingValue(key, newValue);
}


// чек бокс нагадування
void settings_window::on_notificationCB_stateChanged(int arg1)
{
    const QString key = "Notification";
    int newValue;

    // перевірка стану чек бокса
    if (arg1 == Qt::Checked) {
        newValue = 1;
    } else {
        newValue = 0;
    }

    updateSettingValue(key, newValue);
}


// чек бокс історії
void settings_window::on_historyCB_stateChanged(int arg1)
{
    const QString key = "Clear_notification_list";
    int newValue;

    // перевірка стану чек бокса
    if (arg1 == Qt::Checked) {
        newValue = 1;
    } else {
        newValue = 0;
    }

    updateSettingValue(key, newValue);
}


void settings_window::updateSettingValue(const QString &key, int newValue)
{
    QString filePath = "Settings/Settings.json";
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
    if (settingsObj.contains(key)) {
        settingsObj[key] = newValue;
    } else {
        qWarning() << "Ключ" << key << "не знайдено у налаштуваннях.";
        return;
    }

    //запис у файл
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument newDoc(settingsObj);
        file.write(newDoc.toJson(QJsonDocument::Indented)); // красиво
        file.close();
    }
}
