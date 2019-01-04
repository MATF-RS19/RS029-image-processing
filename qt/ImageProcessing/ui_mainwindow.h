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
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
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
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QFormLayout *formLayout;
    QToolBox *toolBox;
    QWidget *open_save;
    QVBoxLayout *verticalLayout_2;
    QPushButton *openImageButton;
    QPushButton *saveImageButton;
    QWidget *binarization;
    QVBoxLayout *verticalLayout_3;
    QPushButton *binarizeKmeansButton;
    QPushButton *binarizeFcmButton;
    QWidget *poster;
    QGridLayout *gridLayout_2;
    QPushButton *posterizeButton;
    QSpinBox *posterSpinBox;
    QWidget *edge_detection;
    QVBoxLayout *verticalLayout_4;
    QSlider *cannySlider;
    QPushButton *cannyButton;
    QSlider *fuzzySlider;
    QPushButton *fuzzyButton;
    QWidget *compression;
    QVBoxLayout *verticalLayout_5;
    QSpinBox *pcaSpinBox;
    QPushButton *pcaButton;
    QWidget *distortion;
    QVBoxLayout *verticalLayout_6;
    QPushButton *distortionButton;
    QMenuBar *menuBar;
    QMenu *menuIP;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(781, 435);
        MainWindow->setMinimumSize(QSize(640, 405));
        QFont font;
        font.setPointSize(50);
        MainWindow->setFont(font);
        MainWindow->setStyleSheet(QLatin1String("#MainWindow {\n"
"	background:#333333;\n"
"}\n"
"\n"
"QWidget { \n"
"	color : #1793D1;\n"
"	background:#333333;\n"
"	font: \"Terminus (TTF)\";\n"
" }\n"
"\n"
"QPushButton {\n"
"	border : 1px solid #1793D1;\n"
"	border-radius: 10px;\n"
"	padding: 2px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background:#1793D1;\n"
"	color: white;\n"
"}\n"
"\n"
"QMenuBar {\n"
"	font: 75 14pt \"Terminus (TTF)\";\n"
"}\n"
""));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setStyleSheet(QLatin1String("#centralWidget{\n"
"background:#333333;\n"
"}"));
        verticalLayout_7 = new QVBoxLayout(centralWidget);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setToolTipDuration(-1);
        frame->setStyleSheet(QLatin1String("#frame {\n"
"background:#333333;\n"
"}"));
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
        displayImageLabel->setMinimumSize(QSize(10, 10));
        QFont font1;
        font1.setPointSize(50);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        displayImageLabel->setFont(font1);
        displayImageLabel->setToolTipDuration(-1);
        displayImageLabel->setLayoutDirection(Qt::LeftToRight);
        displayImageLabel->setStyleSheet(QLatin1String("#displayImageLabel{\n"
"	background:#333333;	\n"
"	color : #1793D1;\n"
"}"));
        displayImageLabel->setLineWidth(0);
        displayImageLabel->setAlignment(Qt::AlignCenter);
        displayImageLabel->setIndent(0);

        gridLayout->addWidget(displayImageLabel, 0, 0, 1, 1);


        verticalLayout_7->addWidget(frame);

        MainWindow->setCentralWidget(centralWidget);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setMinimumSize(QSize(200, 319));
        dockWidget->setAutoFillBackground(false);
        dockWidget->setStyleSheet(QStringLiteral(""));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        dockWidgetContents->setStyleSheet(QLatin1String("#dockWidgetContents {\n"
"background:#333333;\n"
"}"));
        formLayout = new QFormLayout(dockWidgetContents);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        toolBox = new QToolBox(dockWidgetContents);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        toolBox->setStyleSheet(QLatin1String("\n"
"#toolBox {\n"
"	background: #333333;\n"
"}\n"
"\n"
"QWidget { \n"
"	color : #1793D1;\n"
"	background:#333333;\n"
"	font: 75 14pt \"Terminus (TTF)\";\n"
"    font-weight:bold;\n"
" }\n"
"\n"
"\n"
"QSlider::handle:horizontal {\n"
"	background: #1793D1;\n"
"	border-radius: 6px;\n"
"}\n"
"\n"
"\n"
"QPushButton {\n"
"	border : 1px solid #1793D1;\n"
"	border-radius: 10px;\n"
"	padding: 2px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background:#1793D1;\n"
"	color: white;\n"
"}\n"
""));
        open_save = new QWidget();
        open_save->setObjectName(QStringLiteral("open_save"));
        open_save->setGeometry(QRect(0, 0, 182, 78));
        open_save->setStyleSheet(QStringLiteral(""));
        verticalLayout_2 = new QVBoxLayout(open_save);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        openImageButton = new QPushButton(open_save);
        openImageButton->setObjectName(QStringLiteral("openImageButton"));
        openImageButton->setAutoFillBackground(false);
        openImageButton->setStyleSheet(QStringLiteral(""));

        verticalLayout_2->addWidget(openImageButton);

        saveImageButton = new QPushButton(open_save);
        saveImageButton->setObjectName(QStringLiteral("saveImageButton"));
        saveImageButton->setStyleSheet(QStringLiteral(""));

        verticalLayout_2->addWidget(saveImageButton);

        toolBox->addItem(open_save, QStringLiteral("Open/Save"));
        binarization = new QWidget();
        binarization->setObjectName(QStringLiteral("binarization"));
        binarization->setGeometry(QRect(0, 0, 182, 78));
        binarization->setStyleSheet(QStringLiteral(""));
        verticalLayout_3 = new QVBoxLayout(binarization);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        binarizeKmeansButton = new QPushButton(binarization);
        binarizeKmeansButton->setObjectName(QStringLiteral("binarizeKmeansButton"));

        verticalLayout_3->addWidget(binarizeKmeansButton);

        binarizeFcmButton = new QPushButton(binarization);
        binarizeFcmButton->setObjectName(QStringLiteral("binarizeFcmButton"));

        verticalLayout_3->addWidget(binarizeFcmButton);

        toolBox->addItem(binarization, QStringLiteral("Binarization"));
        poster = new QWidget();
        poster->setObjectName(QStringLiteral("poster"));
        poster->setGeometry(QRect(0, 0, 182, 80));
        poster->setStyleSheet(QStringLiteral(""));
        gridLayout_2 = new QGridLayout(poster);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        posterizeButton = new QPushButton(poster);
        posterizeButton->setObjectName(QStringLiteral("posterizeButton"));

        gridLayout_2->addWidget(posterizeButton, 1, 0, 1, 1);

        posterSpinBox = new QSpinBox(poster);
        posterSpinBox->setObjectName(QStringLiteral("posterSpinBox"));
        posterSpinBox->setMinimum(2);

        gridLayout_2->addWidget(posterSpinBox, 0, 0, 1, 1);

        toolBox->addItem(poster, QStringLiteral("Posterize"));
        edge_detection = new QWidget();
        edge_detection->setObjectName(QStringLiteral("edge_detection"));
        edge_detection->setGeometry(QRect(0, 0, 182, 120));
        edge_detection->setStyleSheet(QStringLiteral(""));
        verticalLayout_4 = new QVBoxLayout(edge_detection);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        cannySlider = new QSlider(edge_detection);
        cannySlider->setObjectName(QStringLiteral("cannySlider"));
        cannySlider->setStyleSheet(QStringLiteral(""));
        cannySlider->setMaximum(50);
        cannySlider->setOrientation(Qt::Horizontal);

        verticalLayout_4->addWidget(cannySlider);

        cannyButton = new QPushButton(edge_detection);
        cannyButton->setObjectName(QStringLiteral("cannyButton"));

        verticalLayout_4->addWidget(cannyButton);

        fuzzySlider = new QSlider(edge_detection);
        fuzzySlider->setObjectName(QStringLiteral("fuzzySlider"));
        fuzzySlider->setOrientation(Qt::Horizontal);

        verticalLayout_4->addWidget(fuzzySlider);

        fuzzyButton = new QPushButton(edge_detection);
        fuzzyButton->setObjectName(QStringLiteral("fuzzyButton"));

        verticalLayout_4->addWidget(fuzzyButton);

        toolBox->addItem(edge_detection, QStringLiteral("Edge detection"));
        compression = new QWidget();
        compression->setObjectName(QStringLiteral("compression"));
        compression->setGeometry(QRect(0, 0, 182, 80));
        compression->setStyleSheet(QStringLiteral(""));
        verticalLayout_5 = new QVBoxLayout(compression);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        pcaSpinBox = new QSpinBox(compression);
        pcaSpinBox->setObjectName(QStringLiteral("pcaSpinBox"));
        pcaSpinBox->setMinimum(2);

        verticalLayout_5->addWidget(pcaSpinBox);

        pcaButton = new QPushButton(compression);
        pcaButton->setObjectName(QStringLiteral("pcaButton"));

        verticalLayout_5->addWidget(pcaButton);

        toolBox->addItem(compression, QStringLiteral("Compression"));
        distortion = new QWidget();
        distortion->setObjectName(QStringLiteral("distortion"));
        distortion->setGeometry(QRect(0, 0, 182, 68));
        distortion->setStyleSheet(QStringLiteral(""));
        verticalLayout_6 = new QVBoxLayout(distortion);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        distortionButton = new QPushButton(distortion);
        distortionButton->setObjectName(QStringLiteral("distortionButton"));

        verticalLayout_6->addWidget(distortionButton);

        toolBox->addItem(distortion, QStringLiteral("Distortion"));

        formLayout->setWidget(0, QFormLayout::SpanningRole, toolBox);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 781, 26));
        menuIP = new QMenu(menuBar);
        menuIP->setObjectName(QStringLiteral("menuIP"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuIP->menuAction());

        retranslateUi(MainWindow);

        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        displayImageLabel->setText(QApplication::translate("MainWindow", "\357\200\276", nullptr));
        openImageButton->setText(QApplication::translate("MainWindow", "Open \357\201\274", nullptr));
        saveImageButton->setText(QApplication::translate("MainWindow", "Save \357\203\207", nullptr));
        toolBox->setItemText(toolBox->indexOf(open_save), QApplication::translate("MainWindow", "Open/Save", nullptr));
        binarizeKmeansButton->setText(QApplication::translate("MainWindow", "kmeans", nullptr));
        binarizeFcmButton->setText(QApplication::translate("MainWindow", "fuzzy c-means", nullptr));
        toolBox->setItemText(toolBox->indexOf(binarization), QApplication::translate("MainWindow", "Binarization", nullptr));
        posterizeButton->setText(QApplication::translate("MainWindow", "Posterize", nullptr));
        toolBox->setItemText(toolBox->indexOf(poster), QApplication::translate("MainWindow", "Posterize", nullptr));
        cannyButton->setText(QApplication::translate("MainWindow", "Canny", nullptr));
        fuzzyButton->setText(QApplication::translate("MainWindow", "Fuzzy", nullptr));
        toolBox->setItemText(toolBox->indexOf(edge_detection), QApplication::translate("MainWindow", "Edge detection", nullptr));
        pcaButton->setText(QApplication::translate("MainWindow", "PCA", nullptr));
        toolBox->setItemText(toolBox->indexOf(compression), QApplication::translate("MainWindow", "Compression", nullptr));
        distortionButton->setText(QApplication::translate("MainWindow", "start", nullptr));
        toolBox->setItemText(toolBox->indexOf(distortion), QApplication::translate("MainWindow", "Distortion", nullptr));
        menuIP->setTitle(QApplication::translate("MainWindow", "\357\204\240 ImageProcessing", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
