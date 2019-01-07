#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "binarization.hpp"
#include "fuzzy_edge_detection.hpp"
#include "distortion.hpp"
#include "pca.hpp"
#include "canny.hpp"
#include "image.hpp"
#include "kmeans.hpp"
#include <variant>
#include <QFileDialog>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->displayImageLabel, SIGNAL(sendPoints(QVector<QPoint>&)), this, SLOT(remove_distortion(QVector<QPoint>&)));
	ui->frame->installEventFilter(this);

}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::Resize && obj == ui->frame) {
		// display transformed image and return true if it is defined, otherwise return false
		bool ind = std::visit([this](auto&& i) {
					if (!i) return false;
					display_image(i);
					return true;
				}, im_transformed);

		// if there is no transformed image display the original one
		if (!ind)
			display_image(im);
	}
	return QWidget::eventFilter(obj, event);
}


void MainWindow::display_image(const auto& image)
{
	if (!image) return;

	// fit dimensions
	int height_max = std::min(image.rows(), ui->frame->height()-22);
	int width_max = std::min(image.cols(), ui->frame->width()-22);

	int width = width_max;
	int height = width/image.get_aspect_ratio();
	if (height > height_max) {
		height = height_max;
		width = height*image.get_aspect_ratio();
	}

	ui->displayImageLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	ui->displayImageLabel->resize(width,height);

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

void MainWindow::on_saveImageButton_clicked()
{
	std::visit([this](auto&& i) {
				if (!i) return;
				QString save_path = QFileDialog::getSaveFileName(this, ("Save File"), i.name().c_str(), ("Images (*.png *.xpm *.jpg)"));
				i.save(save_path.toStdString());
			}, im_transformed);
}



void MainWindow::on_binarizeFcmButton_clicked()
{
	if (im) {
		auto im_binarized = binarization_fcm(im.grayscale());
		im_binarized.set_name(im.purename()+"_binarized_fcm.png");
		display_image(im_binarized);

		im_transformed = std::move(im_binarized);
	}
}

void MainWindow::on_binarizeKmeansButton_clicked()
{
	if (im) {
		auto im_binarized = binarization_kmeans(im.grayscale());
		im_binarized.set_name(im.purename()+"_binarized_kmeans.png");
		display_image(im_binarized);

		im_transformed = std::move(im_binarized);
	}
}


void MainWindow::on_cannyButton_clicked()
{
	if (im) {
		auto im_canny = canny(im.grayscale());
		im_canny.set_name(im.purename()+"_canny.png");
		display_image(im_canny);

		im_transformed = std::move(im_canny);

		ui->cannySlider->setValue(20);
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

void MainWindow::on_fuzzyButton_clicked()
{
	if (im) {
		auto im_fuzzy = fuzzy_edge_detection(im.grayscale());
		im_fuzzy.set_name(im.purename()+"_fuzzy_edge_detection.png");
		display_image(im_fuzzy);

		im_transformed = std::move(im_fuzzy);

		ui->fuzzySlider->setValue(35);
	}
}

void MainWindow::on_fuzzySlider_sliderReleased()
{
	if (im) {
		float threshold = ui->fuzzySlider->value()/100.0f;
		auto im_fuzzy = fuzzy_edge_detection(im.grayscale(), threshold);
		im_fuzzy.set_name(im.purename()+"_fuzzy_edge_detection.png");
		display_image(im_fuzzy);

		im_transformed = std::move(im_fuzzy);
	}
}

void MainWindow::on_pcaButton_clicked()
{
	if (im) {
		int k = ui->pcaSpinBox->value();
		auto im_pca = pca(im, k);
		im_pca.set_name(im.purename()+"_pca.png");
		display_image(im_pca);

		im_transformed = std::move(im_pca);
	}
}


void MainWindow::remove_distortion(QVector<QPoint>& points)
{
	float coeff = im.cols()/(float)ui->displayImageLabel->width();

	std::vector<std::pair<float, float>> points_in;
	std::vector<std::pair<float, float>> points_out;

	for (int i = 0; i < 4; i++)
		points_in.push_back({points[i].y()*coeff, points[i].x()*coeff});
	for (int i = 4; i < 8; i++)
		points_out.push_back({points[i].y()*coeff, points[i].x()*coeff});

	auto im_distortion = distortion(im, points_in, points_out);
	im_distortion.set_name(im.purename()+"_distortion.png");
	display_image(im_distortion);

	im_transformed = std::move(im_distortion);
	ui->toolBox->setDisabled(false);
}

void MainWindow::on_distortionButton_clicked()
{
	if (im) {
		ui->displayImageLabel->mouse_enabled = true;
		ui->toolBox->setDisabled(true);
		// TODO disable resizing here
	}
}

void MainWindow::on_posterizeButton_clicked()
{
	if (im) {
		int k = ui->posterSpinBox->value();
		auto im_poster = color_quantization(im, k);
		im_poster.set_name(im.purename()+"_poster.png");
		display_image(im_poster);

		im_transformed = std::move(im_poster);
	}
}

void MainWindow::on_toolBox_currentChanged(int index)
{
	ui->cannySlider->setValue(0);
	ui->fuzzySlider->setValue(0);
	ui->pcaSpinBox->setValue(2);
	ui->posterSpinBox->setValue(2);
	if (im)
		ui->pcaSpinBox->setMaximum(std::min(im.cols(), im.rows()));
}
