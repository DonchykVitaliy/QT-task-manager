#include "settings_window.h"
#include "ui_settings_window.h"
#include <QDir>
#include <QTranslator>
#include <QDebug>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QCoreApplication>
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
        ui->comboBox->setEnabled(false);
    }
    if (settingsObj["Lang"] == "en_US")
        ui->comboBox_2->setCurrentIndex(1);

    if (settingsObj["Timer"] == 10)
    {
        ui->comboBox->setCurrentIndex(1);
    }
    else if (settingsObj["Timer"] == 15)
    {
        ui->comboBox->setCurrentIndex(2);
    }
    else if (settingsObj["Timer"] == 30)
    {
        ui->comboBox->setCurrentIndex(3);
    }
    else  if (settingsObj["Timer"] == 60)
    {
        ui->comboBox->setCurrentIndex(4);
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

    updateSettingValue(key, QJsonValue(newValue));
}


// чек бокс нагадування
void settings_window::on_notificationCB_stateChanged(int arg1)
{
    const QString key = "Notification";
    int newValue;

    // перевірка стану чек бокса
    if (arg1 == Qt::Checked) {
        newValue = 1;
        ui->comboBox->setEnabled(true);
    } else {
        newValue = 0;
        ui->comboBox->setEnabled(false);
    }

    updateSettingValue(key, QJsonValue(newValue));
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

    updateSettingValue(key, QJsonValue(newValue));
}


void settings_window::updateSettingValue(const QString &key, QJsonValue newValue)
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

void settings_window::on_comboBox_currentIndexChanged(int index)
{
    const QString key = "Timer";
    int newValue;

    if (index == 0)
    {
        newValue = 5;
    }
    else if (index == 1)
    {
        newValue = 10;
    }
    else if (index == 2)
    {
        newValue = 15;
    }
    else if (index == 3)
    {
        newValue = 30;
    }
    else if (index == 4)
    {
        newValue = 60;
    }

    updateSettingValue(key, QJsonValue(newValue));
}



void settings_window::on_comboBox_2_activated(int index)
{
    QDialog dialogW(this);
    dialogW.setWindowTitle(tr("Підтвердження"));
    QLabel *label = new QLabel(tr("Щоб змінити мову застосунку, потрібен перезапуск.\nВи впевнені, що хочете цього?"));
    QPushButton *okBtn = new QPushButton(tr("Так"));
    QPushButton *noBtn = new QPushButton(tr("НІ"));
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
    noBtn->setStyleSheet(
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
    noBtn->setMinimumHeight(30);

    QVBoxLayout *layout = new QVBoxLayout(&dialogW);
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(noBtn);

    layout->addWidget(label);
    layout->addLayout(btnLayout);

    QApplication *app = qApp;

    connect(okBtn, &QPushButton::clicked, [&] ()
            {
                const QString key = "Lang";
                QString newValue;

                if (index == 0)
                    newValue = "uk_UA";
                else if (index == 1)
                    newValue = "en_US";

                updateSettingValue(key, QJsonValue(newValue));

                app->closeAllWindows();
            });
    connect(noBtn, &QPushButton::clicked, [&] ()
            {
                if (ui->comboBox_2->currentIndex() == 1)
                    ui->comboBox_2->setCurrentIndex(0);
                else
                    ui->comboBox_2->setCurrentIndex(1);

                dialogW.close();
            });

    dialogW.exec();
}

