#ifndef STAT_LINE_WIDGET_H
#define STAT_LINE_WIDGET_H

#include <QWidget>

class stat_line_widget : public QWidget
{
    Q_OBJECT
public:
    explicit stat_line_widget(QWidget *parent = nullptr);
    void setCounts(int yellow, int green, int red);

protected:
    void paintEvent(QPaintEvent *event) override;
signals:

private:
    int yellowCount = 0;
    int greenCount = 0;
    int redCount = 0;
};

#endif // STAT_LINE_WIDGET_H
