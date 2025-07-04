#ifndef BASKET_WIDGET_H
#define BASKET_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTextEdit>

class basket_widget : public QWidget
{
    Q_OBJECT
public:
    explicit basket_widget(const QString &title, const QString &date, const QString &noteText, bool important, QWidget *parent = nullptr);

signals:
    void taskDeleted();
    void taskResume();

private:
    QLabel *titleLabel;
    QLabel *dateLabel;
    QTextEdit *noteTextEdit;
    QVBoxLayout *taskLayout;
    QPushButton *deleteButton;
    QPushButton *resumeButton;
};

#endif // BASKET_WIDGET_H
