#include "task_widget.h"

task_widget::task_widget(const QString &title, const QString &date, const QString &noteText, bool important, QWidget *parent)
    : QWidget{parent}
{

    // Заголовок і дата
    titleLabel = new QLabel(title, this);
    dateLabel = new QLabel(date, this);

    // Текст нотатки
    QString formattedText = noteText.toHtmlEscaped().replace("\n", "<br>");
    noteTextEdit = new QTextEdit(formattedText, this);
    noteTextEdit->setReadOnly(true);
    noteTextEdit->setStyleSheet("background-color: #c7c7c7; border: none; padding: 5px;");

    // Кнопки
    editButton = new QPushButton("E", this);
    completeButton = new QPushButton("Виконано", this);
    deleteButton = new QPushButton("Видалити", this);
    completeButton->setStyleSheet("background-color: red; color: white; font-weight: bold;");
    deleteButton->setStyleSheet("background-color: red; color: white; font-weight: bold;");

    // Layout для кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(completeButton);
    buttonLayout->addWidget(deleteButton);

    // якщо ввімкнене нагадування
    if (important) {
        dateLabel->setStyleSheet("color: red; font-weight: bold;");
        notificationButton = new QPushButton("Вимкнути нагадування", this);
        notificationButton->setStyleSheet("background-color: red; color: white; font-weight: bold;");
        buttonLayout->addWidget(notificationButton);
    }

    // Основний layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(editButton);
    headerLayout->addWidget(dateLabel);

    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(noteTextEdit);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Підключаємо сигнали кнопок
    connect(completeButton, &QPushButton::clicked, this, &task_widget::taskCompleted);
    connect(deleteButton, &QPushButton::clicked, this, &task_widget::taskDeleted);
    connect(editButton, &QPushButton::clicked, this, &task_widget::taskEdit);

    setFixedHeight(170); // або setMaximumHeight(150);
}

