#ifndef FOLDERS_WINDOW_H
#define FOLDERS_WINDOW_H

#include <QDialog>
#include <QFileSystemWatcher>
#include <QScrollArea>
#include <QVBoxLayout>

namespace Ui {
class folders_window;
}

class folders_window : public QDialog
{
    Q_OBJECT

public:
    explicit folders_window(QWidget *parent = nullptr);
    ~folders_window();

private slots:
    void on_createBtn_clicked();

    void displayFolders();

    void on_closeBtn_clicked();

private:
    Ui::folders_window *ui;

    QFileSystemWatcher *watcher;  // Для спостереження за змінами у папці
    QScrollArea *scrollArea;      // Для відображення кастомних віджетів
    QWidget *container;           // Контейнер для віджетів
    QVBoxLayout *layout;          // Layout для розміщення нотаток
};

#endif // FOLDERS_WINDOW_H
