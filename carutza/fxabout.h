#ifndef FXABOUT_H
#define FXABOUT_H

#include <QDialog>

namespace Ui {
class FxAbout;
}

class FxAbout : public QDialog
{
    Q_OBJECT
    
public:
    explicit FxAbout(QWidget *parent = 0);
    ~FxAbout();
    
private:
    Ui::FxAbout *ui;
};

#endif // FXABOUT_H
