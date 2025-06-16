#include "stat_widget.h"

stat_widget::stat_widget(const QString &title, const QString &date, const QString &noteText, bool important, QWidget *parent)
    : QWidget{parent}
{

    // Заголовок і дата
    titleLabel = new QLabel(title, this);
    dateLabel = new QLabel(date, this);
    dateLabel->setStyleSheet("color: white; padding: 2px;");
    titleLabel->setStyleSheet("color: white; padding: 2px;");

    // Текст нотатки
    QString formattedText = noteText.toHtmlEscaped().replace("\n", "<br>");
    noteTextEdit = new QTextEdit(formattedText, this);
    noteTextEdit->setReadOnly(true);
    noteTextEdit->setStyleSheet(R"(
    QTextEdit {
        background-color: #ebebeb;
        border: none;
        border-radius: 10px;
        padding: 5px;
    }

    QScrollBar:vertical {
        background: #dcdcdc;
        width: 10px;
        margin: 2px 0 2px 0;
        border-radius: 5px;
    }

    QScrollBar::handle:vertical {
        background: #a9a9a9;
        min-height: 20px;
        border-radius: 5px;
    }

    QScrollBar::handle:vertical:hover {
        background: #888;
    }
    QScrollBar::add-line:vertical,
    QScrollBar::sub-line:vertical {
        height: 0;
    }
    QScrollBar::add-page:vertical,
    QScrollBar::sub-page:vertical {
        background: none;
    }
)");

    // Кнопки
    deleteButton = new QPushButton("Видалити", this);
    resumeButton = new QPushButton("Відновити", this);
    deleteButton->setMinimumHeight(30);
    resumeButton->setMinimumHeight(30);
    resumeButton->setStyleSheet(
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
    deleteButton->setStyleSheet(
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

    setFixedHeight(170);
}

