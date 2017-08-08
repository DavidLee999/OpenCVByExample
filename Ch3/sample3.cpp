#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;

int blurAmount = 15;

static void onChange(int pos, void* userInput);

static void onMouse(int event, int x, int y, int, void* userInput);

int main(int argc, char** argv)
{
	Mat lena = imread("./lena.jpg");

	namedWindow("Lena");

	// createTrackbar("Lena", "Lena", &blurAmount, 30, onChange, &lena);

	// setMouseCallback("Lena", onMouse, &lena);

	// onChange(blurAmount, &lena);

	imshow("Lena", lena);

	// displayOverlay("Lena", "Overlay 5secs", 5000);

	//displayStatusBar("Lena", "Status Bar 5secs", 5000);

	saveWindowParameters("Lena");

	loadWindowParameters("Lena");

	waitKey(0);

	destroyWindow("Lena");

	return 0;

}

static void onChange(int pos, void* userInput)
{
	if (pos <= 0)
		return;

	Mat imgBlur;

	Mat* img = static_cast<Mat*>(userInput);

	blur(*img, imgBlur, Size{pos, pos});

	imshow("Lena", imgBlur);
}

static void onMouse(int event, int x, int y, int, void* userInput)
{
	if (event != EVENT_LBUTTONDOWN)
		return;

	Mat* img = static_cast<Mat*>(userInput);

	circle(*img, Point{ x, y }, 10, Scalar{ 0, 255, 0 }, 3);

	onChange(blurAmount, img);
}
