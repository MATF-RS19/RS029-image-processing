#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "binarization.h"
#include "image.hpp"
#include <QFileDialog>

static img::Image<img::Type::RGB> im;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openImageButton_clicked()
{
    QString image_path = QFileDialog::getOpenFileName(this,
            tr("Open File"),
            "",
            tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
            );

    im =  img::Image<img::Type::RGB>(image_path.toStdString());

    img::Image<img::Type::RGB> im_display = im;

    int height = ui->displayImageLabel->height();
    int width = ui->displayImageLabel->width();
    cv::resize(im_display.m_data, im_display.m_data, cv::Size(width, height), 0, 0, cv::INTER_LINEAR);
    cv::cvtColor(im_display.m_data,im_display.m_data,CV_BGR2RGB); //Qt reads in RGB whereas CV in BGR

    QImage imdisplay((uchar*)im_display.m_data.data, im_display.cols(), im_display.rows(), im_display.m_data.step, QImage::Format_RGB888);

    ui->displayImageLabel->setPixmap(QPixmap::fromImage(imdisplay));
}

void MainWindow::on_binarizeButton_clicked()
{
    if (im) {
        auto im_binarized = binarization(im.grayscale());
        int height = ui->displayImageLabel->height();
        int width = ui->displayImageLabel->width();
        cv::resize(im_binarized.m_data, im_binarized.m_data, cv::Size(width, height), 0, 0, cv::INTER_LINEAR);
        //cv::cvtColor(im_binarized.m_data,im_binarized.m_data,CV_BGR2RGB); //Qt reads in RGB whereas CV in BGR

        QImage imdisplay((uchar*)im_binarized.m_data.data, im_binarized.cols(), im_binarized.rows(), im_binarized.m_data.step, QImage::Format_Indexed8);

        ui->displayImageLabel->setPixmap(QPixmap::fromImage(imdisplay));
    }
}
