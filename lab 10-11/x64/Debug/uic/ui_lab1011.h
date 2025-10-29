/********************************************************************************
** Form generated from reading UI file 'lab1011.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAB1011_H
#define UI_LAB1011_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_lab1011Class
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *lab1011Class)
    {
        if (lab1011Class->objectName().isEmpty())
            lab1011Class->setObjectName("lab1011Class");
        lab1011Class->resize(600, 400);
        menuBar = new QMenuBar(lab1011Class);
        menuBar->setObjectName("menuBar");
        lab1011Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(lab1011Class);
        mainToolBar->setObjectName("mainToolBar");
        lab1011Class->addToolBar(mainToolBar);
        centralWidget = new QWidget(lab1011Class);
        centralWidget->setObjectName("centralWidget");
        lab1011Class->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(lab1011Class);
        statusBar->setObjectName("statusBar");
        lab1011Class->setStatusBar(statusBar);

        retranslateUi(lab1011Class);

        QMetaObject::connectSlotsByName(lab1011Class);
    } // setupUi

    void retranslateUi(QMainWindow *lab1011Class)
    {
        lab1011Class->setWindowTitle(QCoreApplication::translate("lab1011Class", "lab1011", nullptr));
    } // retranslateUi

};

namespace Ui {
    class lab1011Class: public Ui_lab1011Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAB1011_H
