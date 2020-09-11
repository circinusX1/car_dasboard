#ifndef CLOCKWINDOW_H
#define CLOCKWINDOW_H

#include <QMainWindow>

namespace Ui {
class ClockWindow;
}
class dashdbIf;
class ClockWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ClockWindow(QWidget *parent = 0);
    ~ClockWindow();
    void setup(dashdbIf* client);
public slots:
    void click_on(const QString& s);
private slots:
    void on_pushButton_clicked();
    void notify_update();

private:
    Ui::ClockWindow* ui;
    dashdbIf*        _client;
    QTimer*         _timer;
    QString         _format;
};

#endif // CLOCKWINDOW_H
