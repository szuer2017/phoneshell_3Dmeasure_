/********************************************************************************
** Form generated from reading UI file 'pclviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PCLVIEWER_H
#define UI_PCLVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_PCLViewer
{
public:
    QWidget *centralwidget;
    QVTKWidget *qvtkWidget;
    QToolButton *Openfile;
    QToolButton *filtered;
    QToolButton *boss_height;
    QToolButton *extract;
    QToolButton *toolButton;
    QLabel *label;
    QToolButton *save;
    QToolButton *boundry;

    void setupUi(QMainWindow *PCLViewer)
    {
        if (PCLViewer->objectName().isEmpty())
            PCLViewer->setObjectName(QStringLiteral("PCLViewer"));
        PCLViewer->setEnabled(true);
        PCLViewer->resize(1252, 750);
        PCLViewer->setMinimumSize(QSize(0, 0));
        PCLViewer->setMaximumSize(QSize(5000, 5000));
        PCLViewer->setMouseTracking(false);
        PCLViewer->setAcceptDrops(false);
        PCLViewer->setAutoFillBackground(false);
        PCLViewer->setStyleSheet(QStringLiteral("QWidget#centralwidget{ border-image: url(:/pclviewer/Resources/1.png);}"));
        PCLViewer->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        PCLViewer->setAnimated(true);
        PCLViewer->setDocumentMode(false);
        PCLViewer->setTabShape(QTabWidget::Rounded);
        centralwidget = new QWidget(PCLViewer);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        qvtkWidget = new QVTKWidget(centralwidget);
        qvtkWidget->setObjectName(QStringLiteral("qvtkWidget"));
        qvtkWidget->setGeometry(QRect(50, 130, 791, 591));
        qvtkWidget->setAutoFillBackground(true);
        Openfile = new QToolButton(centralwidget);
        Openfile->setObjectName(QStringLiteral("Openfile"));
        Openfile->setEnabled(true);
        Openfile->setGeometry(QRect(920, 140, 51, 41));
        QFont font;
        font.setFamily(QStringLiteral("Times New Roman"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        Openfile->setFont(font);
        Openfile->setAutoFillBackground(false);
        Openfile->setStyleSheet(QStringLiteral("QToolButton#Openfile{border-image: url(:/pclviewer/Resources/file.png);}"));
        Openfile->setCheckable(false);
        Openfile->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        Openfile->setAutoRaise(false);
        Openfile->setArrowType(Qt::NoArrow);
        filtered = new QToolButton(centralwidget);
        filtered->setObjectName(QStringLiteral("filtered"));
        filtered->setGeometry(QRect(920, 200, 61, 51));
        filtered->setFont(font);
        filtered->setStyleSheet(QLatin1String("QToolButton#filtered {border-image: url(:/pclviewer/Resources/fitplane.png);}\n"
""));
        filtered->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        boss_height = new QToolButton(centralwidget);
        boss_height->setObjectName(QStringLiteral("boss_height"));
        boss_height->setGeometry(QRect(1020, 200, 41, 51));
        boss_height->setFont(font);
        boss_height->setStyleSheet(QStringLiteral("QToolButton#boss_height{border-image: url(:/pclviewer/Resources/boss_height.png);}"));
        extract = new QToolButton(centralwidget);
        extract->setObjectName(QStringLiteral("extract"));
        extract->setGeometry(QRect(920, 260, 61, 51));
        QFont font1;
        font1.setFamily(QStringLiteral("Times New Roman"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        extract->setFont(font1);
        extract->setStyleSheet(QLatin1String("QToolButton#extract{border-image: url(:/pclviewer/Resources/fit_base.png);}\n"
""));
        toolButton = new QToolButton(centralwidget);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setEnabled(true);
        toolButton->setGeometry(QRect(910, 320, 161, 51));
        toolButton->setFont(font);
        toolButton->setContextMenuPolicy(Qt::DefaultContextMenu);
        toolButton->setLayoutDirection(Qt::LeftToRight);
        toolButton->setAutoFillBackground(false);
        QIcon icon;
        icon.addFile(QStringLiteral(":/pclviewer/Resources/rectLabelSymbol.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon);
        toolButton->setIconSize(QSize(65, 60));
        toolButton->setCheckable(false);
        toolButton->setAutoRepeat(false);
        toolButton->setAutoExclusive(false);
        toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolButton->setAutoRaise(false);
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 20, 261, 81));
        label->setStyleSheet(QStringLiteral("border-image: url(:/pclviewer/Resources/szu1.jpg);"));
        save = new QToolButton(centralwidget);
        save->setObjectName(QStringLiteral("save"));
        save->setEnabled(true);
        save->setGeometry(QRect(1010, 140, 61, 51));
        save->setFont(font);
        save->setAutoFillBackground(false);
        save->setStyleSheet(QStringLiteral("QToolButton#save{border-image: url(:/pclviewer/Resources/save.png);}"));
        boundry = new QToolButton(centralwidget);
        boundry->setObjectName(QStringLiteral("boundry"));
        boundry->setGeometry(QRect(1010, 260, 61, 61));
        boundry->setFont(font);
        boundry->setStyleSheet(QStringLiteral("QToolButton#boundry{border-image: url(:/pclviewer/Resources/border_outer.png);}"));
        PCLViewer->setCentralWidget(centralwidget);
        filtered->raise();
        boss_height->raise();
        extract->raise();
        Openfile->raise();
        toolButton->raise();
        qvtkWidget->raise();
        label->raise();
        save->raise();
        boundry->raise();

        retranslateUi(PCLViewer);
        QObject::connect(boundry, SIGNAL(clicked()), boundry, SLOT(update()));

        QMetaObject::connectSlotsByName(PCLViewer);
    } // setupUi

    void retranslateUi(QMainWindow *PCLViewer)
    {
        PCLViewer->setWindowTitle(QApplication::translate("PCLViewer", "PCLViewer", 0));
        Openfile->setText(QApplication::translate("PCLViewer", "File", 0));
        filtered->setText(QApplication::translate("PCLViewer", "Denoise", 0));
        boss_height->setText(QApplication::translate("PCLViewer", "size", 0));
        extract->setText(QApplication::translate("PCLViewer", "base", 0));
        toolButton->setText(QApplication::translate("PCLViewer", "Plane_Fit_Tool", 0));
        label->setText(QString());
        save->setText(QApplication::translate("PCLViewer", "save", 0));
        boundry->setText(QApplication::translate("PCLViewer", "boudry", 0));
    } // retranslateUi

};

namespace Ui {
    class PCLViewer: public Ui_PCLViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PCLVIEWER_H
