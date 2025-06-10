#include "stat_widget.h"

stat_widget::stat_widget(const QString &title, const QString &date, const QString &noteText, bool important, QWidget *parent)
    : QWidget{parent}
{

    // Заголовок і дата
titleLabel = new QLabel(title, this);
    dateLabel = new QLabel(date, this);
    if (important) {
        titleLabel->setStyleSheet("color: red; font-weight: bold;");
    }

    // Текст нотатки
    QString formattedText = noteText.toHtmlEscaped().replace("\n", "<br>");
    noteTextEdit = new QTextEdit(formattedText, this);
    noteTextEdit->setReadOnly(true);
    noteTextEdit->setStyleSheet("background-color: #c7c7c7; border: none; padding: 5px;");

    // Кнопки
    deleteButton = new QPushButton("Видалити", this);
    resumeButton = new QPushButton("Відновити", this);
    resumeButton->setStyleSheet("background-color: red; color: white; font-weight: bold;");
    deleteButton->setStyleSheet("background-color: red; color: white; font-weight: bold;");

    // Layout для кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(resumeButton);

    // Основний layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(dateLabel);

    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(noteTextEdit);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Підключаємо сигнали кнопок
    connect(deleteButton, &QPushButton::clicked, this, &stat_widget::taskDeleted);
    connect(resumeButton, &QPushButton::clicked, this, &stat_widget::taskResume);
}

