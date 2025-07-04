#ifndef STAT_WINDOW_H
#define STAT_WINDOW_H

#include <QDialog>
#include <QMainWindow>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QFileSystemWatcher>
#include "stat_widget.h"

namespace Ui {
class stat_window;
}

class stat_window : public QDialog
{
    Q_OBJECT

public:
    explicit stat_window(QWidget *parent = nullptr);
    ~stat_window();

private slots:
    void updateStatList();

    void updateDoneLbl();

    void updateBasketLbl();

    void updateNoteLbl();

    void on_pushButton_clicked();

    void on_doneBtn_clicked();

    void on_basketBtn_clicked();

private:
    QScrollArea *scrollArea;
    QWidget *container;
    QVBoxLayout *layout;
    QFileSystemWatcher *watcher;
    Ui::stat_window *ui;
};

#endif // STAT_WINDOW_H
