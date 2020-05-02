/********************************************************************************
** Form generated from reading UI file 'HashTool.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HASHTOOL_H
#define UI_HASHTOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HashToolClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *HashToolClass)
    {
        if (HashToolClass->objectName().isEmpty())
            HashToolClass->setObjectName(QStringLiteral("HashToolClass"));
        HashToolClass->resize(600, 400);
        menuBar = new QMenuBar(HashToolClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        HashToolClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(HashToolClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        HashToolClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(HashToolClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        HashToolClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(HashToolClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        HashToolClass->setStatusBar(statusBar);

        retranslateUi(HashToolClass);

        QMetaObject::connectSlotsByName(HashToolClass);
    } // setupUi

    void retranslateUi(QMainWindow *HashToolClass)
    {
        HashToolClass->setWindowTitle(QApplication::translate("HashToolClass", "HashTool", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class HashToolClass: public Ui_HashToolClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HASHTOOL_H
