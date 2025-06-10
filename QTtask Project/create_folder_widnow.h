#ifndef CREATE_FOLDER_WIDNOW_H
#define CREATE_FOLDER_WIDNOW_H

#include <QDialog>
#include <QDir>

namespace Ui {
class create_folder_widnow;
}

class create_folder_widnow : public QDialog
{
    Q_OBJECT

public:
    explicit create_folder_widnow(QWidget *parent = nullptr);
    ~create_folder_widnow();

private slots:
    void on_closeBtn_clicked();

    void on_createBtn_clicked();

private:
    Ui::create_folder_widnow *ui;

    void loadList(QStringList fileList, QDir dir);

    //список всіх нотаток
    QStringList m_fileList;  // список імен файлів
    QDir m_dir;              // директорія з нотатками
};

#endif // CREATE_FOLDER_WIDNOW_H
