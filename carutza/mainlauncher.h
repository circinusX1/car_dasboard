#ifndef __MAINLAUNCER_H
#define __MAINLAUNCER_H

#include <QObject>
#include <QWidget>
#include <QPropertyAnimation>
#include "defs.h"
#include "qwidget2.h"
#include "dskwatch.h"
#include <vector>

#define TOP_Lancer_HEIGHT 64

class TheApp;
class XClient;
class LunchButt;
class OdButton;
struct XwnSet;
class XPannel : public QWidget2
{
    Q_OBJECT
public:
    explicit XPannel(TheApp *a, QWidget *parent=0);
    virtual ~XPannel(void);

    void    refresh_buts();
    void    ahide(bool hide);
public slots:
    void scrool_lancer(LunchButt* lb, int dx ,int dy);
    void done_scrolling(int);
    void change_folder(LunchButt* pb);
    void run_app(LunchButt* pb);
    void slot_align_edge();
    void slot_anim_done();
    void slot_floder_ch(const QString& folder);
    void process_key(int key);
    void slot_change_folder();
private:
    void    _load_shortcuts(const QString& folder);
    void    _config_ui(void);
    void    _done_scrooling(int);
    void    _scrool_lancer(int dx ,int dy);
    void    _add_button(const XwnSet& xs, int index) ;
    size_t  _load_buts(const QString& folder, const QSize& sz, std::vector<XwnSet>& buts);
    void    _calc_overflow();
    void    _update_btnsel();
protected:
	virtual void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);


    QBoxLayout*      _layout;
	OdButton *      _active_widget;
	WId             _active_wigget_id;
    QString         _basefolder;
    QString         _curfolder;
    bool            _abtomove;
    QPoint          _presspt;
    QPropertyAnimation *_panimation;
    QTimer          _timer;
    bool            _moved;
    int             _icwidth;
    std::vector<LunchButt*> _butons;
    DskWatch              _inotify;
    QIcon*          _showmore;
    int             _overflow;
    int             _layerwidth;
    int             _selected;
    bool            _hidden;
    bool            _folderchanged;
};


#endif

