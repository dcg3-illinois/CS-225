/**
 * @file StickerSheet.h
 * Contains your declaration of the interface for the StickerSheet class.
 */
#pragma once
#include <vector>
#include "Image.h"
//needs an array of POINTERS to images
//each image has index, x coord, y coord
//
using namespace std;

class StickerSheet {
    public:
        StickerSheet(const Image & picture, unsigned max);
        ~StickerSheet();
        StickerSheet(const StickerSheet & other);
        int addSticker(Image & sticker, unsigned x, unsigned y);
        void changeMaxStickers(unsigned max);
        Image* getSticker(unsigned index);
        const StickerSheet & operator=(const StickerSheet & other);
        void removeSticker(unsigned index);
        Image render() const;
        bool translate(unsigned index, unsigned x, unsigned y);
        void _copy(StickerSheet const & other);
        void _clear();
        vector<Image*> imagePtrs;
        vector<unsigned int> xCoords;
        vector<unsigned int> yCoords;
        Image baseImg;
        unsigned int maxSize;

};