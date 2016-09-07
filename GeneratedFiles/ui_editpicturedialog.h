/********************************************************************************
** Form generated from reading UI file 'editpicturedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITPICTUREDIALOG_H
#define UI_EDITPICTUREDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EditPictureDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    QLineEdit *nameLineEdit;
    QLabel *suffixLabel;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_3;
    QSpinBox *resXSpinBox;
    QLabel *label_4;
    QSpinBox *resYSpinBox;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QDialog *EditPictureDialog)
    {
        if (EditPictureDialog->objectName().isEmpty())
            EditPictureDialog->setObjectName(QStringLiteral("EditPictureDialog"));
        EditPictureDialog->resize(300, 150);
        verticalLayout_2 = new QVBoxLayout(EditPictureDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_2 = new QLabel(EditPictureDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(12);
        label_2->setFont(font);

        horizontalLayout->addWidget(label_2);

        nameLineEdit = new QLineEdit(EditPictureDialog);
        nameLineEdit->setObjectName(QStringLiteral("nameLineEdit"));
        nameLineEdit->setMinimumSize(QSize(101, 21));
        nameLineEdit->setFont(font);

        horizontalLayout->addWidget(nameLineEdit);

        suffixLabel = new QLabel(EditPictureDialog);
        suffixLabel->setObjectName(QStringLiteral("suffixLabel"));
        suffixLabel->setFont(font);

        horizontalLayout->addWidget(suffixLabel);

        horizontalSpacer_2 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(17, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_4 = new QSpacerItem(13, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        label_3 = new QLabel(EditPictureDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);

        horizontalLayout_2->addWidget(label_3);

        resXSpinBox = new QSpinBox(EditPictureDialog);
        resXSpinBox->setObjectName(QStringLiteral("resXSpinBox"));
        resXSpinBox->setMinimumSize(QSize(81, 20));
        resXSpinBox->setFont(font);
        resXSpinBox->setMinimum(16);
        resXSpinBox->setMaximum(4096);

        horizontalLayout_2->addWidget(resXSpinBox);

        label_4 = new QLabel(EditPictureDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font);

        horizontalLayout_2->addWidget(label_4);

        resYSpinBox = new QSpinBox(EditPictureDialog);
        resYSpinBox->setObjectName(QStringLiteral("resYSpinBox"));
        resYSpinBox->setMinimumSize(QSize(81, 20));
        resYSpinBox->setFont(font);
        resYSpinBox->setMinimum(16);
        resYSpinBox->setMaximum(4096);

        horizontalLayout_2->addWidget(resYSpinBox);

        horizontalSpacer_3 = new QSpacerItem(13, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_3 = new QSpacerItem(17, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        pushButton = new QPushButton(EditPictureDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setFont(font);

        horizontalLayout_3->addWidget(pushButton);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        pushButton_2 = new QPushButton(EditPictureDialog);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setFont(font);

        horizontalLayout_3->addWidget(pushButton_2);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer_4 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(EditPictureDialog);

        QMetaObject::connectSlotsByName(EditPictureDialog);
    } // setupUi

    void retranslateUi(QDialog *EditPictureDialog)
    {
        EditPictureDialog->setWindowTitle(QApplication::translate("EditPictureDialog", "Dialog", 0));
        label_2->setText(QApplication::translate("EditPictureDialog", "\346\226\207\344\273\266\345\220\215\357\274\232", 0));
        suffixLabel->setText(QString());
        label_3->setText(QApplication::translate("EditPictureDialog", "\345\210\206\350\276\250\347\216\207\357\274\232", 0));
        label_4->setText(QApplication::translate("EditPictureDialog", "X", 0));
        pushButton->setText(QApplication::translate("EditPictureDialog", "\347\241\256\345\256\232", 0));
        pushButton_2->setText(QApplication::translate("EditPictureDialog", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class EditPictureDialog: public Ui_EditPictureDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITPICTUREDIALOG_H
