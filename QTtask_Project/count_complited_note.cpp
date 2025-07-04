#include "count_complited_note.h"
#include <QByteArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

void count_complited_note()
{
    QString filePath = "Settings/Count_notes.json";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Не вдалося відкрити файл Count_notes!";
        return;
    }

    // збір інфи з файлу
    QByteArray fileData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(fileData);  // json -> info
    QJsonObject countObj = doc.object();

    //береться значення за ключем з джисону
    QString key = "Complited_notes";
    int valueCompl = 0;

    if (countObj.contains(key) && countObj[key].isDouble()) {
        valueCompl = countObj[key].toInt();
    } else {
        qDebug() << "Ключ не знайдено або не є цілим числом";
        return;
    }

    valueCompl += 1;     // доповнюється список виконаних нотаток

    //повертаємо в обєкт та файл
    countObj[key] = valueCompl;
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument newDoc(countObj);
        file.write(newDoc.toJson(QJsonDocument::Indented));
        file.close();
    }
}
