#include "stat_line_widget.h"
#include <QPainter>
#include <QRectF>
#include <QPainterPath>

stat_line_widget::stat_line_widget(QWidget *parent)
    : QWidget{parent}
{
    setFixedHeight(40);
}

void stat_line_widget::setCounts(int yellow, int green, int red)
{
    yellowCount = yellow;
    greenCount = green;
    redCount = red;
    update(); // перерисувати віджет
}

void stat_line_widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // згладжування

    const int r = 10;
    const int margin = 2;
    const int innerMargin = 4; // щоб кольорові лінії не стикались із краями
    QRectF backgroundRect(margin, margin, width() - 2 * margin, height() - 2 * margin);

    // Фон: заокруглений прямокутник
    painter.setBrush(QColor(230, 230, 230)); // світло-сірий фон
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(backgroundRect, r, r); // радіус заокруглення 10

    // Якщо немає даних — не малювати лінії
    int total = yellowCount + greenCount + redCount;
    if (total == 0)
        return;

    // Область всередині фону для кольорових сегментів
    QRectF contentRect = backgroundRect.adjusted(innerMargin, innerMargin, -innerMargin, -innerMargin);
    int x = contentRect.left();
    int y = contentRect.top();
    int h = contentRect.height();
    int totalW = contentRect.width();

    // Обчислення ширин сегментів
    int wY = totalW * yellowCount / total;
    int wG = totalW * greenCount / total;
    int wR = totalW - wY - wG;

    // Малювання сегментів
    QPainterPath yellowPath;
    yellowPath.moveTo(x + r, y);
    yellowPath.arcTo(x, y, 2 * r, 2 * r, 90, 90); // верхній лівий кут
    yellowPath.lineTo(x, y + h - r);
    yellowPath.arcTo(x, y + h - 2 * r, 2 * r, 2 * r, 180, 90); // нижній лівий
    yellowPath.lineTo(x + wY, y + h);
    yellowPath.lineTo(x + wY, y);
    yellowPath.closeSubpath();
    painter.setBrush(Qt::yellow);
    painter.drawPath(yellowPath);


    painter.setBrush(Qt::green);
    painter.drawRect(QRectF(x + wY, y, wG, h));

    int redX = x + wY + wG;
    QPainterPath redPath;
    redPath.moveTo(redX, y);
    redPath.lineTo(redX + wR - r, y);
    redPath.arcTo(redX + wR - 2 * r, y, 2 * r, 2 * r, 90, -90); // верхній правий
    redPath.lineTo(redX + wR, y + h - r);
    redPath.arcTo(redX + wR - 2 * r, y + h - 2 * r, 2 * r, 2 * r, 0, -90); // нижній правий
    redPath.lineTo(redX, y + h);
    redPath.closeSubpath();
    painter.setBrush(Qt::red);
    painter.drawPath(redPath);
}

