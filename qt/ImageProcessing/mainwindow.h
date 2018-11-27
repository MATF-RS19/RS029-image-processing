#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "image.hpp"
#include "boost/variant.hpp"

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
    void on_openImageButton_clicked();

    void on_binarizeButton_clicked();

    void on_saveImageButton_clicked();

private:
    Ui::MainWindow *ui;

    // input image
    img::Image<img::Type::RGB> im;
    // transformed image
    boost::variant<img::Image<img::Type::RGB>, img::Image<img::Type::GRAYSCALE>> im_transformed;

};

#endif // MAINWINDOW_H
