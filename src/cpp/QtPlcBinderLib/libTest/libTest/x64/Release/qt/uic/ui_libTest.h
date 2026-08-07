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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_libTestClass
{
public:
    QWidget *centralWidget;
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
    } // retranslateUi

};

namespace Ui {
    class libTestClass: public Ui_libTestClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIBTEST_H
