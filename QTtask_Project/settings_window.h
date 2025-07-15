#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include <QDialog>
#include <QString>
#include <QJsonValue>
#include <QTranslator>

namespace Ui {
class settings_window;
}

class settings_window : public QDialog
{
    Q_OBJECT

public:
    explicit settings_window(QWidget *parent = nullptr);
    ~settings_window();

private slots:
    void updateSettingValue(const QString &key, QJsonValue newValue);

    void on_closeBut_clicked();

    void on_basketCB_stateChanged(int arg1);

    void on_notificationCB_stateChanged(int arg1);

    void on_historyCB_stateChanged(int arg1);

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_activated(int index);

private:
    Ui::settings_window *ui;
    QTranslator *activeTranslator = nullptr;
};

#endif // SETTINGS_WINDOW_H
