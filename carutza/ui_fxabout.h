/********************************************************************************
** Form generated from reading UI file 'fxabout.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FXABOUT_H
#define UI_FXABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_FxAbout
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QDialog *FxAbout)
    {
        if (FxAbout->objectName().isEmpty())
            FxAbout->setObjectName(QStringLiteral("FxAbout"));
        FxAbout->resize(400, 237);
        buttonBox = new QDialogButtonBox(FxAbout);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 190, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(FxAbout);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(90, 40, 231, 21));
        label_2 = new QLabel(FxAbout);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(90, 80, 211, 31));
        label_3 = new QLabel(FxAbout);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(90, 130, 211, 16));

        retranslateUi(FxAbout);
        QObject::connect(buttonBox, SIGNAL(accepted()), FxAbout, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), FxAbout, SLOT(reject()));

        QMetaObject::connectSlotsByName(FxAbout);
    } // setupUi

    void retranslateUi(QDialog *FxAbout)
    {
        FxAbout->setWindowTitle(QApplication::translate("FxAbout", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("FxAbout", "TextLabel", Q_NULLPTR));
        label_2->setText(QApplication::translate("FxAbout", "TextLabel", Q_NULLPTR));
        label_3->setText(QApplication::translate("FxAbout", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FxAbout: public Ui_FxAbout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FXABOUT_H
