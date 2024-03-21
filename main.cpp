#include "Image_Class.h"
#include <bits/stdc++.h>

using namespace std;

void invertFilter(Image& img) {
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            for (int c = 0; c < img.channels; c++) {
                img(x, y, c) = 255 - img(x, y, c);
            }
        }
    }
}

void rotateFilter(Image& img, int angle) {
    int newWidth = (angle % 180 == 0) ? img.width : img.height;
    int newHeight = (angle % 180 == 0) ? img.height : img.width;
    unsigned char* newImageData = (unsigned char*)malloc(newWidth * newHeight * img.channels);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            for (int c = 0; c < img.channels; c++) {
                int newX, newY;
                switch(angle) {
                    case 90:
                        newX = y;
                        newY = img.width - x - 1;
                        break;
                    case 180:
                        newX = img.width - x - 1;
                        newY = img.height - y - 1;
                        break;
                    case 270:
                        newX = img.height - y - 1;
                        newY = x;
                        break;
                }
                newImageData[(newY * newWidth + newX) * img.channels + c] = img(x, y, c);
            }
        }
    }

    stbi_image_free(img.imageData);
    img.imageData = newImageData;
    img.width = newWidth;
    img.height = newHeight;
}

void frameFilter(Image& img, int thickness, unsigned char r, unsigned char g, unsigned char b) {
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            if (x < thickness || x >= img.width - thickness || y < thickness || y >= img.height - thickness) {
                img(x, y, 0) = r;
                img(x, y, 1) = g;
                img(x, y, 2) = b;
            }
        }
    }
}

int main() {
    string inputFileName, outputFileName;
    cout << "Enter the input file name with extension: ";
    cin >> inputFileName;
    cout << "Enter the output file name with extension: ";
    cin >> outputFileName;

    Image img;
    img.loadNewImage(inputFileName);

    int filterChoice;
    cout << "Enter the filter choice (1 for invert, 2 for rotate, 3 for frame): ";
    cin >> filterChoice;

    int angle;
    unsigned char r, g, b;
    int thickness;
    int colorChoice;

    switch(filterChoice) {
        case 1:
            invertFilter(img);
            img.saveImage(outputFileName);
            break;
        case 2:
            cout << "Enter the rotation angle (90, 180, 270): ";
            cin >> angle;
            rotateFilter(img, angle);
            img.saveImage(outputFileName);
            break;
        case 3:
            cout << "Enter the frame thickness: ";
            cin >> thickness;
            cout << "Choose the frame color (1 for Red, 2 for Orange, 3 for Yellow): ";
            cin >> colorChoice;
            switch(colorChoice) {
                case 1: // Red
                    r = 255; g = 0; b = 0;
                    break;
                case 2: // Orange
                    r = 255; g = 165; b = 0;
                    break;
                case 3: // Yellow
                    r = 255; g = 255; b = 0;
                    break;
                default:
                    cout << "Invalid color choice" << endl;
                    return 0; // Exit the program if the color choice is invalid
            }
            frameFilter(img, thickness, r, g, b);
            img.saveImage(outputFileName);
            break;
        default:
            cout << "Invalid filter choice" << endl;
            break;
    }

    return 0;
}