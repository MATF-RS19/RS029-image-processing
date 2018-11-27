#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "binarization.hpp"
#include "canny.hpp"
#include "image.hpp"
#include "boost/variant.hpp"
#include <QFileDialog>

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
    QString image_path = QFileDialog::getOpenFileName(this,  ("Open File"), QDir::currentPath(), ("Images (*.png *.xpm *.jpg)"));

    im = img::Image<img::Type::RGB>(image_path.toStdString());
    display_image(im);

    // clear transformed image
    im_transformed = img::Image<>{};
}


void MainWindow::on_binarizeButton_clicked()
{
    if (im) {
        auto im_binarized = binarization(im.grayscale());
        im_binarized.set_name(im.purename()+"_binarized.png");
        display_image(im_binarized);

        im_transformed = std::move(im_binarized);
    }
}

void MainWindow::on_saveImageButton_clicked()
{
    boost::apply_visitor([this](auto&& i) {
        if (!i) return;
        QString save_path = QFileDialog::getSaveFileName(this, ("Save File"), i.name().c_str(), ("Images (*.png *.xpm *.jpg)"));
        i.save(save_path.toStdString());
    }, im_transformed);
}

void MainWindow::on_cannyButton_clicked()
{
    if (im) {
        auto im_canny = canny(im.grayscale());
        im_canny.set_name(im.purename()+"_canny.png");
        display_image(im_canny);

        im_transformed = std::move(im_canny);

        ui->cannySlider->setValue(10);
    }
}

void MainWindow::on_cannySlider_sliderReleased()
{
    if (im) {
        int lower = ui->cannySlider->value();
        auto im_canny = canny(im.grayscale(), lower, 3*lower);
        im_canny.set_name(im.purename()+"_canny.png");
        display_image(im_canny);

        im_transformed = std::move(im_canny);
    }
}


void MainWindow::on_toolBox_currentChanged(int index)
{
    ui->cannySlider->setValue(0);
}
