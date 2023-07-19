#include "StickerSheet.h"
#include "Image.h"
#include "../lib/cs225/HSLAPixel.h"
#include <vector>


//vector names are imagePtrs, xCoords, yCoords

StickerSheet::StickerSheet(const Image & picture, unsigned max) {
    baseImg = picture;
    maxSize = max;
}

StickerSheet::~StickerSheet() {
    _clear();
}

StickerSheet::StickerSheet(const StickerSheet & other) {
    _copy(other);
}

void StickerSheet::_copy(StickerSheet const & other) {
    baseImg = other.baseImg;//these seem good
    maxSize = other.maxSize;
    for(int i = 0; i < int(other.imagePtrs.size()); i++) {
        imagePtrs.push_back(other.imagePtrs.at(i));
        xCoords.push_back(other.xCoords.at(i));
        yCoords.push_back(other.yCoords.at(i));
    }
}

void StickerSheet::_clear() {
    for(int i = 0; i < int(imagePtrs.size()); i++) {
        if(imagePtrs.at(i) != nullptr) {
            imagePtrs.at(i) = nullptr;
        }
    }
    imagePtrs.resize(0);
}

int StickerSheet::addSticker(Image & sticker, unsigned x, unsigned y) {
    for(int i = 0; i < int(imagePtrs.size()); i++) {
        if(imagePtrs.at(i) == nullptr) {
            imagePtrs.at(i) = &sticker;
            xCoords.at(i) = x;
            yCoords.at(i) = y;
            return i;
        }
    }
    if(maxSize == unsigned(imagePtrs.size())) {
        return -1;
    }
    imagePtrs.push_back(&sticker);
    xCoords.push_back(x);
    yCoords.push_back(y);
    return int(imagePtrs.size()) - 1;
}

void StickerSheet::changeMaxStickers(unsigned max) {
    if(max < unsigned(imagePtrs.size())) { 
        for(int i = max; i < int(imagePtrs.size()); i++) {
            if(imagePtrs.at(i) != nullptr) {
                //delete imagePtrs.at(i);
                imagePtrs.at(i) = nullptr;
            }
        }
        imagePtrs.resize(max);
        xCoords.resize(max);
        yCoords.resize(max);
    }
    maxSize = max;
}

Image* StickerSheet::getSticker(unsigned index) {
    if(index >= unsigned(imagePtrs.size())) {
        return NULL;
    }
    return imagePtrs.at(index);
}

const StickerSheet & StickerSheet::operator=(const StickerSheet & other) {
    _clear();
    _copy(other);
    return *this;
}

void StickerSheet::removeSticker(unsigned index) {
    //make the sticker null
    if(index < unsigned(imagePtrs.size()))
    {
        if(imagePtrs.at(index) != nullptr)
        {
            //delete imagePtrs.at(index);
            imagePtrs.at(index) = nullptr;
        }
    }
}

Image StickerSheet::render() const {
    //naive interpretation: draw the base, then draw all stickers on top
    //intricacy: if the sticker would go off of the sheet, expand the sheet
    //intricacy: if any alpha value is 0, just don't draw it
    Image p(baseImg); //stack
    unsigned int w = p.width();
    unsigned int h = p.height();
    //go through every sticker
    for(int i = 0; i < int(imagePtrs.size()); i++) {
        if(imagePtrs.at(i) != nullptr) { //if the sticker exists
            Image tempImg = *imagePtrs.at(i); //make a reference to whatever is in the thing
            unsigned int tempWid = tempImg.width();
            unsigned int tempHeight = tempImg.height();
            if(xCoords.at(i) + tempWid >= w) { //if the image would go out of bounds
                p.resize(xCoords.at(i) + tempWid, h); //increase the size
                w = xCoords.at(i) + tempWid;
            }
            if(yCoords.at(i) + tempHeight >= h) {
                p.resize(w, yCoords.at(i) + tempHeight);
                h = yCoords.at(i) + tempHeight;
            }

            //now we put the sticker on top
            for(unsigned int offsetX = 0; offsetX < tempWid; offsetX++) {
                for(unsigned int offsetY = 0; offsetY < tempHeight; offsetY++) {
                    //get the pixel
                    HSLAPixel tempPixel = tempImg.getPixel(offsetX, offsetY);
                    //check to see if the alpha is 0
                    if(tempPixel.a != 0) {
                        //place this pixel in the location it should be
                        p.getPixel(offsetX + xCoords.at(i), offsetY + yCoords.at(i)) = tempPixel;
                    }
                }
            }
        }
    }
    return p;
}

bool StickerSheet::translate(unsigned index, unsigned x, unsigned y) {
    if(index < unsigned(imagePtrs.size())) {
        if(imagePtrs.at(index) != nullptr) {
            xCoords.at(index) = x;
            yCoords.at(index) = y;
            return true;
        }
    }
    return false;
}