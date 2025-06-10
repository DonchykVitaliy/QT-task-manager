#ifndef EDIT_TASK_WINDOW_H
#define EDIT_TASK_WINDOW_H

#include <QDialog>

namespace Ui {
class edit_task_window;
}

class edit_task_window : public QDialog
{
    Q_OBJECT

public:
    explicit edit_task_window(const QString &nameFile, QWidget *parent = nullptr);
    ~edit_task_window();

private slots:
    void on_pushButton_clicked();

    void on_doneBtn_clicked();

private:
    Ui::edit_task_window *ui;

    QString nameFile;
};

#endif // EDIT_TASK_WINDOW_H
