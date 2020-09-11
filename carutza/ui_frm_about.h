/********************************************************************************
** Form generated from reading UI file 'frm_about.ui'
**
** Created: Wed Aug 28 12:10:00 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRM_ABOUT_H
#define UI_FRM_ABOUT_H

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

class Ui_aboutDlg
{
public:
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_4;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QLabel *osLbl;
    QLabel *label_5;
    QLabel *vendorLbl;
    QLabel *modelLbl;
    QLabel *speedLbl;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_2;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_13;
    QLabel *label_12;
    QLabel *memoryLbl;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *closeBtn;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_3;
    QLabel *label;
    QLabel *versionLbl;

    void setupUi(QDialog *aboutDlg)
    {
        if (aboutDlg->objectName().isEmpty())
            aboutDlg->setObjectName(QString::fromUtf8("aboutDlg"));
        aboutDlg->resize(416, 478);
        gridLayout_2 = new QGridLayout(aboutDlg);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalSpacer_3 = new QSpacerItem(114, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_3, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(151, 86, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(150, 86, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(114, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_2, 2, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_4, 5, 1, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_4 = new QLabel(aboutDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QFont font;
        font.setPointSize(8);
        font.setBold(true);
        font.setWeight(75);
        label_4->set_font(font);
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        osLbl = new QLabel(aboutDlg);
        osLbl->setObjectName(QString::fromUtf8("osLbl"));
        QFont font1;
        font1.setPointSize(8);
        osLbl->set_font(font1);

        gridLayout->addWidget(osLbl, 0, 1, 1, 1);

        label_5 = new QLabel(aboutDlg);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->set_font(font);
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 1, 0, 1, 1);

        vendorLbl = new QLabel(aboutDlg);
        vendorLbl->setObjectName(QString::fromUtf8("vendorLbl"));
        vendorLbl->set_font(font1);

        gridLayout->addWidget(vendorLbl, 1, 1, 1, 1);

        modelLbl = new QLabel(aboutDlg);
        modelLbl->setObjectName(QString::fromUtf8("modelLbl"));
        modelLbl->set_font(font1);

        gridLayout->addWidget(modelLbl, 2, 1, 1, 1);

        speedLbl = new QLabel(aboutDlg);
        speedLbl->setObjectName(QString::fromUtf8("speedLbl"));
        speedLbl->set_font(font1);

        gridLayout->addWidget(speedLbl, 3, 1, 1, 1);

        label_6 = new QLabel(aboutDlg);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->set_font(font);
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        label_7 = new QLabel(aboutDlg);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->set_font(font);
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        label_8 = new QLabel(aboutDlg);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->set_font(font);
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_8, 7, 0, 1, 1);

        label_9 = new QLabel(aboutDlg);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->set_font(font);
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_9, 8, 0, 1, 1);

        label_2 = new QLabel(aboutDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->set_font(font1);

        gridLayout->addWidget(label_2, 7, 1, 1, 1);

        label_10 = new QLabel(aboutDlg);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->set_font(font1);

        gridLayout->addWidget(label_10, 5, 1, 1, 1);

        label_11 = new QLabel(aboutDlg);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->set_font(font1);

        gridLayout->addWidget(label_11, 6, 1, 1, 1);

        label_13 = new QLabel(aboutDlg);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->set_font(font1);

        gridLayout->addWidget(label_13, 9, 1, 1, 1);

        label_12 = new QLabel(aboutDlg);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->set_font(font1);

        gridLayout->addWidget(label_12, 8, 1, 1, 1);

        memoryLbl = new QLabel(aboutDlg);
        memoryLbl->setObjectName(QString::fromUtf8("memoryLbl"));
        memoryLbl->set_font(font1);

        gridLayout->addWidget(memoryLbl, 4, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 6, 0, 1, 3);

        verticalSpacer = new QSpacerItem(114, 7, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 7, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(151, 24, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        closeBtn = new QPushButton(aboutDlg);
        closeBtn->setObjectName(QString::fromUtf8("closeBtn"));
        QFont font2;
        font2.setBold(true);
        font2.setWeight(75);
        closeBtn->set_font(font2);
        closeBtn->setDefault(true);

        horizontalLayout->addWidget(closeBtn);

        horizontalSpacer_4 = new QSpacerItem(150, 24, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        gridLayout_2->addLayout(horizontalLayout, 8, 0, 1, 3);

        label_3 = new QLabel(aboutDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setPixmap(Imagez("","logo.png"));
        label_3->setScaledContents(true);

        gridLayout_2->addWidget(label_3, 1, 1, 1, 1);

        label = new QLabel(aboutDlg);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font3;
        font3.setPointSize(12);
        font3.setBold(true);
        font3.setWeight(75);
        label->set_font(font3);
        label->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label, 3, 1, 1, 1);

        versionLbl = new QLabel(aboutDlg);
        versionLbl->setObjectName(QString::fromUtf8("versionLbl"));
        versionLbl->set_font(font1);
        versionLbl->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(versionLbl, 4, 1, 1, 1);


        retranslateUi(aboutDlg);
        QObject::connect(closeBtn, SIGNAL(clicked()), aboutDlg, SLOT(accept()));

        QMetaObject::connectSlotsByName(aboutDlg);
    } // setupUi

    void retranslateUi(QDialog *aboutDlg)
    {
        aboutDlg->setWindowTitle(QApplication::translate("aboutDlg", "Dialog", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("aboutDlg", "Operating System:", 0, QApplication::UnicodeUTF8));
        osLbl->setText(QApplication::translate("aboutDlg", "GNU/Linux", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("aboutDlg", "Processor:", 0, QApplication::UnicodeUTF8));
        vendorLbl->setText(QApplication::translate("aboutDlg", "Processor vendor", 0, QApplication::UnicodeUTF8));
        modelLbl->setText(QApplication::translate("aboutDlg", "Processor model", 0, QApplication::UnicodeUTF8));
        speedLbl->setText(QApplication::translate("aboutDlg", "Processor speed/freq", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("aboutDlg", "Memory:", 0, QApplication::UnicodeUTF8));
QApplication::UnicodeUTF8));
        closeBtn->setText(QApplication::translate("aboutDlg", "Close", 0, QApplication::UnicodeUTF8));
        label_3->setText(QString());
        label->setText(QApplication::translate("aboutDlg", "FLEXCAROWA", 0, QApplication::UnicodeUTF8));
        versionLbl->setText(QApplication::translate("aboutDlg", "ver. ....", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class aboutDlg: public Ui_aboutDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRM_ABOUT_H
