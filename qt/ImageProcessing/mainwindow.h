#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "image.hpp"
#include <variant>
#include <QMouseEvent>
#include <QPushButton>
#include <QVector>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();
private:
	void display_image(const auto& im);

private slots:
	bool eventFilter(QObject *obj, QEvent *event);

	void on_openImageButton_clicked();

	void on_binarizeFcmButton_clicked();

	void on_saveImageButton_clicked();

	void on_cannyButton_clicked();

	void on_cannySlider_sliderReleased();

	void on_toolBox_currentChanged(int index);

	void on_fuzzyButton_clicked();

	void on_fuzzySlider_sliderReleased();

	void on_pcaButton_clicked();

	void on_distortionButton_clicked();

	void on_posterizeButton_clicked();

	void on_binarizeKmeansButton_clicked();

	public slots:
		void remove_distortion(QVector<QPoint>& points);

private:
	Ui::MainWindow *ui;
	// input image
	img::Image<img::Type::RGB> im;
	// transformed image
	std::variant<img::Image<img::Type::RGB>, img::Image<img::Type::GRAYSCALE>> im_transformed;
};

#endif // MAINWINDOW_H
