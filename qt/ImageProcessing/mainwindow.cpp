#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "binarization.h"
#include "image.hpp"
#include <QFileDialog>

static img::Image<img::Type::RGB> im;
static img::Image<img::Type::RGB> im_rgb_transformed;
static img::Image<img::Type::GRAYSCALE> im_gray_transformed;
enum class At { GRAYSCALE, RGB, NONE };
static At active_type = At::NONE;


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
    im =  img::Image<img::Type::RGB>(image_path.toStdString());

    display_image(im);

    active_type = At::NONE;
}

void MainWindow::on_binarizeButton_clicked()
{
    if (im) {
        im_gray_transformed = binarization(im.grayscale());
        im_gray_transformed.set_name(im.purename()+"_binarized.png");
        display_image(im_gray_transformed);
        active_type = At::GRAYSCALE;
    }
}

void MainWindow::on_saveImageButton_clicked()
{
    if (active_type == At::GRAYSCALE && im_gray_transformed) {
        QString save_path = QFileDialog::getSaveFileName(this, ("Save File"), im_gray_transformed.name().c_str(), ("Images (*.png *.xpm *.jpg)"));
        im_gray_transformed.save(save_path.toStdString());
    }
    else if (active_type == At::RGB && im_rgb_transformed) {
        QString save_path = QFileDialog::getSaveFileName(this, ("Save File"), im_rgb_transformed.name().c_str(), ("Images (*.png *.xpm *.jpg)"));
        im_rgb_transformed.save(save_path.toStdString());
    }
}
