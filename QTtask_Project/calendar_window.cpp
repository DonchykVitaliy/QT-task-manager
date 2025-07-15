#include "calendar_window.h"
#include "ui_calendar_window.h"
#include "open_note.h"
#include <QFileSystemWatcher>
#include <QTextCharFormat>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QTranslator>
#include <QStringList>
#include <QJsonDocument>
#include <QDate>
#include <QDebug>

calendar_window::calendar_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::calendar_window)
{
    ui->setupUi(this);

    //locale calendar
    bool eng = checkLang();
    if (eng)
        ui->calendarWidget->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));



    // шлях до нотаток
    QString directoryPath = "Notes/";
    watcher = new QFileSystemWatcher(this);
    watcher->addPath(directoryPath);

    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &calendar_window::day_marker);

    day_marker();
}

calendar_window::~calendar_window()
{
    delete ui;
}

void calendar_window::on_pushButton_clicked()
{
    this->close();
}

//макерує дні в календарі, де є нотатки
void calendar_window::day_marker()
{
    //стиль маркеру
    QTextCharFormat fmt;
    fmt.setBackground(QBrush(QColor("#9885f8")));  // будь-який колір
    fmt.setForeground(Qt::white);
    fmt.setFontWeight(QFont::Bold);

    //директорія
    QDir dir("Notes/");
    QStringList jsonFiles = dir.entryList(QStringList() << "*.json", QDir::Files);

    for (const QString &fileName : jsonFiles) {
        QString filePath = dir.filePath(fileName);
        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly)) {
            QByteArray fileData = file.readAll();
            file.close();

            QJsonDocument doc = QJsonDocument::fromJson(fileData);
            if (!doc.isNull() && doc.isObject()) {
                QJsonObject noteObject = doc.object();
                QString noteDate = noteObject["Date"].toString();
                bool noteNotf = noteObject["Notf"].toBool();    //нотатка з нагадуванням

                //створення маркера на даті
                if (noteNotf)
                {
                    QDate date = QDate::fromString(noteDate,"yyyy-MM-dd");
                    ui->calendarWidget->setDateTextFormat(date, fmt);

                    dateToFileMap[date].append(fileName);     //запис у список
                }

            }
        }
    }
}

bool calendar_window::checkLang()
{
    QFile fileSettings("Settings/Settings.json");

    if (fileSettings.open(QIODevice::ReadOnly))
    {
        QByteArray fileData = fileSettings.readAll();
        fileSettings.close();

        QJsonDocument doc = QJsonDocument::fromJson(fileData);
        // читання налаштувань
        QString settLang;
        if (!doc.isNull() && doc.isObject()) {
            QJsonObject settObject = doc.object();
            settLang = settObject["Lang"].toString();
        }

        if (settLang == "en_US")
            return true;
        else
            return false;
    }
}


void calendar_window::on_calendarWidget_clicked(const QDate &date)
{
    if (dateToFileMap.contains(date)) {
        const QStringList &files = dateToFileMap[date];

        //выкна нотаток
        for (const QString &fileName : files) {
            open_note *note = new open_note(fileName, this);
            note->setWindowFlag(Qt::Window);
            note->setAttribute(Qt::WA_DeleteOnClose);
            note->show();
        }
    }
}

