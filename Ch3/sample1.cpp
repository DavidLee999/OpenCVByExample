#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;

// const int CV_GUI_NORMAL = 0x10; 

int main(int argc, const char** argv)
{
	Mat lena = imread("./lena.jpg");
	Mat photo = imread("./photo.jpg");

	namedWindow("Lena", CV_GUI_NORMAL);
	namedWindow("Photo", WINDOW_AUTOSIZE);

	moveWindow("Lena", 10, 10);
	moveWindow("Photo", 520, 10);

	imshow("Lena", lena);
	imshow("Photo", photo);

	resizeWindow("Lena", 512, 512);

	waitKey(0);

	destroyWindow("Lena");
	destroyWindow("Photo");

	for (int i = 0; i < 10; i++)
	{
		ostringstream ss;
		ss << "Photo" << i;
		namedWindow(ss.str());
		moveWindow(ss.str(), 20 * i, 20 * i);
		imshow(ss.str(), photo);
	}

	waitKey(0);
	destroyAllWindows();
	return 0;
}