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

void MainWindow::display_image(const auto& image)
{
    if (!image) return;

    // fit dimensions
    int height_max = std::min(image.rows(), ui->displayImageLabel->height());
    int width_max = std::min(image.cols(), ui->displayImageLabel->width());

    int width = width_max;
    int height = width/image.get_aspect_ratio();
    if (height > height_max) {
        height = height_max;
        width = height*image.get_aspect_ratio();
    }

    // resize and keep aspect ratio
    auto im_display = image.resize(width, height);

    if (im_display.type() == img::Type::RGB) {
        // Qt reads in RGB whereas CV in BGR
        im_display.bgr2rgb();
    }

    // create Qt image and display it
    QImage qim_display(im_display.data(),
                       im_display.cols(),
                       im_display.rows(),
                       im_display.step(),
                       (im_display.type() == img::Type::RGB) ? QImage::Format_RGB888 : QImage::Format_Indexed8);

    ui->displayImageLabel->setPixmap(QPixmap::fromImage(std::move(qim_display)));
}

void MainWindow::on_openImageButton_clicked()
{
    if (!im) {
        QString image_path = QFileDialog::getOpenFileName(this, ("Open File"),  QDir::currentPath(), ("Images (*.png *.xpm *.jpg)"));

        im =  img::Image<img::Type::RGB>(image_path.toStdString());
    }

    display_image(im);
}

void MainWindow::on_binarizeButton_clicked()
{
    if (im) {
        auto im_binarized = binarization(im.grayscale());
        display_image(im_binarized);
    }
}
