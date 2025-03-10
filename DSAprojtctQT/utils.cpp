#include"headers.h"


void saveBMP(const string& filename, const vector<vector<unsigned char>>& image, int width, int height) {
    // 计算行字节数，需对齐到4字节边界
    int rowSize = ((width * 3 + 3) / 4) * 4;
    int padding = rowSize - width * 3;

    // 创建文件头
    MYBITMAPFILEHEADER fileHeader = { 0 };
    fileHeader.bfType = 0x4D42; // BM
    fileHeader.bfSize = sizeof(MYBITMAPFILEHEADER) + sizeof(MYBITMAPINFOHEADER) + rowSize * height;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfOffBits = sizeof(MYBITMAPFILEHEADER) + sizeof(MYBITMAPINFOHEADER);

    // 创建信息头
    MYBITMAPINFOHEADER infoHeader = { 0 };
    infoHeader.biSize = sizeof(MYBITMAPINFOHEADER);
    infoHeader.biWidth = width;
    infoHeader.biHeight = height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;
    infoHeader.biCompression = 0;
    infoHeader.biSizeImage = rowSize * height;
    infoHeader.biXPelsPerMeter = 2835; // 72 DPI
    infoHeader.biYPelsPerMeter = 2835; // 72 DPI
    infoHeader.biClrUsed = 0;
    infoHeader.biClrImportant = 0;

    // 创建图像数据缓冲区
    vector<unsigned char> imageData(rowSize * height);

    // 将图像数据填充到缓冲区中
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int pos = ((height - 1 - y) * rowSize + x * 3);
            imageData[pos + 2] = image[y][x * 3 + 0]; // R
            imageData[pos + 1] = image[y][x * 3 + 1]; // G
            imageData[pos + 0] = image[y][x * 3 + 2]; // B
        }
        // 添加填充字节
        if (padding > 0) {
            memset(&imageData[(height - 1 - y) * rowSize + width * 3], 0, padding);
        }
    }

    // 打开输出文件
    ofstream outFile(filename, ios::binary);
    if (!outFile.is_open()) {
        cerr << "Error: Could not create output file." << endl;
        return;
    }

    // 写入文件头
    outFile.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));

    // 写入信息头
    outFile.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    // 写入图像数据
    outFile.write(reinterpret_cast<char*>(imageData.data()), imageData.size());

    // 关闭文件
    outFile.close();
}

Image readBMP(const string& filename)
{
    Image imagest;
    imagest.width = 0;
    imagest.height = 0;
    ifstream file(filename.c_str(), ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Could not open file." << endl;
        return imagest;
    }

    // 读取文件头
    MYBITMAPFILEHEADER fileHeader;
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    if (fileHeader.bfType != 0x4D42) { // BM
        cerr << "Error: Not a valid BMP file." << endl;
        return imagest;
    }

    // 读取信息头
    MYBITMAPINFOHEADER infoHeader;
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    if (infoHeader.biBitCount != 24 || infoHeader.biCompression != 0) {
        cerr << "Error: Only 24-bit uncompressed BMP files are supported." << endl;
        return imagest;
    }

    int width = infoHeader.biWidth;
    int height = abs(infoHeader.biHeight); // 取绝对值以确保高度为正数
    int rowSize = ((width * 3 + 3) / 4) * 4; // 每行字节数，需对齐到4字节边界
    vector<unsigned char> imageData(rowSize * height);

    // 跳过颜色表（如果有）
    file.seekg(fileHeader.bfOffBits, ios::beg);

    // 读取图像数据
    file.read(reinterpret_cast<char*>(imageData.data()), imageData.size());

    // 显示一些基本信息
    //cout << "Image Width: " << width << " pixels" << endl;
    //cout << "Image Height: " << height << " pixels" << endl;
    //cout << "Image Size: " << imageData.size() << " bytes" << endl;

    // 动态分配二维数组
    unsigned char*** image = new unsigned char** [height];
    for (int i = 0; i < height; ++i) {
        image[i] = new unsigned char* [width];
        for (int j = 0; j < width; ++j) {
            image[i][j] = new unsigned char[3]; // 每个像素有三个通道（R, G, B）
        }
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int pos = ((height - 1 - y) * rowSize + x * 3);
            image[y][x][0] = imageData[pos + 2]; // R
            image[y][x][1] = imageData[pos + 1]; // G
            image[y][x][2] = imageData[pos + 0]; // B
        }
    }
    imagest.imagedata = image;
    imagest.width = width;
    imagest.height = height;
    return imagest;
}

GrayImage changeGrayBMP(Image imagest)
{
    GrayImage grayimage;
    grayimage.height = imagest.height;
    grayimage.width = imagest.width;

    // 动态分配内存
    grayimage.imagedata = new unsigned char* [grayimage.height];
    for (int i = 0; i < grayimage.height; ++i) {
        grayimage.imagedata[i] = new unsigned char[grayimage.width];
    }

    // 计算灰度值
    for (int y = 0; y < grayimage.height; ++y) {
        for (int x = 0; x < grayimage.width; ++x) {
            grayimage.imagedata[y][x] = static_cast<unsigned char>(
                imagest.imagedata[y][x][0] * 0.299 +
                imagest.imagedata[y][x][1] * 0.587 +
                imagest.imagedata[y][x][2] * 0.114
                );
        }
    }

    return grayimage;
}

BlackImage changeToBMP(GrayImage grayimagest, int threshold)
{
    BlackImage Toimage;
    Toimage.height = grayimagest.height;
    Toimage.width = grayimagest.width;

    // 动态分配内存
    Toimage.imagedata = new unsigned char* [Toimage.height];
    for (int i = 0; i < Toimage.height; ++i) {
        Toimage.imagedata[i] = new unsigned char[Toimage.width];
    }

    // 计算灰度值
    for (int y = 0; y < Toimage.height; ++y) {
        for (int x = 0; x < Toimage.width; ++x) {
            if (grayimagest.imagedata[y][x] < threshold)
                Toimage.imagedata[y][x] = 255;
            else
                Toimage.imagedata[y][x] = 0;
        }
    }

    return Toimage;
}

vector<vector<unsigned char>> Changeimage(unsigned char** imagedata, int height, int width)
{
    vector<vector<unsigned char>> newimageData(height, vector<unsigned char>(width * 3, 0));

    // 从二维数组中提取像素数据
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            newimageData[y][x * 3 + 0] = imagedata[y][x]; // R
            newimageData[y][x * 3 + 1] = imagedata[y][x];   // G
            newimageData[y][x * 3 + 2] = imagedata[y][x];   // B
        }
    }
    return newimageData;
}

int adaptiveThreshold(GrayImage grayimagest) {
    int threshold=127;
    for (int i = 0; i < grayimagest.height; i++) {
            for (int j = 0; j < grayimagest.width; j++) {
                // 计算局部均值作为阈值（简单示例）
                int sum = 0, count = 0;
                for (int m = -5; m <= 5; m++) {
                    for (int n = -5; n <= 5; n++) {
                        int x = i + m, y = j + n;
                        if (x >= 0 && x < grayimagest.height && y >= 0 && y < grayimagest.width) {
                            sum += grayimagest.imagedata[x][y];
                            count++;
                        }
                    }
                }
                int threshold = sum / count;

            }
    }
    return threshold;
}

int otsuThreshold(GrayImage grayimagest) {
    int hist[256] = {0};
    int totalPixels = grayimagest.height * grayimagest.width;

    // 计算直方图
    for (int i = 0; i < grayimagest.height; i++) {
        for (int j = 0; j < grayimagest.width; j++) {
            hist[grayimagest.imagedata[i][j]]++;
        }
    }

    double sum = 0, sumB = 0;
    for (int i = 0; i < 256; i++) {
        sum += i * hist[i];
    }

    int wB = 0, wF = 0;
    double varMax = 0, threshold = 0;

    for (int i = 0; i < 256; i++) {
        wB += hist[i]; // 背景权重
        if (wB == 0) continue;

        wF = totalPixels - wB; // 前景权重
        if (wF == 0) break;

        sumB += (double)(i * hist[i]);
        double mB = sumB / wB; // 背景均值
        double mF = (sum - sumB) / wF; // 前景均值

        double varBetween = (double)wB * (double)wF * pow(mB - mF, 2); // 类间方差

        if (varBetween > varMax) {
            varMax = varBetween;
            threshold = i;
        }
    }

    return threshold;
}

/*
int triangleThreshold(GrayImage grayimagest) {
    int hist[256] = {0};
    int totalPixels = grayimagest.height * grayimagest.width;

    // 计算直方图
    for (int i = 0; i < grayimagest.height; i++) {
        for (int j = 0; j < grayimagest.width; j++) {
            hist[grayimagest.imagedata[i][j]]++;
        }
    }

    // 平滑直方图（简单移动平均）
    int smoothedHist[256];
    for (int i = 0; i < 256; i++) {
        int sum = 0, count = 0;
        for (int k = -5; k <= 5; k++) {
            int idx = i + k;
            if (idx >= 0 && idx < 256) {
                sum += hist[idx];
                count++;
            }
        }
        smoothedHist[i] = (count > 0) ? (sum / count) : 0; // 避免除零错误
    }

    // 找到直方图的最大值（峰值）
    int peak = 0;
    for (int i = 0; i < 256; i++) {
        if (smoothedHist[i] > smoothedHist[peak]) {
            peak = i;
        }
    }

    // 找到峰值右侧的第一个零点
    int zeroPoint = -1;
    for (int i = peak + 1; i < 256; i++) {
        if (smoothedHist[i] == 0) {
            zeroPoint = i;
            break;
        }
    }

    // 如果没有找到零点，则外推直线
    if (zeroPoint == -1) {
        zeroPoint = 255; // 默认取最大灰度值
    }

    // 确保 slope 的计算不会出现除零错误
    double slope = 0.0;
    if (smoothedHist[peak] != smoothedHist[zeroPoint]) {
        slope = (double)(zeroPoint - peak) / (smoothedHist[peak] - smoothedHist[zeroPoint]);
    } else {
        slope = 1e9; // 设置一个极大值避免除零
    }

    double intercept = zeroPoint - slope * smoothedHist[zeroPoint];

    // 找到直线与x轴的交点作为阈值
    int threshold = -1;
    if (fabs(slope) > 1e-6) { // 避免除零错误
        threshold = (int)(intercept / slope);
    } else {
        threshold = peak; // 如果斜率接近零，直接取峰值作为阈值
    }

    // 确保阈值在合理范围内
    if (threshold < 0 || threshold > 255) {
        threshold = (peak + zeroPoint) / 2; // 如果阈值超出范围，取峰值和零点的中点
    }

    return threshold;
}
*/

int Triangle(GrayImage grayimagest)
{

    int i = 0,j = 0;
    int gray;
    int temp = 0;
    bool isflipped = false;

    QVector<int> histogram(256);



    for (int i = 0; i < grayimagest.height; i++) {
        for (int j = 0; j < grayimagest.width; j++) {
            histogram[grayimagest.imagedata[i][j]]++;
        }
    }
    //3. 寻找直方图中两侧边界
    int left_bound = 0;
    int right_bound = 0;
    int max = 0;
    int max_index = 0;

    //左侧为零的位置
    for(i = 0; i<256; i++)
    {
        if(histogram[i]>0)
        {
            left_bound = i;
            break;
        }
    }
    //直方图为零的位置
    if(left_bound > 0)
    {
        left_bound --;
    }


    //直方图右侧为零的位置
    for(i = 255; i>0; i--)
    {
        if(histogram[i]>0)
        {
            right_bound = i;
            break;
        }
    }
    //直方图为零的地方
    if(right_bound >0)
    {
        right_bound++;
    }

    //4. 寻找直方图最大值
    for(i = 0; i<256;i++)
    {
        if(histogram[i] > max)
        {
            max = histogram[i];
            max_index = i;
        }
    }

    //判断最大值是否在最左侧，如果是则不用翻转
    //因为三角法二值化只能适用于最大值在最右侧
    if(max_index - left_bound  < right_bound - max_index)
    {
        isflipped = true;
        i = 0;
        j = 255;
        while( i < j )
        {
            // 左右交换
            temp = histogram[i]; histogram[i] = histogram[j]; histogram[j] = temp;
            i++; j--;
        }
        left_bound = 255-right_bound;
        max_index = 255-max_index;
    }


    // 计算求得阈值
    double thresh = left_bound;
    double a, b, dist = 0, tempdist;
    a = max; b = left_bound-max_index;
    for( i = left_bound+1; i <= max_index; i++ )
    {
       // 计算距离 - 不需要真正计算
       tempdist = a*i + b*histogram[i];
       if( tempdist > dist)
       {
           dist = tempdist;
           thresh = i;
       }
    }
    thresh--;

    // 对已经得到的阈值T,如果前面已经翻转了，则阈值要用255-T
    if( isflipped )
    {
         thresh = 255-thresh;
    }



    return thresh;

}
