#include "folders_widget.h"

folders_widget::folders_widget(const QString &title, const QString &count, QWidget *parent)
    : QWidget{parent}
{

    // Заголовок і дата
    QString noteName = title;
    noteName.chop(5);           //приберає з назви ".json"
    titleLabel = new QLabel(noteName, this);
    countLabel = new QLabel(tr("Кількість нотаток: ") + count, this);
    titleLabel->setStyleSheet("color: white; padding: 2px; font-size: 10pt;");
    countLabel->setStyleSheet("color: white; padding: 2px; font-size: 10pt;");

    //buttons
    openButton = new QPushButton(tr("Відкрити"), this);
    openButton->setMinimumHeight(30);
    openButton->setStyleSheet(
        "QPushButton {"
        "   font-size: 12pt;"
        "   border-style: outset;"
        "   border-width: 2px;"
        "   border-radius: 10px;"
        "   background-color: rgb(112, 91, 218);"
        "   color: white;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(92, 71, 198);"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgb(72, 51, 178);"
        "}"
        );

    //layout для кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(openButton);

    //main layout
    QVBoxLayout *textLayout = new QVBoxLayout;
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    textLayout->addWidget(titleLabel);
    textLayout->addStretch();
    textLayout->addWidget(countLabel);

    mainLayout->addLayout(textLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setStretch(0,3);
    mainLayout->setStretch(1,1);

    setLayout(mainLayout);

    //сигнали
    connect(openButton, &QPushButton::clicked, this, &folders_widget::openFolder);

    setFixedHeight(60);
}

void folders_widget::changeEvent(QEvent *event)
{
    QString Count = "1";
    if (event->type() == QEvent::LanguageChange) {
        openButton->setText(tr("Відкрити")); // Оновлюємо текст кнопки
        countLabel->setText(tr("Кількість нотаток: ") + Count); // Оновлюємо текст мітки
        qDebug() << "folders_widget UI retranslated."; // Для налагодження
    }

    QWidget::changeEvent(event);
}
