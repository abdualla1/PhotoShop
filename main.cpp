#include "Image_Class.h"
#include <bits/stdc++.h>

using namespace std;
const int FRAME_WIDTH = 10;
// Function to apply the invert filter to an image
// The invert filter inverts the color of each pixel in the image
void invertFilter(Image &img) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                img(i, j, c) = 255 - img(i, j, c);
            }
        }
    }
}
// Function to rotate an image 90 degrees clockwise

void rotate90( Image &img, Image &newImg) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                newImg(j, img.width - 1 - i, c) = img(i, j, c);
            }
        }
    }
}
// Function to rotate an image 180 degrees

void rotate180( Image &img, Image &newImg) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                newImg(img.width - 1 - i, img.height - 1 - j, c) = img(i, j, c);
            }
        }
    }
}
// Function to rotate an image 270 degrees clockwise
void rotate270( Image &img, Image &newImg) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                newImg(img.height - 1 - j, i, c) = img(i, j, c);
            }
        }
    }
}
// Function to apply a blur filter to an image
// The blur filter blurs the image by averaging the color of each pixel with the color of its neighbors

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
// Function to add a frame to an image
// The frame is added around the border of the image
// The frame can be either simple or fancy
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
// Function to copy the contents of one image to another
// The function first deallocates the memory of the destination image, then allocates new memory and copies the contents of the source image to the destination image

void copyImage(Image& img, const Image& other) {
    if (&img != &other) { // protect against invalid self-assignment
        // 1: deallocate old memory
        if (img.imageData != nullptr) {
            stbi_image_free(img.imageData);
        }

        // 2: allocate new memory and copy the elements
        img.width = other.width;
        img.height = other.height;
        img.channels = other.channels;
        img.imageData = (unsigned char*)malloc(img.width * img.height * img.channels);
        memcpy(img.imageData, other.imageData, img.width * img.height * img.channels);
    }
}
// Function to apply a series of filters to an image
// The function prompts the user to choose a filter, applies the chosen filter to the image, and repeats until the user chooses to save the image and exit
// Function to apply a "sunlight" filter to an image
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
void convertToGrayscale(Image &img) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            int gray = (img(i, j, 0) + img(i, j, 1) + img(i, j, 2)) / 3;
            img(i, j, 0) = gray;
            img(i, j, 1) = gray;
            img(i, j, 2) = gray;
        }
    }
}
void highPassFilter(Image &img, Image &blurredImg, int blurLevel = 5) {
    // Apply a low-pass filter (blur) to the image
    blurFilter(blurredImg, blurLevel);

    // Subtract the blurred image from the original image
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                int highPassValue = img(i, j, c) - blurredImg(i, j, c);
                img(i, j, c) = max(0, min(255, highPassValue));
            }
        }
    }
}
void oilPaintingFilter(Image &img, int levels = 7, double contrast = 1.09) {
    // Calculate the average color intensity
    double avgColor = 0;
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
                avgColor += img(i, j, c);
            }
        }
    }
    avgColor /= (img.width * img.height * img.channels);

    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int c = 0; c < img.channels; c++) {
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
                    int new_value = img(i, j, c) + 50;
                    img(i, j, c) = new_value > 255 ? 255 : new_value;
                }
                // Decrease green channel
                if (c == 1) { // Green channel
                    int new_value = img(i, j, c) - 20;
                    img(i, j, c) = new_value < 0 ? 0 : new_value;
                }
            }
        }
    }
}
void infraredFilter(Image &img) {
    for (int i = 0; i < img.width; i++) {
         for (int j = 0; j < img.height; j++) {
             double a=0;
             a = img(i,j,0)*0.299+img(i,j,1)*0.587+img(i,j,2)*0.114;
             img(i,j,0) = 255;
             img(i,j,1) = 255-a;
             img(i,j,2) = 255-a;

             }
         }
}
void skewImage(Image &img, double skewFactor) {
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
            int newX = i + skewPixels * (j / (double)img.height) + offsetX; // Adjust the x-coordinate based on skew factor and offset
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
void applyFilters(Image &img, const string& outputFilename) {
    string style, color;
    int choice;
    do {
        cout << "1. Invert\n2. Rotate 90\n3. Rotate 180\n4. Rotate 270\n5. Blur\n6. Add Frame\n7. Sunlight\n8. Oil Painting\n9. TV Filter\n10. Night Purple Filter\n11. Infrared Filter\n12. Skew Image\n13. Save and Exit\nEnter your choice: ";
        cin >> choice;
        Image img90(img.height, img.width);
        Image img180(img.width, img.height);
        Image img270(img.height, img.width);
        switch (choice) {
            case 1:
                invertFilter(img);
                cout << "Invert filter applied successfully.\n";
                break;
            case 2:
                rotate90(img, img90);
                cout << "Image rotated 90 degrees successfully.\n";
                copyImage(img, img90); // Update img with rotated image
                break;
            case 3:
                rotate180(img, img180);
                cout << "Image rotated 180 degrees successfully.\n";
                copyImage(img, img180); // Update img with rotated image
                break;
            case 4:
                rotate270(img, img270);
                cout << "Image rotated 270 degrees successfully.\n";
                copyImage(img, img270); // Update img with rotated image
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
                sunlightFilter(img);
                cout << "Sunlight filter applied successfully.\n";
                break;
            case 8:
                oilPaintingFilter(img);
                cout << "Oil painting filter applied successfully.\n";
                break;
            case 9:
                tvFilter(img);
                cout << "TV filter applied successfully.\n";
                break;
            case 10:
                nightPurpleFilter(img);
                cout << "Night Purple filter applied successfully.\n";
                break;
            case 11:
                infraredFilter(img);
                cout << "Infrared filter applied successfully.\n";
                break;
            case 12:
                double skewFactor;
                cout << "Enter skew factor (in degrees): ";
                cin >> skewFactor;
                skewFactor = skewFactor * M_PI / 180.0; // Convert to radians
                skewImage(img, skewFactor);
                cout << "Skew filter applied successfully.\n";
                break;
            case 13:
                img.saveImage(outputFilename);
                cout << "Image saved successfully.\n";
                break;
            default:
                cout << "Invalid choice\n";
                break;
        }
    } while (choice != 13);
}
int main() {
    string inputFilename = "s.png", outputFilename= "tt.png";
    int choice;
    do {
//        cout << "Enter input filename: ";
//        cin >> inputFilename;
//        cout << "Enter output filename: ";
//        cin >> outputFilename;

        Image img(inputFilename);
        applyFilters(img, outputFilename);

        cout << "Do you want to load another image? (1 for yes, 0 for no): ";
        cin >> choice;
    } while (choice == 1);

    cout << "Exiting...\n";

    return 0;
}