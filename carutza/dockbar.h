
// create the dockbar on bottom of the _get_screen

#ifndef DOCKBAR_H
#define DOCKBAR_H

#include "defs.h"

#define MAX_DOCK_HEIGHT 48

class XClient;
class DockIcon;
class TheApp;

typedef QList<DockIcon *> DockIconsList;

class Dockbar : public QLabel
{
    Q_OBJECT
public:

	enum DockState {
        DOCK_DEFAULT,
        DOCK_HIDDEN,
        DOCK_PROG_OFF,
        DOCK_PROG_ON,
        DOCK_APEARING
	};

    Dockbar(TheApp *a, QWidget *parent=0);
    ~Dockbar();

	void set_autohide(bool active=true);
    void add_xcli(XClient *);
	bool remove_xcli(XClient *);
	bool remove_childs();
	void set_size_dims(int factor, bool doSave=true);
    void refresh();
    void load_config();
	int  instant_height();

public slots:
    void slot_remove_icon(DockIcon *);
	void slot_onoff_dock();
	void slot_animate();
	void slot_animate_show();
	void slot_hide_1();
	void slot_hide_2();

signals:
	void clientRemoved(XClient *);

private:
	bool checkCursor();

private:
    QHBoxLayout     *dockLayout;
    QHBoxLayout     *iconLayout;
    QFrame          *dockFrame;
    DockIconsList   *iconsList;
    int             dockFactor; 	// Dockbar set_size factor (0,100) percents
    QString         dockPix;	//
	QTimer          *timer, *hideTimer;
	int             dockState;
	bool            autoHide;
};
#endif

