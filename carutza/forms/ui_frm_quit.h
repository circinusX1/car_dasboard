/********************************************************************************
** Form generated from reading UI file 'frm_quit.ui'
**
** Created: Wed Aug 28 12:10:00 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRM_QUIT_H
#define UI_FRM_QUIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_quitDlg
{
public:
    QGridLayout *gridLayout;
    QLabel *quitIcon;
    QLabel *question;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_2;
    QLabel *reminder;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelBtn;
    QPushButton *quitBtn;

    void setupUi(QDialog *quitDlg)
    {
        if (quitDlg->objectName().isEmpty())
            quitDlg->setObjectName(QString::fromUtf8("quitDlg"));
        quitDlg->resize(438, 148);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(quitDlg->sizePolicy().hasHeightForWidth());
        quitDlg->setSizePolicy(sizePolicy);
        quitDlg->setModal(true);
        gridLayout = new QGridLayout(quitDlg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        quitIcon = new QLabel(quitDlg);
        quitIcon->setObjectName(QString::fromUtf8("quitIcon"));
        sizePolicy.setHeightForWidth(quitIcon->sizePolicy().hasHeightForWidth());
        quitIcon->setSizePolicy(sizePolicy);
        quitIcon->setMaximumSize(QSize(64, 64));
        QFont font;
        font.setKerning(false);
        quitIcon->set_font(font);
        quitIcon->setScaledContents(true);

        gridLayout->addWidget(quitIcon, 0, 0, 2, 1);

        question = new QLabel(quitDlg);
        question->setObjectName(QString::fromUtf8("question"));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        question->set_font(font1);
        question->setWordWrap(true);

        gridLayout->addWidget(question, 0, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(88, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 1, 1, 1);

        reminder = new QLabel(quitDlg);
        reminder->setObjectName(QString::fromUtf8("reminder"));
        reminder->setWordWrap(true);

        gridLayout->addWidget(reminder, 1, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 14, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 2, 2, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(148, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancelBtn = new QPushButton(quitDlg);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));
        cancelBtn->set_font(font1);
        cancelBtn->setAutoDefault(false);

        horizontalLayout->addWidget(cancelBtn);

        quitBtn = new QPushButton(quitDlg);
        quitBtn->setObjectName(QString::fromUtf8("quitBtn"));
        quitBtn->set_font(font1);
        quitBtn->setAutoDefault(false);
        quitBtn->setDefault(true);

        horizontalLayout->addWidget(quitBtn);


        gridLayout->addLayout(horizontalLayout, 3, 0, 1, 4);


        retranslateUi(quitDlg);
        QObject::connect(cancelBtn, SIGNAL(clicked()), quitDlg, SLOT(reject()));
        QObject::connect(quitBtn, SIGNAL(clicked()), quitDlg, SLOT(accept()));

        QMetaObject::connectSlotsByName(quitDlg);
    } // setupUi

    void retranslateUi(QDialog *quitDlg)
    {
        quitDlg->setWindowTitle(QApplication::translate("quitDlg", "Dialog", 0, QApplication::UnicodeUTF8));
        quitIcon->setText(QString());
        question->setText(QApplication::translate("quitDlg", "--", 0, QApplication::UnicodeUTF8));
        reminder->setText(QApplication::translate("quitDlg", "--", 0, QApplication::UnicodeUTF8));
        cancelBtn->setText(QApplication::translate("quitDlg", "Cancel", 0, QApplication::UnicodeUTF8));
        quitBtn->setText(QApplication::translate("quitDlg", "-", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class quitDlg: public Ui_quitDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRM_QUIT_H
