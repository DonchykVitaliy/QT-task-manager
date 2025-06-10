#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemWatcher>
#include <QScrollArea>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_createBut_clicked();
    void updateNoteList();

    void countComplitedNote();

    void on_basketBut_clicked();

    void on_statBut_clicked();

    void on_setingsBut_clicked();

    void on_searchText_textChanged();

    void on_clearBut_clicked();

    void on_calendarBut_clicked();

    void on_foldersBut_clicked();

private:
    Ui::MainWindow *ui;

    // Нові об'єкти
    QFileSystemWatcher *watcher;  // Для спостереження за змінами у папці
    QScrollArea *scrollArea;      // Для відображення кастомних віджетів
    QWidget *container;           // Контейнер для віджетів
    QVBoxLayout *layout;          // Layout для розміщення нотаток
};
#endif // MAINWINDOW_H
