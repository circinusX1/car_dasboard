#ifndef DIALOG_H
#define DIALOG_H

#include <QTimer>
#include <vector>
#include <QDialog>
#include <QListWidgetItem>
#include "playthread.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void run();
    void apply_freq(int fr);
private slots:


    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_valueChanged(int value);

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

    void timerEvent(QTimerEvent *event);
    void on_verticalSlider_sliderMoved(int position);

    void on_enter_returnPressed();

    void on_enter_editingFinished();

private:

private:
    QBasicTimer _timer;
    Ui::Dialog  *ui;
    PlayThread* _thr;
    int         _value  =0;
    int         _startf;
    int         _endf;
    int         _stepf;
    std::vector<int> _gains;
};

extern Dialog* PDLG;

#endif // DIALOG_H
