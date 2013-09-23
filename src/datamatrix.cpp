#include "datamatrix.h"
#include <dmtx.h>
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <sstream>

using namespace std;
using namespace cv;

DataMatrix::DataMatrix(): m_filename(), m_data() {
	srand(time(NULL));
}

DataMatrix::~DataMatrix() {
}

void DataMatrix::setData(const std::string &data) {
	m_data = data;
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

bool DataMatrix::generate(dm_data &result) {
    vector<unsigned char> data(m_data.begin(), m_data.end());
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

    Mat img(width, height, CV_8UC3);

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

bool DataMatrix::decode(const std::string &filePath, std::string &decodedText) {
    decodedText = "blah";
    cv::Mat frame = imread(filePath);

    cout<<"Decoding..."<<" from: "<<filePath<<endl;

    // 1) create dmtx image structure
    DmtxImage *dmtxImage;
    if (frame.channels() == 1) {
        dmtxImage = dmtxImageCreate((unsigned char*) frame.data, frame.cols, frame.rows, DmtxPack8bppK);
    } else if (frame.channels() == 3) {
        dmtxImage = dmtxImageCreate((unsigned char*) frame.data, frame.cols, frame.rows, DmtxPack24bppRGB);
    } else {
        cout << "DataMatrixReader: image format unknown" << endl;
        return false;
    }

    // 2) set dmtx image properties
    if (dmtxImageSetProp(dmtxImage, DmtxPropChannelCount,
                    frame.channels()) == DmtxFail) {
        cout << "DataMatrixReader: "
            << "can't set image property DmtxPropChannelCount"
            << endl;
        return false;
    }
    if (dmtxImageSetProp(dmtxImage, DmtxPropRowSizeBytes,
                    frame.step) == DmtxFail) {
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
    DmtxTime time = dmtxTimeAdd(dmtxTimeNow(), 400);
    DmtxRegion *region = dmtxRegionFindNext(dmtxDecode, &time);
    if (region != NULL) {
        DmtxMessage *message = dmtxDecodeMatrixRegion(dmtxDecode, region, DmtxUndefined);
        if (message != NULL) { // message found!
            decodedText = string((char*) message->output);
            messageFound = true;
            dmtxMessageDestroy(&message);
        }
        dmtxRegionDestroy(&region);
    }

    dmtxDecodeDestroy(&dmtxDecode);
    dmtxImageDestroy(&dmtxImage);

    return messageFound;
}
