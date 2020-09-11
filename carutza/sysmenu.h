#ifndef SYSMENU_H
#define SYSMENU_H

#include "button.h"

class QuickLaunch;
class TheApp;
struct XwnSet;
class SystemMenu : public OdButton
{
	Q_OBJECT
public:
    explicit SystemMenu( QWidget2 *p, const QSize& size, const XwnSet& set,QWidget *parent = 0);
	virtual ~SystemMenu();
    void    update_menu(bool dockHiding = false);

public slots:
	void app_about();
protected:
	QAction *_about;
};

#endif
