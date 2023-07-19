/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"
#include "sourceimage.h"
#include "mosaiccanvas.h"
#include "kdtree.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    //make a mosaiccanvas to store the answer    
    MosaicCanvas* result = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
    std::map<Point<3>, TileImage*> mapThing;
    //make a vector of points to store the average colors
    vector<Point<3>> points;
    //put the average color of each tile into the points vector
    for(size_t k = 0; k < theTiles.size(); k++) {
        LUVAPixel tempL = theTiles.at(k).getAverageColor();
        Point<3> tempPoint(tempL.l, tempL.u, tempL.v);
        points.push_back(tempPoint);
        mapThing[tempPoint] = &theTiles.at(k);
    }
    //KDTree containing the average colors of the tileimages
    KDTree colors(points);

    for(int row = 0; row < theSource.getRows(); row++) {
        for(int col = 0; col < theSource.getColumns(); col++) {
            //get the current color
            LUVAPixel currColor = theSource.getRegionColor(row, col);
            //get the color closest to it
            Point<3> closestPoint = colors.findNearestNeighbor(Point<3>(currColor.l, currColor.u, currColor.v));
            result->setTile(row, col, mapThing[closestPoint]);
        }
    }

    return result;
}

