#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"headers.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString selectDir = QFileDialog::getOpenFileName();
    string selectDir_str=selectDir.toStdString();
    QPixmap pixmap(selectDir);
    ui->orginimagelabel->setPixmap(pixmap.scaled(ui->orginimagelabel->size()));
    this->filename=selectDir_str;
}


void MainWindow::on_pushButton_2_clicked()
{
    Image image = readBMP(this->filename);
    GrayImage grayimage = changeGrayBMP(image);
    vector<vector<unsigned char>> newimageData(image.height, vector<unsigned char>(image.width * 3, 0));
    newimageData = Changeimage(grayimage.imagedata, grayimage.height, grayimage.width);
    QImage image1(grayimage.width, grayimage.height, QImage::Format_Indexed8);
        image1.setColorCount(256);
        for (int i = 0; i < 256; ++i) {
            image1.setColor(i, qRgb(i, i, i)); // 设置灰度颜色表
        }

        // 将二维数组的数据填充到 QImage 中
        for (int y = 0; y < grayimage.height; ++y) {
            for (int x = 0; x < grayimage.width; ++x) {
                image1.setPixel(x, y, grayimage.imagedata[y][x]); // 设置像素值
            }
        }
    QPixmap pixmap = QPixmap::fromImage(image1);
    this->graypix=pixmap;
    ui->grayimagelabel->setPixmap(pixmap.scaled(ui->grayimagelabel->size()));
}


void MainWindow::on_pushButton_3_clicked()
{
    Image image = readBMP(this->filename);
    GrayImage grayimage = changeGrayBMP(image);
    vector<vector<unsigned char>> newimageData(image.height, vector<unsigned char>(image.width * 3, 0));
    BlackImage Toimage = changeToBMP(grayimage, ui->lineEdit->text().toInt());
    newimageData = Changeimage(Toimage.imagedata, Toimage.height, Toimage.width);
    QImage image1(Toimage.width, Toimage.height, QImage::Format_Indexed8);
        image1.setColorCount(256);
        for (int i = 0; i < 256; ++i) {
            image1.setColor(i, qRgb(i, i, i)); // 设置灰度颜色表
        }

        // 将二维数组的数据填充到 QImage 中
        for (int y = 0; y < Toimage.height; ++y) {
            for (int x = 0; x < Toimage.width; ++x) {
                image1.setPixel(x, y, Toimage.imagedata[y][x]); // 设置像素值
            }
        }
    QPixmap pixmap = QPixmap::fromImage(image1);
    this->birpix=pixmap;
    ui->binaryimagelabel->setPixmap(pixmap.scaled(ui->binaryimagelabel->size()));
}


void MainWindow::on_pushButton_4_clicked()
{
    Image image = readBMP(this->filename);
    GrayImage grayimage = changeGrayBMP(image);
    int threshold = adaptiveThreshold(grayimage);
    BlackImage Toimage = changeToBMP(grayimage, threshold);
    vector<vector<unsigned char>> newimageData(image.height, vector<unsigned char>(image.width * 3, 0));
    newimageData = Changeimage(Toimage.imagedata, Toimage.height, Toimage.width);
    QImage image1(Toimage.width, Toimage.height, QImage::Format_Indexed8);
        image1.setColorCount(256);
        for (int i = 0; i < 256; ++i) {
            image1.setColor(i, qRgb(i, i, i)); // 设置灰度颜色表
        }

        // 将二维数组的数据填充到 QImage 中
        for (int y = 0; y < Toimage.height; ++y) {
            for (int x = 0; x < Toimage.width; ++x) {
                image1.setPixel(x, y, Toimage.imagedata[y][x]); // 设置像素值
            }
        }
    QPixmap pixmap = QPixmap::fromImage(image1);
    this->birpix=pixmap;
    ui->binaryimagelabel->setPixmap(pixmap.scaled(ui->binaryimagelabel->size()));
}


void MainWindow::on_pushButton_5_clicked()
{
    Image image = readBMP(this->filename);
    GrayImage grayimage = changeGrayBMP(image);
    int threshold = otsuThreshold(grayimage);
    BlackImage Toimage = changeToBMP(grayimage, threshold);
    vector<vector<unsigned char>> newimageData(image.height, vector<unsigned char>(image.width * 3, 0));
    newimageData = Changeimage(Toimage.imagedata, Toimage.height, Toimage.width);
    QImage image1(Toimage.width, Toimage.height, QImage::Format_Indexed8);
        image1.setColorCount(256);
        for (int i = 0; i < 256; ++i) {
            image1.setColor(i, qRgb(i, i, i)); // 设置灰度颜色表
        }

        // 将二维数组的数据填充到 QImage 中
        for (int y = 0; y < Toimage.height; ++y) {
            for (int x = 0; x < Toimage.width; ++x) {
                image1.setPixel(x, y, Toimage.imagedata[y][x]); // 设置像素值
            }
        }
    QPixmap pixmap = QPixmap::fromImage(image1);
    this->birpix=pixmap;
    ui->binaryimagelabel->setPixmap(pixmap.scaled(ui->binaryimagelabel->size()));
}


void MainWindow::on_pushButton_6_clicked()
{
    Image image = readBMP(this->filename);
    GrayImage grayimage = changeGrayBMP(image);
    int threshold = Triangle(grayimage);
    //qDebug()<<threshold;
    BlackImage Toimage = changeToBMP(grayimage, threshold);
    vector<vector<unsigned char>> newimageData(image.height, vector<unsigned char>(image.width * 3, 0));
    newimageData = Changeimage(Toimage.imagedata, Toimage.height, Toimage.width);
    QImage image1(Toimage.width, Toimage.height, QImage::Format_Indexed8);
        image1.setColorCount(256);
        for (int i = 0; i < 256; ++i) {
            image1.setColor(i, qRgb(i, i, i)); // 设置灰度颜色表
        }

        // 将二维数组的数据填充到 QImage 中
        for (int y = 0; y < Toimage.height; ++y) {
            for (int x = 0; x < Toimage.width; ++x) {
                image1.setPixel(x, y, Toimage.imagedata[y][x]); // 设置像素值
            }
        }
    QPixmap pixmap = QPixmap::fromImage(image1);
    this->birpix=pixmap;
    ui->binaryimagelabel->setPixmap(pixmap.scaled(ui->binaryimagelabel->size()));
}


void MainWindow::on_pushButton_8_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
            tr("Save Image"),
            "",
            tr("*.bmp")); //选择路径
    this->graypix.toImage().save(filename);
}


void MainWindow::on_pushButton_7_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
            tr("Save Image"),
            "",
            tr("*.bmp")); //选择路径
    this->birpix.toImage().save(filename);
}

