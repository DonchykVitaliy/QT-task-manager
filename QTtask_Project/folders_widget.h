#ifndef FOLDERS_WIDGET_H
#define FOLDERS_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QButtonGroup>

class folders_widget : public QWidget
{
    Q_OBJECT
public:
    explicit folders_widget(const QString &title, const QString &count,QWidget *parent = nullptr);

signals:
    void openFolder();

private:
    QLabel *titleLabel;
    QLabel *countLabel;
    QPushButton *openButton;

};

#endif // FOLDERS_WIDGET_H
