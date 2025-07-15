#include "notifications.h"
#include "open_note.h"
#include <QApplication>
#include <QCoreApplication>
#include <QWidget>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QJsonObject>
#include <QByteArray>
#include <QStringList>
#include <QJsonDocument>

void notifications()
{
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
                bool noteNotf = noteObject["Notf"].toBool();    //нотатка з нагадуванням

                //створення маркера на даті
                if (noteNotf)
                {
                    open_note *note = new open_note(fileName);
                    note->setWindowFlag(Qt::Window);
                    note->setAttribute(Qt::WA_DeleteOnClose);
                    note->show();
                }

            }
        }
    }
}
