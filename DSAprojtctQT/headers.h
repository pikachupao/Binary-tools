#ifndef HEADERS_H
#define HEADERS_H

#endif // HEADERS_H


#define WIN32_LEAN_AND_MEAN //去除已引用头文件
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <wingdi.h>
#include <QDebug>
#include <QFileDialog>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <math.h>
using namespace std;

#pragma pack(push, 1)

typedef struct {
    unsigned short bfType;      // 文件类型标识，必须为0x4D42 (BM)，表示这是一个位图文件
    unsigned int   bfSize;      // 文件大小，整个文件的字节大小
    unsigned short bfReserved1; // 保留字段，必须为0
    unsigned short bfReserved2; // 保留字段，必须为0
    unsigned int   bfOffBits;   // 数据起始位置，从文件头开始到像素数据开始的偏移量
} MYBITMAPFILEHEADER;

typedef struct {
    unsigned int   biSize;        // 结构体大小，本结构体的字节大小
    int            biWidth;       // 图像宽度，单位为像素
    int            biHeight;      // 图像高度，单位为像素
    unsigned short biPlanes;      // 平面数，总是1
    unsigned short biBitCount;    // 每个像素占用的位数，如1、4、8、24或32位
    unsigned int   biCompression; // 压缩方式，0表示不压缩
    unsigned int   biSizeImage;   // 图像大小，图像数据的字节大小
    int            biXPelsPerMeter;// 水平分辨率，单位为像素/米
    int            biYPelsPerMeter;// 垂直分辨率，单位为像素/米
    unsigned int   biClrUsed;     // 实际使用的颜色数，0表示使用所有颜色
    unsigned int   biClrImportant;// 显示时重要的颜色数，0表示所有颜色都重要
} MYBITMAPINFOHEADER;

typedef struct
{
    unsigned char*** imagedata;
    int height;
    int width;
}Image;

typedef struct
{
    unsigned char** imagedata;
    int height;
    int width;
}GrayImage;

typedef struct
{
    unsigned char** imagedata;
    int height;
    int width;
}BlackImage;

#pragma pack(pop)

void saveBMP(const string& filename, const vector<vector<unsigned char>>& image, int width, int height);

Image readBMP(const string& filename);

GrayImage changeGrayBMP(Image imagest);

BlackImage changeToBMP(GrayImage grayimagest, int threshold);

vector<vector<unsigned char>> Changeimage(unsigned char** imagedata, int height, int width);

int adaptiveThreshold(GrayImage grayimagest);

int otsuThreshold(GrayImage grayimagest);

//int triangleThreshold(GrayImage grayimagest);

int Triangle(GrayImage grayimagest);
