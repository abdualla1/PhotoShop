    #include <bits/stdc++.h>
    #include "Image_Class.h"

    using namespace std;
    #define endl '\n'
    #define ll long long
    void Resize(Image &Img, int x_new, int y_new);
    void copyImage(Image &img, const Image &other) {
        if (&img != &other) { // protect against invalid self-assignment
            // 1: deallocate old memory
            stbi_image_free(img.imageData);
            img.width = other.width;
            img.height = other.height;
            img.channels = other.channels;
            img.imageData =static_cast<unsigned char *>(malloc(img.width * img.height * img.channels));
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
        Resize(img1, newWidth, newHeight);
        Resize(img2, newWidth, newHeight);
        Image newImg(newWidth, newHeight);
        for (int i = 0; i < newWidth; i++) {
            for (int j = 0; j < newHeight; j++) {
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
        if (color == "red") {
            r = 255;
        } else if (color == "green") {
            g = 255;
        } else if (color == "blue") {
            b = 255;
        } else if (color == "white") {
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
                             {-4, 0, 4},
                             {-1, 0, 1}};
        int kernely[3][3] = {{1,  4,  1},
                             {0,  0,  0},
                             {-1, -4, -1}};
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
                img(i, j, 1) = min(255, img(i, j, 1) + 28); // Green
                img(i, j, 2) = max(0, img(i, j, 2) - 30); // Blue
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

    void oilPainting(Image &img) {
        float radius = 2;
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
                int noise = rand() % 50 - 40;
                for (int c = 0; c < img.channels; c++) {// Generate a random number between -20 and 20
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

    void skewImage(Image &img, double angle) {
        double skewFactor =
                tan(angle * M_PI / 180.0) / 1.655; // Convert angle to radians, calculate skew factor and reduce it
        int newWidth = img.width + abs(int(img.height * skewFactor)); // Calculate new width
        Image newImg(newWidth, img.height); // Create new image with new dimensions
        for (int i = 0; i < img.width; i++) {
            for (int j = 0; j < img.height; j++) {
                int newI = i + skewFactor * (img.height - j); // Skew in the clockwise direction
                if (newI >= 0 && newI < newWidth) { // Check if newI is within new image width
                    for (int c = 0; c < img.channels; c++) {
                        newImg(newI, j, c) = img(i, j, c);
                    }
                }
            }
        }
        copyImage(img, newImg); // Copy new image to original image
    }
    string toLower(string str) {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }
    void applyFilters(Image &img, string outputFilename) {
        int choice;

        do {
            cout
                    << "1. Grey Scale\n2. Black and White\n3. Invert Filter\n4. Merge Images\n5. Flip Horizontal\n6. Flip Vertical\n7. Rotate 90\n8. Rotate 180\n9. Rotate 270\n10. Darken\n11. Brighten\n12. Crop\n13. Add Frame\n14. Edge Detection\n15. Resize\n16. Blur Filter\n17. Sunlight Filter\n18. Oil Painting Filter\n19. TV Filter\n20. Night Purple Filter\n21. Infrared Filter\n22. Skew Image\n23. Save and Exit\nEnter your choice: ";
            cin >> choice;
            if(cin.fail()){
                cout << "Invalid input. Please enter an integer." << endl;
                cin.clear(); // Clear error flags
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore remaining input
                continue; // Restart the loop
            }
            int x, y, w, h;
            int versions;
            int x_new, y_new;
            string style, color;
            string secondImageFilename;
            Image secondImg;
            Image img90(img.height, img.width);
            Image img180(img.width, img.height);
            Image img270(img.height, img.width);
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
                    while (true) {
                        cout << "Enter second image filename: ";
                        cin >> secondImageFilename;
                        ifstream file(secondImageFilename);
                        if (!file.good()) {
                            cout << "Error: Invalid filename. File does not exist." << endl;
                            cin.clear(); // Clear error flags
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        } else {
                            break; // Exit the loop if a valid filename is provided
                        }
                    }
                    secondImg.loadNewImage(secondImageFilename);
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
                    do {
                        cout << "Enter x, y, width, height: ";
                        if (!(cin >> x >> y >> w >> h)) {
                            cout << "Invalid input. Please enter integers only." << endl;
                            cin.clear(); // Clear error flags
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore remaining input
                            continue; // Restart the loop
                        }
                        if (x < 0 || y < 0 || w <= 0 || h <= 0 || x + w > img.width || y + h > img.height) {
                            cout << "Invalid crop dimensions. Please enter valid values." << endl;
                        } else {
                            Crop(img, x, y, w, h);
                            break; // Exit the loop if valid input is provided
                        }
                    } while (true);
                    break;
                case 13:
                    do {
                        cout << "Enter frame style (simple or fancy): ";
                        cin >> style;
                        style = toLower(style); // Convert input to lowercase for case-insensitivity
                    } while (style != "simple" && style != "fancy");

                    do {
                        cout << "Enter frame color (RED, GREEN, BLUE, WHITE): ";
                        cin >> color;
                        color = toLower(color); // Convert input to lowercase for case-insensitivity
                    } while (color != "red" && color != "green" && color != "blue" && color != "white");
                     addFrame(img, style, color);
                    break;
                case 14:
                    edgeDetection(img);
                    break;
                case 15:
                    do {
                        cout << "Enter new width and height: ";
                        if (!(cin >> x_new >> y_new)) {
                            cout << "Invalid input. Please enter integers only." << endl;
                            cin.clear(); // Clear error flags
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore remaining input
                            continue; // Restart the loop
                        }
                        if (x_new <= 0 || y_new <= 0) {
                            cout << "Invalid dimensions. Width and height must be greater than zero." << endl;
                        } else {
                            Resize(img, x_new, y_new);
                            break; // Exit the loop if valid input is provided
                        }
                    } while (true);
                    break;
                case 16:
                    int blurLevel;
                    bool validBlurLevel;
                    do {
                        // Prompt the user to enter the blur level
                        cout << "Enter blur level (max 10): ";
                        cin >> blurLevel;

                        // Check if the blur level input is valid
                        if (cin.fail() || blurLevel < 0 || blurLevel > 10) {
                            cout << "Invalid input for blur level. Please enter a number between 0 and 10." << endl;
                            cin.clear(); // Clear error flags
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore remaining input
                            validBlurLevel = false;
                        } else {
                            validBlurLevel = true;
                        }
                    } while (!validBlurLevel);
                    break;
                case 17:
                    sunlightFilter(img);
                    break;
                case 18:
                    do {
                        cout << "Enter your version (1 or 2): ";
                        cin >> versions;
                        if (cin.fail()) {
                            cout << "Invalid input. Please enter an integer." << endl;
                            cin.clear(); // Clear error flags
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore remaining input
                        } else if (versions != 1 && versions != 2) {
                            cout << "Invalid version. Please enter 1 or 2." << endl;
                        }
                    } while (versions != 1 && versions != 2);
                    if (versions == 1) {
                        oilPaintingFilter(img);
                    } else {
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
                    do {
                        cout << "Enter skew factor (Max 360): ";
                        if (!(cin >> skewFactor)) {
                            cout << "Invalid input. Please enter a numeric value." << endl;
                            cin.clear(); // Clear error flags
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                            continue; // Restart the loop
                        }
                        if (skewFactor <= 0 || skewFactor > 360) {
                            cout << "Invalid skew factor. Please enter a value between 0 and 360." << endl;
                        }
                    } while (skewFactor <= 0 || skewFactor > 360);
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
        int choice;
        do {
            try {
                string inputFilename, outputFilename;
                cout << "Enter input filename: ";
                cin >> inputFilename;
                cout << "Enter output filename: ";
                cin >> outputFilename;

                ifstream file(inputFilename);
                ifstream file2(outputFilename);
                if (!file.good() || !file2.good()) {
                    throw runtime_error("Invalid filename, File Does not Exist");
                }

                Image img(inputFilename);
                applyFilters(img, outputFilename);

                cout << "Do you want to load another image? (1 for yes, 0 for no): ";
                cin >> choice;
                if (choice == 1) {
                    img.saveImage(outputFilename);
                }
            } catch (const exception &e) {
                cout << "Please enter valid filenames." << endl;
                choice = 1; // Continue loop for re-entering filenames
                cin.clear(); // Clear error flags
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore remaining input
            }
        } while (choice == 1);


        cout << "Exiting...\n";
        return 0;
    }
