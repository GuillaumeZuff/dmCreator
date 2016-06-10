#include "datamatrix.h"
#include <dmtx.h>
#include <vector>
#include <iostream>
#include <ctime>
#include <sstream>

using namespace std;

DataMatrix::DataMatrix(): m_filename() {
	srand(time(NULL));
}

DataMatrix::~DataMatrix() {
}

std::string getFilename() {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    stringstream name;
    name << "dm" << timeinfo->tm_hour << timeinfo->tm_min << timeinfo->tm_sec;
    name << "_" <<  rand();
    name << ".jpg";
    return name.str();
}

bool DataMatrix::generate(const std::string &text, dm_data &result) {
    vector<unsigned char> data(text.begin(), text.end());
    DmtxEncode *dm = dmtxEncodeCreate();
    dmtxEncodeDataMatrix(dm, (int)data.size(), &data[0]);

    int width = dmtxImageGetProp(dm->image, DmtxPropWidth);
    int height = dmtxImageGetProp(dm->image, DmtxPropHeight);
    int bytesPerPixel = dmtxImageGetProp(dm->image, DmtxPropBytesPerPixel);

    result.width = width;
    result.height = height;
    result.channels = bytesPerPixel;
    result.pixels = vector<unsigned char>(width*height*bytesPerPixel);

    //cout << "DataMatrix: "<<width<<" "<<height<<" "<<bytesPerPixel<<endl;

    int index=0;
    int val;
    for (int i=0; i<width; i++) {
        for (int j=0; j<height; j++) {
            for (int k=0; k<bytesPerPixel; k++) {
                dmtxImageGetPixelValue(dm->image,j,i,k,&val);
                result.pixels[index] = val;
                index++;
            }
        }
    }

    //cout << "Datamatrix: size: "<<width<<" "<<height<<" "<<bytesPerPixel<<endl;

    return true;
}

bool DataMatrix::decode(const dm_image &image, std::string &decodedText) {
    decodedText = "";

    // 1) create dmtx image structure
    DmtxImage *dmtxImage;
    if (image.channels == 1) {
        dmtxImage = dmtxImageCreate(image.data, image.cols, image.rows, DmtxPack8bppK);
    } else if (image.channels == 3) {
        dmtxImage = dmtxImageCreate(image.data, image.cols, image.rows, DmtxPack24bppRGB);
    } else if (image.channels == 4) {
        dmtxImage = dmtxImageCreate(image.data, image.cols, image.rows, DmtxPack32bppRGBX);
    } else {
        cout << "DataMatrixReader: image format unknown" << endl;
        return false;
    }

    // 2) set dmtx image properties
    if (dmtxImageSetProp(dmtxImage, DmtxPropChannelCount, image.channels) == DmtxFail) {
        cout << "DataMatrixReader: "
            << "can't set image property DmtxPropChannelCount"
            << endl;
        return false;
    }
    if (dmtxImageSetProp(dmtxImage, DmtxPropRowSizeBytes, image.channels) == DmtxFail) {
        cout << "DataMatrixReader: "
            << "can't set image property DmtxPropRowSizeBytes"
            << endl;
        return false;
    }

    // 3) create dmtx decode matrix
    DmtxDecode *dmtxDecode = dmtxDecodeCreate(dmtxImage, 1);

    // 4) optional: set dmtx decode properties

    // 5) search for next region
    bool messageFound = false;
    DmtxTime time = dmtxTimeAdd(dmtxTimeNow(), 1000);
    DmtxRegion *region = dmtxRegionFindNext(dmtxDecode, &time);
    while ((region != NULL) && !messageFound) {
        //cout << "Testing region..." << region << endl;
        DmtxMessage *message = dmtxDecodeMatrixRegion(dmtxDecode, region, DmtxUndefined);
        if (message != NULL) { // message found!
            decodedText = string((char*) message->output);
            messageFound = true;
            dmtxMessageDestroy(&message);
        }
        dmtxRegionDestroy(&region);
        time = dmtxTimeAdd(dmtxTimeNow(), 1000);
        region = dmtxRegionFindNext(dmtxDecode, &time);
    }

    dmtxDecodeDestroy(&dmtxDecode);
    dmtxImageDestroy(&dmtxImage);

    return messageFound;
}

