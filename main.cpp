#include <bits/stdc++.h>
#include "Image_Class.h"
using namespace std;
#define endl '\n'
#define ll long long

void greyScale(Image &img)
{
    for (int i = 0; i < img.width; i++)
    {
        for (int j = 0; j < img.height; j++)
        {
            unsigned av = 0;
            for (int k = 0; k < 3; ++k)
            {
                av += img(i, j, k);
            }
            av /= 3;
            for (int k = 0; k < 3; ++k)
            {
                img(i, j, k) = av;
            }
        }
    }
    img.saveImage("grey.jpg");
}
void Darken(Image &img)
{
    for (int i = 0; i < img.width; i++)
    {
        for (int j = 0; j < img.height; j++)
        {
            for (int k = 0; k < 3; ++k)
            {
                img(i, j, k) = img(i, j, k) / 2;
            }
        }
    }
    img.saveImage("darken.jpg");
}
void Brighten(Image &img)
{
    for (int i = 0; i < img.width; i++)
    {
        for (int j = 0; j < img.height; j++)
        {
            for (int k = 0; k < 3; ++k)
            {
                img(i, j, k) = min(255, img(i, j, k) * 2);
            }
        }
    }
    img.saveImage("brighten.jpg");
}
void edgeDetection(Image &img)
{
    greyScale(img);
    int kernelx[3][3] = {{-1, 0, 1},
                         {-2, 0, 2},
                         {-1, 0, 1}};
    int kernely[3][3] = {{1, 2, 1},
                         {0, 0, 0},
                         {-1, -2, -1}};
    int thereshold = 100;
    Image newImg(img.width, img.height);
    for (int i = 1; i < img.width - 1; i++)
    {
        for (int j = 1; j < img.height - 1; j++)
        {
            for (int k = 0; k < 3; ++k)
            {
                int sum1 = 0, sum2 = 0;
                for (int x = -1; x <= 1; x++)
                {
                    for (int y = -1; y <= 1; y++)
                    {
                        sum1 += img(i + x, j + y, k) * kernelx[x + 1][y + 1];
                        sum2 += img(i + x, j + y, k) * kernely[x + 1][y + 1];
                    }
                }
                float gradient_magnitude = sqrt(pow(sum1, 2) + pow(sum2, 2)) / 2;
                newImg(i, j, k) = (gradient_magnitude > thereshold) ? 0 : 255;
            }
        }
    }
    newImg.saveImage("edgeDetection.jpg");
}
// Function to calculate intensity level
void oilPainting(Image &img)
{
    float radius = 1;
    Image newImg(img.width, img.height);
    for (int i = 0; i < img.width; ++i)
    {
        for (int j = 0; j < img.height; ++j)
        {
            vector<int> intensityCount(256, 0);
            vector<int> averageR(256, 0);
            vector<int> averageG(256, 0);
            vector<int> averageB(256, 0);
            for (int dx = -radius; dx <= radius; ++dx)
            {
                for (int dy = -radius; dy <= radius; ++dy)
                {
                    int nx = i + dx;
                    int ny = j + dy;
                    if (nx >= 0 && nx < img.width && ny >= 0 && ny < img.height)
                    {
                        unsigned curR = img(nx, ny, 0);
                        unsigned curG = img(nx, ny, 1);
                        unsigned curB = img(nx, ny, 2);
                        int intensityBin = (double(curR + curG + curG) / 3);
                        intensityCount[intensityBin]++;
                        averageR[intensityBin] += curR;
                        averageG[intensityBin] += curG;
                        averageB[intensityBin] += curB;
                    }
                    int curMax = 0;
                    int maxIndex = 0;
                    for (int i = 0; i < 256; ++i)
                    {
                        if (intensityCount[i] > curMax)
                        {
                            curMax = intensityCount[i];
                            maxIndex = i;
                        }
                    }
                    if (curMax != 0)
                    {
                        newImg(i, j, 0) = averageR[maxIndex] / curMax;
                        newImg(i, j, 1) = averageG[maxIndex] / curMax;
                        newImg(i, j, 2) = averageB[maxIndex] / curMax;
                    }
                }
            }
        }
    }
    newImg.saveImage("oilPainting.jpg");
}
void posterize(Image& img, int levels = 7) {
    Image newImg(img.width, img.height);
    for (int y = 0; y < img.height; ++y) {
        for (int x = 0; x < img.width; ++x) {
            // Reduce the number of distinct color levels
            int r = ((img(x,y,0) * levels) / 255) * (255 / levels);
            int g = ((img(x,y,1) * levels) / 255) * (255 / levels);
            int b = ((img(x,y,2) * levels) / 255) * (255 / levels);
            newImg(x,y,0) = r;
            newImg(x,y,1) = g;
            newImg(x,y,2) = b;
        }
    }
    newImg.saveImage("posterize.jpg");
}
void mergeImages(Image &img1, Image &img2)
{
    int newWidth = min(img1.width, img2.width);
    int newHeight = min(img1.height, img2.height);
    for (int i = 0; i < newWidth; i++)
    {
        for (int j = 0; j < newHeight; j++)
        {
            for (int k = 0; k < 3; ++k)
            {
                img1(i, j, k) = img1(i * img1.width / newWidth, j * img1.height / newHeight, k);
                img2(i, j, k) = img2(i * img2.width / newWidth, j * img2.height / newHeight, k);
            }
        }
    }
    Image newImg(img1.width, img1.height);
    for (int i = 0; i < img1.width; i++)
    {
        for (int j = 0; j < img1.height; j++)
        {
            for (int k = 0; k < 3; ++k)
            {
                newImg(i, j, k) = (img1(i, j, k) + img2(i, j, k)) / 2;
            }
        }
    }
    newImg.saveImage("mergeImages.jpg");
}
int main()
{
    Image img("farm.jpeg");
    posterize(img);
}