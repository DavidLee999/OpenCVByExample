#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;

int main(int argc, char* argv[])
{
	cv::Mat img = cv::imread("./lena.jpg");
	namedWindow("Lena");
	imshow("Lena", img);

	cv::Mat imgGray;
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	namedWindow("Gray");
	imshow("Gray", imgGray);

	cv::Mat imgBlur;
	blur(img, imgBlur, Size{ 5, 5 });
	namedWindow("Blur");
	imshow("Blur", imgBlur);

	cv::Mat imgEdge;
	Sobel(img, imgEdge, CV_8U, 1, 1);
	namedWindow("Sobel");
	imshow("Sobel", imgEdge);

	waitKey(0);
	destroyAllWindows();

	return 0;
}