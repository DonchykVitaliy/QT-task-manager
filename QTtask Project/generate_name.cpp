#include "generate_name.h"
#include <QFile>

QString generateFileName(const QString &directoryPath) {
    int counter = 1;
    QString fileName;

    do {
        fileName = QString("%1note_%2.json").arg(directoryPath).arg(counter);
        counter++;
    } while (QFile::exists(fileName));

    return fileName;
}
