#include <iostream>
#include "cmake-build-debug/Image_Class.h"
using namespace std;

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
void filter_2(Image& Img){
    int z=0;
    for (int i=0 ; i<Img.width;i++){
        for(int j =0; j <Img.height;j++){
            unsigned int a=0;
            for (int k=0; k<3;k++){
                a+=Img(i,j,k);
            }
            if (a/3 < 129){
                z=0;
            }else z=255;
            for (int s=0; s < 3;s++ ){
                Img(i,j,s)=z;
            }
        }
    }
    Img.saveImage("black_white1.png");
    cout << "end";

}
void filter_5_1(Image& Img){
    int z=0;
    int count = Img.width-1;
    for (int i=0 ; i<Img.width/2;i++){
        for(int j =0; j <Img.height;j++){
            unsigned int a=0;
            for (int s=0; s < 3;s++ ){
                z=Img(count-i,j,s);
                Img(count-i,j,s)=Img(i,j,s);
                Img(i,j,s) =z;
            }
        }
    }
    Img.saveImage("flip.png");
    cout << "end";
}
void filter_5_2(Image& Img){
    int z=0 ;
    int count = Img.height-1;
    for (int i=0 ; i<Img.width;i++){
        for(int j =0; j <Img.height/2;j++){
            unsigned int a=0;
            for (int s=0; s < 3;s++ ){
                z=Img(i,count-j,s);
                Img(i,count-j,s)=Img(i,j,s);
                Img(i,j,s) =z;
            }
        }
    }
    Img.saveImage("flip.png");
    cout << "end";
}
void filter_8(Image& Img){
    int x , y , w , h ;
    cin >> x >> y >> w >> h;
    Image Img2(w,h);
    for (int i=0;i<w;i++){
        for (int j = 0; j <h ;j++) {
            for(int k=0; k<3;k++){

                Img2(i,j,k)=Img(i+x,j+y,k);
            }
        }
    }
    Img2.saveImage("crop.png");
    cout << "end";
}
void filter_11(Image& Img){
    int x_new ;
    int y_new ;
    cin >> x_new >> y_new;
    int wid=Img.width;
    int heig=Img.height;
    double z= (double)wid/x_new;
    double z1 = (double)heig/y_new;
    Image Imag(x_new,y_new);
    for (int i=0;i<x_new;i++){
        for (int j = 0; j <y_new ;j++) {
            int x1 = i*z;
            int y1 = j*z1;
            for(int k=0; k<3;k++){
                Imag(i,j,k)=Img(x1,y1,k);
            }
        }
    }
    Imag.saveImage("resizing.png");
}

void filter_IR(Image& img){
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            double a=0;
            a = img(i,j,0)*0.299+img(i,j,1)*0.587+img(i,j,2)*0.114;
            img(i,j,0) = 255;
            img(i,j,1) = 255-a;
            img(i,j,2) = 255-a;

        }
    }
    img.saveImage("gray.png");
}

int main(){
    return 0;

}






