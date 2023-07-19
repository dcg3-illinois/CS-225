#include "Image.h"
#include <cstdlib>
#include <cmath>

using namespace std;
using namespace cs225;

void Image::darken() {
    unsigned int w = width();
    unsigned int h = height();

    //loop through every pixel
    for(unsigned wid = 0; wid < w; wid++){
        for(unsigned hig = 0; hig < h; hig++) {
            //reduce luminescence
            getPixel(wid,hig).l -= 0.1;
            if(getPixel(wid,hig).l < 0) {
                getPixel(wid,hig).l = 0;
            }
        }
    }
}

void Image::darken(double amount) {
    unsigned int w = width();
    unsigned int h = height();

    //loop through every pixel
    for(unsigned wid = 0; wid < w; wid++){
        for(unsigned hig = 0; hig < h; hig++) {
            //reduce luminescence
            getPixel(wid,hig).l -= amount;
            if(getPixel(wid,hig).l < 0) {
                getPixel(wid,hig).l = 0;
            }
        }
    }
}

void Image::desaturate() {
    unsigned int w = width();
    unsigned int h = height();

    //loop through every pixel
    for(unsigned wid = 0; wid < w; wid++){
        for(unsigned hig = 0; hig < h; hig++) {
            //reduce saturation
            getPixel(wid,hig).s -= 0.1;
            if(getPixel(wid,hig).s < 0) {
                getPixel(wid,hig).s = 0;
            }
        }
    }
}

void Image::desaturate(double amount) {
    unsigned int w = width();
    unsigned int h = height();

    //loop through every pixel
    for(unsigned wid = 0; wid < w; w++){
        for(unsigned hig = 0; hig < h; h++) {
            //reduce saturation
            getPixel(wid,hig).s -= amount;
            if(getPixel(wid,hig).s < 0) {
                getPixel(wid,hig).s = 0;
            }
        }
    }
}

void Image::grayscale() {
    unsigned int w = width();
    unsigned int h = height();

    //loop through every pixel
    for(unsigned wid = 0; wid < w; wid++){
        for(unsigned hig = 0; hig < h; hig++) {
            //set saturation to 0
            getPixel(wid,hig).s = 0;
        }
    }
}

void Image::illinify() {
    unsigned int w = width();
    unsigned int h = height();

    //loop through every pixel
    for(unsigned wid = 0; wid < w; wid++){
        for(unsigned hig = 0; hig < h; hig++) {
            //increase luminescence
            int d_from_blue = std::min(abs(216 - getPixel(wid,hig).h), 360 - abs(216 - getPixel(wid,hig).h));
            int d_from_orange = std::min(abs(11 - getPixel(wid, hig).h), 360 - abs(11 - getPixel(wid, hig).h));

            if(d_from_blue < d_from_orange) {
                getPixel(wid,hig).h = 216;
            }
            else {
                getPixel(wid,hig).h = 11;
            }
        }
    }
}

void Image::lighten() {
    unsigned int w = width();
    unsigned int h = height();

    //loop through every pixel
    for(unsigned wid = 0; wid < w; wid++){
        for(unsigned hig = 0; hig < h; hig++) {
            //increase luminescence
            getPixel(wid,hig).l += 0.1;
            if(getPixel(wid,hig).l > 1) {
                getPixel(wid,hig).l = 1;
            }
        }
    }
}

void Image::lighten(double amount) {
    unsigned int w = width();
    unsigned int h = height();

    //loop through every pixel
    for(unsigned wid = 0; wid < w; wid++){
        for(unsigned hig = 0; hig < h; hig++) {
            //increase luminescence
            getPixel(wid,hig).l += amount;
            if(getPixel(wid,hig).l > 1) {
                getPixel(wid,hig).l = 1;
            }

        }
    }
}

void Image::rotateColor(double degrees) {
    unsigned int w = width();
    unsigned int h = height();

    //loop through every pixel
    for(unsigned wid = 0; wid < w; wid++){
        for(unsigned hig = 0; hig < h; hig++) {
            //increase saturation
            double newH = std::fmod((getPixel(wid,hig).h + degrees),360);
            if(newH < 0) {
                newH += 360;
            }
            getPixel(wid,hig).h = newH;
        }
    }
}

void Image::saturate() {
    unsigned int w = width();
    unsigned int h = height();

    //loop through every pixel
    for(unsigned wid = 0; wid < w; wid++){
        for(unsigned hig = 0; hig < h; hig++) {
            //increase saturation
            getPixel(wid,hig).s += 0.1;
            if(getPixel(wid,hig).s > 1) {
                getPixel(wid,hig).s = 1;
            }
        }
    }
}

void Image::saturate(double amount) {
    unsigned int w = width();
    unsigned int h = height();

    //loop through every pixel
    for(unsigned wid = 0; wid < w; wid++){
        for(unsigned hig = 0; hig < h; hig++) {
            //increase saturation
            getPixel(wid,hig).s += amount;
            if(getPixel(wid,hig).s > 1) {
                getPixel(wid,hig).s = 1;
            }
        }
    }
}

void Image::scale(double factor) {
    unsigned int newWidth = width() * factor;
    unsigned int newHeight = height() * factor;
    //create an array to represent the new size of the image
    HSLAPixel * newImageData = new HSLAPixel[newWidth * newHeight];
    for(unsigned int w = 0; w < newWidth; w++) {
        for(unsigned int h = 0; h < newHeight; h++) {
            newImageData[h*newWidth + w] = getPixel(int(double(w)/factor), int(double(h)/factor));
        }
    }
    resize(newWidth, newHeight);

    for(unsigned int w = 0; w < newWidth; w++) {
        for(unsigned int h = 0; h < newHeight; h++) {
            HSLAPixel & currPixel = getPixel(w,h);
            currPixel = newImageData[newWidth*h + w];
        }
    }

    delete[] newImageData;
}

void Image::scale(unsigned w, unsigned h) {
    //first step: determine if it's getting bigger or smaller
    unsigned int wid = width();
    unsigned int heig = height();
    float fracW = float(w)/float(wid);
    float fracH = float(h)/float(heig);
    float s = (fracW < fracH) ? fracW : fracH;

    scale(s);
}
