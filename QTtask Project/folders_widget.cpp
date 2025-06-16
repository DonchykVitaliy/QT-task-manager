#include "folders_widget.h"

folders_widget::folders_widget(const QString &title, const QString &count, QWidget *parent)
    : QWidget{parent}
{

    // Заголовок і дата
    QString noteName = title;
    noteName.chop(5);           //приберає з назви ".json"
    titleLabel = new QLabel(noteName, this);
    countLabel = new QLabel("Кількість нотаток: " + count, this);
    titleLabel->setStyleSheet("color: white; padding: 2px; font-size: 10pt;");
    countLabel->setStyleSheet("color: white; padding: 2px; font-size: 10pt;");

    //buttons
    openButton = new QPushButton("Відкрити", this);
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
