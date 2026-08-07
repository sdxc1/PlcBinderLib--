/********************************************************************************
** Form generated from reading UI file 'libTest.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIBTEST_H
#define UI_LIBTEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_libTestClass
{
public:
    QWidget *centralWidget;
    QPushButton *m100;
    QLineEdit *rd100;
    QPushButton *wd100;
    QPushButton *m2;
    QPushButton *m1;
    QPushButton *pushButton;
    QLineEdit *lineEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *libTestClass)
    {
        if (libTestClass->objectName().isEmpty())
            libTestClass->setObjectName(QString::fromUtf8("libTestClass"));
        libTestClass->resize(600, 400);
        centralWidget = new QWidget(libTestClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        m100 = new QPushButton(centralWidget);
        m100->setObjectName(QString::fromUtf8("m100"));
        m100->setGeometry(QRect(0, 150, 75, 23));
        rd100 = new QLineEdit(centralWidget);
        rd100->setObjectName(QString::fromUtf8("rd100"));
        rd100->setGeometry(QRect(80, 10, 113, 20));
        wd100 = new QPushButton(centralWidget);
        wd100->setObjectName(QString::fromUtf8("wd100"));
        wd100->setGeometry(QRect(0, 10, 75, 23));
        m2 = new QPushButton(centralWidget);
        m2->setObjectName(QString::fromUtf8("m2"));
        m2->setGeometry(QRect(0, 220, 75, 23));
        m1 = new QPushButton(centralWidget);
        m1->setObjectName(QString::fromUtf8("m1"));
        m1->setGeometry(QRect(140, 150, 75, 23));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(0, 260, 75, 23));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(0, 190, 113, 20));
        libTestClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(libTestClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        libTestClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(libTestClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        libTestClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(libTestClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        libTestClass->setStatusBar(statusBar);

        retranslateUi(libTestClass);

        QMetaObject::connectSlotsByName(libTestClass);
    } // setupUi

    void retranslateUi(QMainWindow *libTestClass)
    {
        libTestClass->setWindowTitle(QCoreApplication::translate("libTestClass", "libTest", nullptr));
        m100->setText(QCoreApplication::translate("libTestClass", "\347\275\256\344\270\272M100", nullptr));
        wd100->setText(QCoreApplication::translate("libTestClass", "\350\257\273D1000", nullptr));
        m2->setText(QCoreApplication::translate("libTestClass", "\345\217\226\345\217\215M2", nullptr));
        m1->setText(QCoreApplication::translate("libTestClass", "\345\217\226\345\217\215M1", nullptr));
        pushButton->setText(QCoreApplication::translate("libTestClass", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class libTestClass: public Ui_libTestClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIBTEST_H
