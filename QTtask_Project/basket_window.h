#ifndef BASKET_WINDOW_H
#define BASKET_WINDOW_H

#include <QDialog>
#include <QMainWindow>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QFileSystemWatcher>
#include "basket_widget.h"

namespace Ui {
class basket_window;
}

class basket_window : public QDialog
{
    Q_OBJECT

public:
    explicit basket_window(QWidget *parent = nullptr);
    ~basket_window();

private slots:
    void updateBasketList();

    void on_pushButton_clicked();

private:
    QScrollArea *scrollArea;
    QWidget *container;
    QVBoxLayout *layout;
    QFileSystemWatcher *watcher;
    Ui::basket_window *ui;
};

#endif // BASKET_WINDOW_H
