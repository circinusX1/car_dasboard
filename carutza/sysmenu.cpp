#include <QtDBus>
#include "sysmenu.h"
#include "panel.h"
#include "fxabout.h"
#include "theapp.h"
#include "mysett.h"

SystemMenu::SystemMenu(QWidget2 *p, const QSize& size, const XwnSet& set, QWidget *parent):
                        OdButton(p, size, set, parent)
{
    this->setFixedSize(size);
    set_image(CFG(_images), "logo.png");
	update_menu();
}

void SystemMenu::update_menu(bool bh)
{
    Q_UNUSED(bh);
    if (menu())
    {
		menu()->clear();
	}

	QMenu *menu = new QMenu(this);
    _about = new QAction("About dash", this);
	menu->addAction(_about);
    connect(_about, SIGNAL(triggered()), this, SLOT(app_about()));
    this->setMenu(menu);
	connect(this, SIGNAL(clicked()), this, SLOT(slot_show_menu()));

}

void SystemMenu::app_about()
{
    PA->app_about();
}

SystemMenu::~SystemMenu()
{
}

