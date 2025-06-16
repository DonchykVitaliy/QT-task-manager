#ifndef TASK_WIDGET_H
#define TASK_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTextEdit>

class task_widget : public QWidget
{
    Q_OBJECT
public:
    explicit task_widget(const QString &title, const QString &date, const QString &noteText, bool important, QWidget *parent = nullptr);

signals:
    void taskCompleted();
    void taskDeleted();
    void taskEdit();
    void taskNotf();

private:
    QLabel *titleLabel;
    QLabel *dateLabel;
    QTextEdit *noteTextEdit;
    QVBoxLayout *taskLayout;
    QPushButton *editButton;
    QPushButton *completeButton;
    QPushButton *deleteButton;
    QPushButton *notificationButton;
};

#endif // TASK_WIDGET_H
