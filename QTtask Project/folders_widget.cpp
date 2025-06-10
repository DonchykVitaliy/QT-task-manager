#include "folders_widget.h"

folders_widget::folders_widget(const QString &title, const QString &count, QWidget *parent)
    : QWidget{parent}
{

    // Заголовок і дата
    QString noteName = title;
    noteName.chop(5);           //приберає з назви ".json"
    titleLabel = new QLabel(noteName, this);
    countLabel = new QLabel("Кількість нотаток: " + count, this);

    //buttons
    openButton = new QPushButton("Відкрити", this);
    openButton->setStyleSheet("background-color: red; color: white; font-weight: bold;");

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
}
