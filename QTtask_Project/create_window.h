#ifndef CREATE_WINDOW_H
#define CREATE_WINDOW_H

#include <QDialog>

namespace Ui {
class create_window;
}

class create_window : public QDialog
{
    Q_OBJECT

public:
    explicit create_window(QWidget *parent = nullptr);
    ~create_window();

private slots:
    void on_closeBtn_clicked();

    void on_createBtn_clicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::create_window *ui;

    bool allow = false;
};

#endif // CREATE_WINDOW_H
