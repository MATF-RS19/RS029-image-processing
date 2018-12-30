/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <displaylabel.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_7;
    QFrame *frame;
    QGridLayout *gridLayout;
    displayLabel *displayImageLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QFormLayout *formLayout;
    QToolBox *toolBox;
    QWidget *page;
    QVBoxLayout *verticalLayout_2;
    QPushButton *openImageButton;
    QPushButton *saveImageButton;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_3;
    QPushButton *binarizeKmeansButton;
    QPushButton *binarizeFcmButton;
    QWidget *page_2;
    QGridLayout *gridLayout_2;
    QPushButton *posterizeButton;
    QSpinBox *posterSpinBox;
    QWidget *EdgeDetection;
    QVBoxLayout *verticalLayout_4;
    QSlider *cannySlider;
    QPushButton *cannyButton;
    QSlider *fuzzySlider;
    QPushButton *fuzzyButton;
    QWidget *page_4;
    QVBoxLayout *verticalLayout_5;
    QSpinBox *pcaSpinBox;
    QPushButton *pcaButton;
    QWidget *page_5;
    QVBoxLayout *verticalLayout_6;
    QPushButton *distortionButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(781, 435);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_7 = new QVBoxLayout(centralWidget);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setToolTipDuration(-1);
        frame->setStyleSheet(QStringLiteral(""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame->setLineWidth(0);
        gridLayout = new QGridLayout(frame);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(10, 10, 10, 10);
        displayImageLabel = new displayLabel(frame);
        displayImageLabel->setObjectName(QStringLiteral("displayImageLabel"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(displayImageLabel->sizePolicy().hasHeightForWidth());
        displayImageLabel->setSizePolicy(sizePolicy);
        displayImageLabel->setToolTipDuration(-1);
        displayImageLabel->setLayoutDirection(Qt::LeftToRight);
        displayImageLabel->setStyleSheet(QStringLiteral(""));
        displayImageLabel->setLineWidth(0);
        displayImageLabel->setAlignment(Qt::AlignCenter);
        displayImageLabel->setIndent(0);

        gridLayout->addWidget(displayImageLabel, 0, 0, 1, 1);


        verticalLayout_7->addWidget(frame);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 781, 24));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setMinimumSize(QSize(200, 305));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        formLayout = new QFormLayout(dockWidgetContents);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        toolBox = new QToolBox(dockWidgetContents);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        page->setGeometry(QRect(0, 0, 182, 78));
        verticalLayout_2 = new QVBoxLayout(page);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        openImageButton = new QPushButton(page);
        openImageButton->setObjectName(QStringLiteral("openImageButton"));

        verticalLayout_2->addWidget(openImageButton);

        saveImageButton = new QPushButton(page);
        saveImageButton->setObjectName(QStringLiteral("saveImageButton"));

        verticalLayout_2->addWidget(saveImageButton);

        toolBox->addItem(page, QStringLiteral("Open/Save"));
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        page_3->setGeometry(QRect(0, 0, 168, 78));
        verticalLayout_3 = new QVBoxLayout(page_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        binarizeKmeansButton = new QPushButton(page_3);
        binarizeKmeansButton->setObjectName(QStringLiteral("binarizeKmeansButton"));

        verticalLayout_3->addWidget(binarizeKmeansButton);

        binarizeFcmButton = new QPushButton(page_3);
        binarizeFcmButton->setObjectName(QStringLiteral("binarizeFcmButton"));

        verticalLayout_3->addWidget(binarizeFcmButton);

        toolBox->addItem(page_3, QStringLiteral("Binarization"));
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        page_2->setGeometry(QRect(0, 0, 182, 79));
        gridLayout_2 = new QGridLayout(page_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        posterizeButton = new QPushButton(page_2);
        posterizeButton->setObjectName(QStringLiteral("posterizeButton"));

        gridLayout_2->addWidget(posterizeButton, 1, 0, 1, 1);

        posterSpinBox = new QSpinBox(page_2);
        posterSpinBox->setObjectName(QStringLiteral("posterSpinBox"));
        posterSpinBox->setMinimum(2);

        gridLayout_2->addWidget(posterSpinBox, 0, 0, 1, 1);

        toolBox->addItem(page_2, QStringLiteral("Posterize"));
        EdgeDetection = new QWidget();
        EdgeDetection->setObjectName(QStringLiteral("EdgeDetection"));
        EdgeDetection->setGeometry(QRect(0, 0, 182, 120));
        verticalLayout_4 = new QVBoxLayout(EdgeDetection);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        cannySlider = new QSlider(EdgeDetection);
        cannySlider->setObjectName(QStringLiteral("cannySlider"));
        cannySlider->setMaximum(50);
        cannySlider->setOrientation(Qt::Horizontal);

        verticalLayout_4->addWidget(cannySlider);

        cannyButton = new QPushButton(EdgeDetection);
        cannyButton->setObjectName(QStringLiteral("cannyButton"));

        verticalLayout_4->addWidget(cannyButton);

        fuzzySlider = new QSlider(EdgeDetection);
        fuzzySlider->setObjectName(QStringLiteral("fuzzySlider"));
        fuzzySlider->setOrientation(Qt::Horizontal);

        verticalLayout_4->addWidget(fuzzySlider);

        fuzzyButton = new QPushButton(EdgeDetection);
        fuzzyButton->setObjectName(QStringLiteral("fuzzyButton"));

        verticalLayout_4->addWidget(fuzzyButton);

        toolBox->addItem(EdgeDetection, QStringLiteral("Edge detection"));
        page_4 = new QWidget();
        page_4->setObjectName(QStringLiteral("page_4"));
        page_4->setGeometry(QRect(0, 0, 182, 79));
        verticalLayout_5 = new QVBoxLayout(page_4);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        pcaSpinBox = new QSpinBox(page_4);
        pcaSpinBox->setObjectName(QStringLiteral("pcaSpinBox"));
        pcaSpinBox->setMinimum(2);

        verticalLayout_5->addWidget(pcaSpinBox);

        pcaButton = new QPushButton(page_4);
        pcaButton->setObjectName(QStringLiteral("pcaButton"));

        verticalLayout_5->addWidget(pcaButton);

        toolBox->addItem(page_4, QStringLiteral("Compression"));
        page_5 = new QWidget();
        page_5->setObjectName(QStringLiteral("page_5"));
        page_5->setGeometry(QRect(0, 0, 182, 68));
        verticalLayout_6 = new QVBoxLayout(page_5);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        distortionButton = new QPushButton(page_5);
        distortionButton->setObjectName(QStringLiteral("distortionButton"));

        verticalLayout_6->addWidget(distortionButton);

        toolBox->addItem(page_5, QStringLiteral("Distortion"));

        formLayout->setWidget(0, QFormLayout::SpanningRole, toolBox);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);

        retranslateUi(MainWindow);

        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        displayImageLabel->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        openImageButton->setText(QApplication::translate("MainWindow", "Open", nullptr));
        saveImageButton->setText(QApplication::translate("MainWindow", "Save", nullptr));
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("MainWindow", "Open/Save", nullptr));
        binarizeKmeansButton->setText(QApplication::translate("MainWindow", "kmeans", nullptr));
        binarizeFcmButton->setText(QApplication::translate("MainWindow", "fuzzy c-means", nullptr));
        toolBox->setItemText(toolBox->indexOf(page_3), QApplication::translate("MainWindow", "Binarization", nullptr));
        posterizeButton->setText(QApplication::translate("MainWindow", "Posterize", nullptr));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("MainWindow", "Posterize", nullptr));
        cannyButton->setText(QApplication::translate("MainWindow", "Canny", nullptr));
        fuzzyButton->setText(QApplication::translate("MainWindow", "Fuzzy", nullptr));
        toolBox->setItemText(toolBox->indexOf(EdgeDetection), QApplication::translate("MainWindow", "Edge detection", nullptr));
        pcaButton->setText(QApplication::translate("MainWindow", "PCA", nullptr));
        toolBox->setItemText(toolBox->indexOf(page_4), QApplication::translate("MainWindow", "Compression", nullptr));
        distortionButton->setText(QApplication::translate("MainWindow", "start", nullptr));
        toolBox->setItemText(toolBox->indexOf(page_5), QApplication::translate("MainWindow", "Distortion", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
