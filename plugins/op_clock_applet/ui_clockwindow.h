/********************************************************************************
** Form generated from reading UI file 'clockwindow.ui'
**
** Created: Fri Sep 20 23:01:15 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLOCKWINDOW_H
#define UI_CLOCKWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QDial>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClockWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QDateTimeEdit *dateTimeEdit;
    QDial *dial;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ClockWindow)
    {
        if (ClockWindow->objectName().isEmpty())
            ClockWindow->setObjectName(QString::fromUtf8("ClockWindow"));
        ClockWindow->resize(400, 300);
        centralWidget = new QWidget(ClockWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(130, 160, 107, 29));
        dateTimeEdit = new QDateTimeEdit(centralWidget);
        dateTimeEdit->setObjectName(QString::fromUtf8("dateTimeEdit"));
        dateTimeEdit->setGeometry(QRect(100, 30, 194, 31));
        dial = new QDial(centralWidget);
        dial->setObjectName(QString::fromUtf8("dial"));
        dial->setGeometry(QRect(70, 80, 50, 64));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 210, 321, 21));
        ClockWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ClockWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 25));
        ClockWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ClockWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ClockWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ClockWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ClockWindow->setStatusBar(statusBar);

        retranslateUi(ClockWindow);

        QMetaObject::connectSlotsByName(ClockWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ClockWindow)
    {
        ClockWindow->setWindowTitle(QApplication::translate("ClockWindow", "ClockWindow", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("ClockWindow", "HIDE", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ClockWindow", "Suppose to setup the time here", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ClockWindow: public Ui_ClockWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOCKWINDOW_H
