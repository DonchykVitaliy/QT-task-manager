#ifndef STAT_WIDGET_H
#define STAT_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTextEdit>

class stat_widget : public QWidget
{
    Q_OBJECT
public:
    explicit stat_widget(const QString &title, const QString &date, const QString &noteText, bool important, QWidget *parent = nullptr);

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

#endif // STAT_WIDGET_H
