#ifndef CALENDAR_WINDOW_H
#define CALENDAR_WINDOW_H

#include <QDialog>
#include <QFileSystemWatcher>
#include <QStringList>

namespace Ui {
class calendar_window;
}

class calendar_window : public QDialog
{
    Q_OBJECT

public:
    explicit calendar_window(QWidget *parent = nullptr);
    ~calendar_window();

private slots:
    void on_pushButton_clicked();
    void day_marker();
    bool checkLang();

    void on_calendarWidget_clicked(const QDate &date);

private:
    Ui::calendar_window *ui;

    QMap<QDate, QStringList> dateToFileMap;
    QFileSystemWatcher *watcher;  //спостереження за змінами в папці
};

#endif // CALENDAR_WINDOW_H
