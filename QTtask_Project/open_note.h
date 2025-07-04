#ifndef OPEN_NOTE_H
#define OPEN_NOTE_H

#include <QWidget>

namespace Ui {
class open_note;
}

class open_note : public QWidget
{
    Q_OBJECT

public:
    explicit open_note(const QString &fileName, QWidget *parent = nullptr);
    ~open_note();

private slots:
    void on_notfBtn_clicked();

    void on_deleteBtn_clicked();

    void on_doneBtn_clicked();

private:
    Ui::open_note *ui;
    QString fileName;
};

#endif // OPEN_NOTE_H
