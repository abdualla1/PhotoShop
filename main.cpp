#include "Image_Class.h"
#include <bits/stdc++.h>

using namespace std;
const int FRAME_WIDTH = 10;
void invertFilter(Image &img) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                img(i, j, c) = 255 - img(i, j, c);
            }
        }
    }
}

void rotate90(Image &img, Image &newImg) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                newImg(j, img.width - 1 - i, c) = img(i, j, c);
            }
        }
    }
}

void rotate180(Image &img, Image &newImg) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                newImg(img.width - 1 - i, img.height - 1 - j, c) = img(i, j, c);
            }
        }
    }
}

void rotate270(Image &img, Image &newImg) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                newImg(img.height - 1 - j, i, c) = img(i, j, c);
            }
        }
    }
}

void blurFilter(Image &img, int blurLevel) {
    // Create a padded image
    Image paddedImg(img.width + 2 * blurLevel, img.height + 2 * blurLevel);
    for (int i = 0; i < paddedImg.width; i++) {
        for (int j = 0; j < paddedImg.height; j++) {
            for (int c = 0; c < paddedImg.channels; c++) {
                if (i < blurLevel || i >= img.width + blurLevel || j < blurLevel || j >= img.height + blurLevel) {
                    // Padding pixel, copy color from nearest pixel in original image
                    int nearestI = std::min(std::max(i - blurLevel, 0), img.width - 1);
                    int nearestJ = std::min(std::max(j - blurLevel, 0), img.height - 1);
                    paddedImg(i, j, c) = img(nearestI, nearestJ, c);
                } else {
                    // Non-padding pixel, copy color from original image
                    paddedImg(i, j, c) = img(i - blurLevel, j - blurLevel, c);
                }
            }
        }
    }

    // Apply the blur filter
    int kernelSize = 2 * blurLevel + 1;
    for (int i = blurLevel; i < img.width + blurLevel; i++) {
        for (int j = blurLevel; j < img.height + blurLevel; j++) {
            for (int c = 0; c < img.channels; c++) {
                int sum = 0;
                for (int dx = -blurLevel; dx <= blurLevel; dx++) {
                    for (int dy = -blurLevel; dy <= blurLevel; dy++) {
                        sum += paddedImg(i + dx, j + dy, c);
                    }
                }
                img(i - blurLevel, j - blurLevel, c) = sum / (kernelSize * kernelSize);
            }
        }
    }
}

void addFrame(Image &img, string style, string color) {
    unsigned char r = 0, g = 0, b = 0;
    if (color == "RED") {
        r = 255;
    } else if (color == "GREEN") {
        g = 255;
    } else if (color == "BLUE") {
        b = 255;
    } else if (color == "WHITE") {
        r = 255;
        g = 255;
        b = 255;
    }

    if (style == "simple") {
        // Draw a gradient border around the entire image
        for (int i = 0; i < img.width; i++) {
            for (int j = 0; j < img.height; j++) {
                if (i < FRAME_WIDTH || i >= img.width - FRAME_WIDTH || j < FRAME_WIDTH || j >= img.height - FRAME_WIDTH) {
                    double distance = sqrt((i - img.width / 2.0) * (i - img.width / 2.0) + (j - img.height / 2.0) * (j - img.height / 2.0));
                    double maxDistance = sqrt((img.width / 2.0) * (img.width / 2.0) + (img.height / 2.0) * (img.height / 2.0));
                    unsigned char gradient = (unsigned char) ((distance / maxDistance) * 255 * 2.0); // Multiply by 1.5 to make the gradient stronger
                    img(i, j, 0) = r * (1 - gradient / 255.0);
                    img(i, j, 1) = g * (1 - gradient / 255.0);
                    img(i, j, 2) = b * (1 - gradient / 255.0);
                }
            }
        }
    } else if (style == "fancy") {
        // Draw a double border around the original image with a circle in the corner squares
        for (int i = 0; i < img.width; i++) {
            for (int j = 0; j < img.height; j++) {
                if (i < FRAME_WIDTH || i >= img.width - FRAME_WIDTH || j < FRAME_WIDTH || j >= img.height - FRAME_WIDTH) {
                    if (i == 0 || i == FRAME_WIDTH - 1 || i == img.width - 1 || i == img.width - FRAME_WIDTH || j == 0 || j == FRAME_WIDTH - 1 || j == img.height - 1 || j == img.height - FRAME_WIDTH) {
                        img(i, j, 0) = r;
                        img(i, j, 1) = g;
                        img(i, j, 2) = b;
                    } else if ((i - FRAME_WIDTH) * (i - FRAME_WIDTH) + (j - FRAME_WIDTH) * (j - FRAME_WIDTH) <= FRAME_WIDTH * FRAME_WIDTH / 4 || (i - FRAME_WIDTH) * (i - FRAME_WIDTH) + (j - img.height + FRAME_WIDTH) * (j - img.height + FRAME_WIDTH) <= FRAME_WIDTH * FRAME_WIDTH / 4 || (i - img.width + FRAME_WIDTH) * (i - img.width + FRAME_WIDTH) + (j - FRAME_WIDTH) * (j - FRAME_WIDTH) <= FRAME_WIDTH * FRAME_WIDTH / 4 || (i - img.width + FRAME_WIDTH) * (i - img.width + FRAME_WIDTH) + (j - img.height + FRAME_WIDTH) * (j - img.height + FRAME_WIDTH) <= FRAME_WIDTH * FRAME_WIDTH / 4) {
                        img(i, j, 0) = r;
                        img(i, j, 1) = g;
                        img(i, j, 2) = b;
                    }
                }
            }
        }
    }
}
void applyFilters(Image &img, const string& outputFilename) {
    string style, color;
    int choice;
    do {
        cout << "1. Invert\n2. Rotate 90\n3. Rotate 180\n4. Rotate 270\n5. Blur\n6. Add Frame\n7. Save and Exit\nEnter your choice: ";
        cin >> choice;
        Image img90(img.height, img.width);
        Image img180(img.width, img.height);
        switch (choice) {
            case 1:
                invertFilter(img);
                cout << "Invert filter applied successfully.\n";
                break;
            case 2:
                rotate90(img, img90);
                cout << "Image rotated 90 degrees successfully.\n";
                img = img90; // Update img with rotated image
                break;
            case 3:
                rotate180(img, img180);
                cout << "Image rotated 180 degrees successfully.\n";
                img = img180; // Update img with rotated image
                break;
            case 4:
                rotate270(img, img90);
                cout << "Image rotated 270 degrees successfully.\n";
                img = img90; // Update img with rotated image
                break;
            case 5:
                int blurLevel;
                cout << "Enter blur level: ";
                cin >> blurLevel;
                blurFilter(img, blurLevel);
                cout << "Blur filter applied successfully.\n";
                break;
            case 6:
                cout << "Enter frame style (simple or fancy): ";
                cin >> style;
                cout << "Enter frame color (RED, GREEN, BLUE, WHITE): ";
                cin >> color;
                addFrame(img, style, color);
                cout << "Frame added successfully.\n";
                break;
            case 7:
                img.saveImage(outputFilename);
                cout << "Image saved successfully.\n";
                break;
            default:
                cout << "Invalid choice\n";
                break;
        }
    } while (choice != 7);
}

int main() {
    string inputFilename, outputFilename;
    int choice;
    do {
        cout << "Enter input filename: ";
        cin >> inputFilename;
        cout << "Enter output filename: ";
        cin >> outputFilename;

        Image img(inputFilename);
        applyFilters(img, outputFilename);

        cout << "Do you want to load another image? (1 for yes, 0 for no): ";
        cin >> choice;
    } while (choice == 1);

    cout << "Exiting...\n";

    return 0;
}