#include <bits/stdc++.h>
#include "Image_Class.h"

using namespace std;
#define endl '\n'
#define ll long long

void copyImage(Image &img, const Image &other) {
    if (&img != &other) { // protect against invalid self-assignment
        // 1: deallocate old memory
        if (img.imageData != nullptr) {
            stbi_image_free(img.imageData);
        }

        // 2: allocate new memory and copy the elements
        img.width = other.width;
        img.height = other.height;
        img.channels = other.channels;
        img.imageData = (unsigned char *) malloc(img.width * img.height * img.channels);
        memcpy(img.imageData, other.imageData, img.width * img.height * img.channels);
    }
}

void greyScale(Image &img) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            unsigned av = 0;
            for (int k = 0; k < 3; ++k) {
                av += img(i, j, k);
            }
            av /= 3;
            for (int k = 0; k < 3; ++k) {
                img(i, j, k) = av;
            }
        }
    }
}

void BlackAndWhite(Image &Img) {
    int z = 0;
    for (int i = 0; i < Img.width; i++) {
        for (int j = 0; j < Img.height; j++) {
            unsigned int a = 0;
            for (int k = 0; k < 3; k++) {
                a += Img(i, j, k);
            }
            if (a / 3 < 129) {
                z = 0;
            } else z = 255;
            for (int s = 0; s < 3; s++) {
                Img(i, j, s) = z;
            }
        }
    }
}

void invertFilter(Image &img) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                img(i, j, c) = 255 - img(i, j, c);
            }
        }
    }
}

void mergeImages(Image &img1, Image &img2) {
    int newWidth = min(img1.width, img2.width);
    int newHeight = min(img1.height, img2.height);
    for (int i = 0; i < newWidth; i++) {
        for (int j = 0; j < newHeight; j++) {
            for (int k = 0; k < 3; ++k) {
                img1(i, j, k) = img1(i * img1.width / newWidth, j * img1.height / newHeight, k);
                img2(i, j, k) = img2(i * img2.width / newWidth, j * img2.height / newHeight, k);
            }
        }
    }
    Image newImg(img1.width, img1.height);
    for (int i = 0; i < img1.width; i++) {
        for (int j = 0; j < img1.height; j++) {
            for (int k = 0; k < 3; ++k) {
                newImg(i, j, k) = (img1(i, j, k) + img2(i, j, k)) / 2;
            }
        }
    }
    copyImage(img1, newImg);
}

void FlipHorizontal(Image &Img) {
    int z = 0;
    int count = Img.width - 1;
    for (int i = 0; i < Img.width / 2; i++) {
        for (int j = 0; j < Img.height; j++) {
            unsigned int a = 0;
            for (int s = 0; s < 3; s++) {
                z = Img(count - i, j, s);
                Img(count - i, j, s) = Img(i, j, s);
                Img(i, j, s) = z;
            }
        }
    }
    Img.saveImage("flip.png");
    cout << "end";
}

void FlipVertical(Image &Img) {
    int z = 0;
    int count = Img.height - 1;
    for (int i = 0; i < Img.width; i++) {
        for (int j = 0; j < Img.height / 2; j++) {
            unsigned int a = 0;
            for (int s = 0; s < 3; s++) {
                z = Img(i, count - j, s);
                Img(i, count - j, s) = Img(i, j, s);
                Img(i, j, s) = z;
            }
        }
    }
    Img.saveImage("flip.png");
    cout << "end";
}

void rotate90(Image &img, Image &newImg) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                newImg(img.height - 1 - j, i, c) = img(i, j, c);
            }
        }
    }
}
// Function to rotate an image 180 degrees

void rotate180(Image &img, Image &newImg) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                newImg(img.width - 1 - i, img.height - 1 - j, c) = img(i, j, c);
            }
        }
    }
}

// Function to rotate an image 270 degrees clockwise
void rotate270(Image &img, Image &newImg) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                newImg(j, img.width - 1 - i, c) = img(i, j, c);
            }
        }
    }
}

// Function to apply a blur filter to an image
// The blur filter blurs the image by averaging the color of each pixel with the color of its neighbors
void Darken(Image &img) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int k = 0; k < 3; ++k) {
                img(i, j, k) = img(i, j, k) / 2;
            }
        }
    }
}

void Brighten(Image &img) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int k = 0; k < 3; ++k) {
                img(i, j, k) = min(255, img(i, j, k) * 2);
            }
        }
    }
}

void Crop(Image &Img, int x, int y, int w, int h) {
    Image Img2(w, h);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            for (int k = 0; k < 3; k++) {

                Img2(i, j, k) = Img(i + x, j + y, k);
            }
        }
    }
    copyImage(Img, Img2);
}

int FRAME_WIDTH = 10;

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

    int frameThickness = 3;
    if (style == "simple") {
        // Draw a gradient border around the entire image
        for (int i = 0; i < img.width; i++) {
            for (int j = 0; j < img.height; j++) {
                if (i < FRAME_WIDTH || i >= img.width - FRAME_WIDTH || j < FRAME_WIDTH ||
                    j >= img.height - FRAME_WIDTH) {
                    double distance = sqrt((i - img.width / 2.0) * (i - img.width / 2.0) +
                                           (j - img.height / 2.0) * (j - img.height / 2.0));
                    double maxDistance = sqrt(
                            (img.width / 2.0) * (img.width / 2.0) + (img.height / 2.0) * (img.height / 2.0));
                    unsigned char gradient = (unsigned char) ((distance / maxDistance) * 255 *
                                                              2.0); // Multiply by 1.5 to make the gradient stronger
                    img(i, j, 0) = r * (1 - gradient / 255.0);
                    img(i, j, 1) = g * (1 - gradient / 255.0);
                    img(i, j, 2) = b * (1 - gradient / 255.0);
                }
            }
        }
    } else if (style == "fancy") {
        FRAME_WIDTH = 20;
        // Draw a double border around the original image with a circle in the corner squares
        for (int i = 0; i < img.width; i++) {
            for (int j = 0; j < img.height; j++) {
                if (i < FRAME_WIDTH || i >= img.width - FRAME_WIDTH || j < FRAME_WIDTH ||
                    j >= img.height - FRAME_WIDTH) {
                    if ((i >= 0 && i < frameThickness) || (i >= FRAME_WIDTH - frameThickness && i < FRAME_WIDTH) ||
                        (i >= img.width - FRAME_WIDTH && i < img.width - FRAME_WIDTH + frameThickness) ||
                        (i >= img.width - frameThickness && i < img.width) ||
                        (j >= 0 && j < frameThickness) || (j >= FRAME_WIDTH - frameThickness && j < FRAME_WIDTH) ||
                        (j >= img.height - FRAME_WIDTH && j < img.height - FRAME_WIDTH + frameThickness) ||
                        (j >= img.height - frameThickness && j < img.height)) {
                        img(i, j, 0) = r;
                        img(i, j, 1) = g;
                        img(i, j, 2) = b;
                    } else if ((i - FRAME_WIDTH) * (i - FRAME_WIDTH) + (j - FRAME_WIDTH) * (j - FRAME_WIDTH) <=
                               FRAME_WIDTH * FRAME_WIDTH / 4 || (i - FRAME_WIDTH) * (i - FRAME_WIDTH) +
                                                                (j - img.height + FRAME_WIDTH) *
                                                                (j - img.height + FRAME_WIDTH) <=
                                                                FRAME_WIDTH * FRAME_WIDTH / 4 ||
                               (i - img.width + FRAME_WIDTH) * (i - img.width + FRAME_WIDTH) +
                               (j - FRAME_WIDTH) * (j - FRAME_WIDTH) <= FRAME_WIDTH * FRAME_WIDTH / 4 ||
                               (i - img.width + FRAME_WIDTH) * (i - img.width + FRAME_WIDTH) +
                               (j - img.height + FRAME_WIDTH) * (j - img.height + FRAME_WIDTH) <=
                               FRAME_WIDTH * FRAME_WIDTH / 4) {
                        img(i, j, 0) = r;
                        img(i, j, 1) = g;
                        img(i, j, 2) = b;
                    }
                }
            }
        }
    }
}

void edgeDetection(Image &img) {
    greyScale(img);
    int kernelx[3][3] = {{-1, 0, 1},
                         {-2, 0, 2},
                         {-1, 0, 1}};
    int kernely[3][3] = {{1,  2,  1},
                         {0,  0,  0},
                         {-1, -2, -1}};
    int thereshold = 100;
    Image newImg(img.width, img.height);
    for (int i = 1; i < img.width - 1; i++) {
        for (int j = 1; j < img.height - 1; j++) {
            for (int k = 0; k < 3; ++k) {
                int sum1 = 0, sum2 = 0;
                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        sum1 += img(i + x, j + y, k) * kernelx[x + 1][y + 1];
                        sum2 += img(i + x, j + y, k) * kernely[x + 1][y + 1];
                    }
                }
                float gradient_magnitude = sqrt(pow(sum1, 2) + pow(sum2, 2)) / 2;
                newImg(i, j, k) = (gradient_magnitude > thereshold) ? 0 : 255;
            }
        }
    }
    copyImage(img, newImg);
}

void Resize(Image &Img, int x_new, int y_new) {
    int wid = Img.width;
    int heig = Img.height;
    double z = (double) wid / x_new;
    double z1 = (double) heig / y_new;
    Image Imag(x_new, y_new);
    for (int i = 0; i < x_new; i++) {
        for (int j = 0; j < y_new; j++) {
            int x1 = i * z;
            int y1 = j * z1;
            for (int k = 0; k < 3; k++) {
                Imag(i, j, k) = Img(x1, y1, k);
            }
        }
    }
    copyImage(Img, Imag);
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

void sunlightFilter(Image &img) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            // Increase red and green channels, decrease blue channel
            img(i, j, 0) = min(255, img(i, j, 0) + 30); // Red
            img(i, j, 1) = min(255, img(i, j, 1) + 25); // Green
            img(i, j, 2) = max(0, img(i, j, 2) - 30); // Blue
        }
    }
}

void oilPaintingFilter(Image &img) {
    int levels = 7;
    double contrast = 1.09;
    double avgColor = 0;
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < min(3, img.channels); c++) {
                avgColor += img(i, j, c);
            }
        }
    }
    avgColor /= (img.width * img.height * min(3, img.channels));

    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < min(3, img.channels); c++) {
                int color = img(i, j, c);
                int newColor = ((color * levels) / 256) * (256 / levels);

                // Adjust contrast
                double diff = newColor - avgColor;
                diff *= contrast;
                newColor = avgColor + diff;

                // Clamp the new color value between 0 and 255
                newColor = max(0, min(255, newColor));

                img(i, j, c) = newColor;
            }
        }
    }
}

void oilPainting(Image &img) {
    float radius = 1;
    Image newImg(img.width, img.height);
    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {
            vector<int> intensityCount(256, 0);
            vector<int> averageR(256, 0);
            vector<int> averageG(256, 0);
            vector<int> averageB(256, 0);
            for (int dx = -radius; dx <= radius; ++dx) {
                for (int dy = -radius; dy <= radius; ++dy) {
                    int nx = i + dx;
                    int ny = j + dy;
                    if (nx >= 0 && nx < img.width && ny >= 0 && ny < img.height) {
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
                    for (int i = 0; i < 256; ++i) {
                        if (intensityCount[i] > curMax) {
                            curMax = intensityCount[i];
                            maxIndex = i;
                        }
                    }
                    if (curMax != 0) {
                        newImg(i, j, 0) = averageR[maxIndex] / curMax;
                        newImg(i, j, 1) = averageG[maxIndex] / curMax;
                        newImg(i, j, 2) = averageB[maxIndex] / curMax;
                    }
                }
            }
        }
    }
    copyImage(img, newImg);
}

void tvFilter(Image &img) {
    srand(time(0)); // Seed the random number generator
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                // Increase blue channel
                if (c == 2) { // Blue channel
                    int new_value = img(i, j, c) + 50;
                    img(i, j, c) = new_value > 255 ? 255 : new_value;
                }
                // Decrease red and green channels
                if (c == 0 || c == 1) { // Red or Green channel
                    int new_value = img(i, j, c) - 20;
                    img(i, j, c) = new_value < 0 ? 0 : new_value;
                }
                // Add noise
                int noise = rand() % 40 - 20; // Generate a random number between -20 and 20
                int new_value = img(i, j, c) + noise;
                img(i, j, c) = new_value < 0 ? 0 : new_value > 255 ? 255 : new_value;
            }
        }
    }
}

void nightPurpleFilter(Image &img) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                // Increase red and blue channels
                if (c == 0 || c == 2) { // Red or Blue channel
                    img(i, j, c) = min(255.0, img(i, j, c) * 1.155);
                }
                if (c == 1) { // Green channel
                    img(i, j, c) = min(255.0, img(i, j, c) * 0.8);
                }
            }
        }
    }
}

void infraredFilter(Image &img) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            double a = 0;
            a = img(i, j, 0) * 0.299 + img(i, j, 1) * 0.587 + img(i, j, 2) * 0.114;
            img(i, j, 0) = 255;
            img(i, j, 1) = 255 - a;
            img(i, j, 2) = 255 - a;

        }
    }
}

void skewImage(Image &img, double skewFactor) {
    skewFactor = -skewFactor;
    // Calculate the skew factor in pixels
    int skewPixels = -tan(skewFactor) * img.height;

    // Calculate the new dimensions of the image
    int newWidth = img.width + abs(skewPixels);
    int newHeight = img.height;

    // Calculate the horizontal offset to center the skewed image
    int offsetX = skewPixels < 0 ? abs(skewPixels) : 0;

    // Create a new image with the new dimensions
    Image newImg(newWidth, newHeight);

    // Iterate over each pixel in the original image
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            // Calculate the new position of the pixel
            int newX = i + skewPixels * (j / (double) img.height) +
                       offsetX; // Adjust the x-coordinate based on skew factor and offset
            int newY = j;

            // Check if the new position is within the bounds of the new image
            if (newX >= 0 && newX < newWidth && newY >= 0 && newY < newHeight) {
                // Set the color of the new position to the color of the original pixel
                for (int c = 0; c < img.channels; c++) {
                    newImg(newX, newY, c) = img(i, j, c);
                }
            }
        }
    }

    // Replace the original image with the new image
    copyImage(img, newImg);
}

void applyFilters(Image &img, string outputFilename) {
    int choice;
    int x, y, w, h;
    int versions;
    int x_new, y_new;
    string style, color;
    string secondImageFilename;
    Image secondImg;
    Image img90(img.height, img.width);
    Image img180(img.width, img.height);
    Image img270(img.height, img.width);
    do {
        cout
                << "1. Grey Scale\n2. Black and White\n3. Invert Filter\n4. Merge Images\n5. Flip Horizontal\n6. Flip Vertical\n7. Rotate 90\n8. Rotate 180\n9. Rotate 270\n10. Darken\n11. Brighten\n12. Crop\n13. Add Frame\n14. Edge Detection\n15. Resize\n16. Blur Filter\n17. Sunlight Filter\n18. Oil Painting Filter\n19. TV Filter\n20. Night Purple Filter\n21. Infrared Filter\n22. Skew Image\n23. Save and Exit\nEnter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                greyScale(img);
                break;
            case 2:
                BlackAndWhite(img);
                break;
            case 3:
                invertFilter(img);
                break;
            case 4:
                cout << "Enter second image filename: ";
                cin >> secondImageFilename;
                secondImg = Image(secondImageFilename);
                mergeImages(img, secondImg);
                break;
            case 5:
                FlipHorizontal(img);
                break;
            case 6:
                FlipVertical(img);
                break;
            case 7:
                rotate90(img, img90);
                copyImage(img, img90);
                break;
            case 8:
                rotate180(img, img180);
                copyImage(img, img180);
                break;
            case 9:
                rotate270(img, img270);
                copyImage(img, img270);
                break;
            case 10:
                Darken(img);
                break;
            case 11:
                Brighten(img);
                break;
            case 12:
                cout << "Enter x, y, width, height: ";
                cin >> x >> y >> w >> h;
                Crop(img, x, y, w, h);
                break;
            case 13:
                cout << "Enter frame style (simple or fancy): ";
                cin >> style;
                cout << "Enter frame color (RED, GREEN, BLUE, WHITE): ";
                cin >> color;
                addFrame(img, style, color);
                break;
            case 14:
                edgeDetection(img);
                break;
            case 15:
                cout << "Enter new width and height: ";
                cin >> x_new >> y_new;
                Resize(img, x_new, y_new);
                break;
            case 16:
                int blurLevel;
                cout << "Enter blur level: ";
                cin >> blurLevel;
                blurFilter(img, blurLevel);
                break;
            case 17:
                sunlightFilter(img);
                break;
            case 18:
                cout << "Enter your version (1, 2): ";
                cin >> versions;
                if (versions == 1) {
                    oilPaintingFilter(img);
                }  else {
                    oilPainting(img);
                }
                break;
            case 19:
                tvFilter(img);
                break;
            case 20:
                nightPurpleFilter(img);
                break;
            case 21:
                infraredFilter(img);
                break;
            case 22:
                double skewFactor;
                cout << "Enter skew factor: ";
                cin >> skewFactor;
                skewImage(img, skewFactor);
                break;
            case 23:
                img.saveImage(outputFilename);
                break;
            default:
                cout << "Invalid choice\n";
                break;
        }
    } while (choice != 23);
}

int main() {
    string inputFilename = "luffy.jpg", outputFilename = "tt.png";
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