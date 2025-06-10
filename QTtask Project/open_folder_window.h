#ifndef OPEN_FOLDER_WINDOW_H
#define OPEN_FOLDER_WINDOW_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>

namespace Ui {
class open_folder_window;
}

class open_folder_window : public QWidget
{
    Q_OBJECT

public:
    explicit open_folder_window(const QString &fileName, QWidget *parent = nullptr);
    ~open_folder_window();

private slots:

    void on_renameBtn_clicked();

    void on_deleteBtn_clicked();

private:
    Ui::open_folder_window *ui;

    void loadNotes();

    QScrollArea *scrollArea;
    QWidget *container;
    QVBoxLayout *layout;
    QString fileName;
};

#endif // OPEN_FOLDER_WINDOW_H
