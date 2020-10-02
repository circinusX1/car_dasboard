#ifndef DIALOG_H
#define DIALOG_H

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
private slots:


     void on_pushButton_9_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_valueChanged(int value);

    void on_lineEdit_returnPressed();

    void on_radioButton_clicked(bool checked);

    void on_radioButton_2_clicked(bool checked);

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    void _applyfreq(int fr);
private:
    Ui::Dialog *ui;
    PlayThread* _thr;
    int _value=0;
};

#endif // DIALOG_H
