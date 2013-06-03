#include "datamatrix.h"
#include <dmtx.h>
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <sstream>

using namespace std;
using namespace cv;

DataMatrix::DataMatrix(): m_path(), m_filename(), m_data() {
	srand(time(NULL));
}

DataMatrix::~DataMatrix() {
}

void DataMatrix::setPath(const std::string &path) {
	m_path = path;
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
	name << ".png";
	return name.str();
}

std::string DataMatrix::getPath(const std::string &filename) const {
    return m_path+filename;
}

bool DataMatrix::generate() {
	vector<unsigned char> data(m_data.begin(), m_data.end());
	DmtxEncode *dm = dmtxEncodeCreate();
	dmtxEncodeDataMatrix(dm, (int)data.size(), &data[0]);

	int width = dmtxImageGetProp(dm->image, DmtxPropWidth);
	int height = dmtxImageGetProp(dm->image, DmtxPropHeight);
	int bytesPerPixel = dmtxImageGetProp(dm->image, DmtxPropBytesPerPixel);

    //cout << "DataMatrix: "<<width<<" "<<height<<" "<<bytesPerPixel<<endl;

	Mat img(width, height, CV_8UC3);

	int val;
	Mat_<Vec3b>& ptr = (Mat_<Vec3b>&)img;
	for (int i=0; i<width; i++) {
		for (int j=0; j<height; j++) {
			for (int k=0; k<bytesPerPixel; k++) {
				dmtxImageGetPixelValue(dm->image,j,i,k,&val);
				ptr(height-i-1,j)[k] = val;
			}
		}
	}

	m_filename = getFilename();
    string path = getPath(m_filename);

    //cout << "DataMatrix: "<<"writing to "<<path<<endl;

	return imwrite(path, img);
}

std::string DataMatrix::pathToFile() const {
    return getPath(m_filename);
}
