#include <iostream>
#include <string>
// #include <vector>
#include <sstream>
#include <cmath>
using namespace std;

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
// #include "utils/MultipleImageWindow.h"
using namespace cv;

const char* keys =
{
	"{ help h usage ? | | print this message}"
	"{ @image | | Image to process}"
	"{ @lightPattern | | Image light pattern to apply to image input}"
	"{ lightMethod | 0 | Method to remove background light, 0: difference; 1: div.}"
	"{ segMethod | 1 | Method to segment, 1: connected Components; 2: connected components with stats; 3: find Contours.}"
};

static Scalar randomColor(RNG& rng)
{
	int icolor = (unsigned)rng;
	return Scalar(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255);
}

Mat calculateLightPattern(const Mat& img)
{
	Mat pattern;
	blur(img, pattern, Size{ img.cols / 3, img.cols / 3 });
	return pattern;
}

Mat removeLight(const Mat& img, const Mat& pattern, int method)
{
	Mat aux;

	if (method == 1)
	{
		Mat img32, pattern32;
		img.convertTo(img32, CV_32F);
		pattern.convertTo(pattern32, CV_32F);

		aux = 1 - (img32 / pattern32);
		aux = aux * 255;
		aux.convertTo(aux, CV_8U);
	}
	else
	{
		aux = pattern - img;
	}

	return aux;
}

void ConneectedComponent(const Mat& img)
{
	Mat labels;
	int num_objects = connectedComponents(img, labels);
	
	if (num_objects < 2)
	{
		cout << "No objects detected" << endl;
		return;
	}
	else
		cout << "Number of objects detected: " << num_objects - 1 << endl;

	Mat output = Mat::zeros(img.rows, img.cols, CV_8UC3);
	RNG rng{ 0xFFFFFFFF };
	for (int i = 1; i < num_objects; ++i)
	{
		Mat mask = labels == i;
		output.setTo(randomColor(rng), mask);
	}
	imshow("Result", output);
}

void connectedComponentsStats(const Mat& img)
{
	Mat labels, stats, centroids;
	
	int num_objects = connectedComponentsWithStats(img, labels, stats, centroids);

	if (num_objects < 2)
	{
		cout << "No objects detected." << endl;
		return;
	}
	else
		cout << "Number of objects detected: " << num_objects - 1 << endl;

	Mat output = Mat::zeros(img.rows, img.cols, CV_8UC3);
	RNG rng{ 0xFFFFFFFF };
	for (int i = 1; i < num_objects; ++i)
	{
		cout << centroids.at<double>(i,0) << " " << centroids.at<double>(i,1) << endl;
	}
	for (int i = 1; i < num_objects; ++i)
	{
		Point2d pos{ centroids.at<double>(i,0), centroids.at<double>(i,1) };
		cout << "Object " << i << " with pos: " << pos
			<< " with area " << stats.at<int>(i, CC_STAT_AREA) << endl;
		Mat mask = labels == i;
		output.setTo(randomColor(rng), mask);
		
		stringstream ss;
		ss << "area: " << stats.at<int>(i, CC_STAT_AREA);
		putText(output, ss.str(), pos, FONT_HERSHEY_SIMPLEX, 0.4, Scalar{ 255, 255, 255 });

	}
	imshow("Result", output);
}

void FindContoursBasic(const Mat& img)
{
	vector<vector<Point>> contours;
	Mat img_copy;
	img.copyTo(img_copy);

	findContours(img_copy, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	Mat output = Mat::zeros(img.rows, img.cols, CV_8UC3);
	if (contours.size() == 0)
	{
		cout << "No objects detected" << endl;
		return;
	}
	else
		cout << "number of objects detected: " << contours.size() << endl;

	RNG rng{ 0xFFFFFFFF };
	for (int i = 0; i < contours.size(); i++)
		drawContours(output, contours, i, randomColor(rng));
	
	imshow("Result", output);
}
int main(int argc, const char** argv)
{
	CommandLineParser parser(argc, argv, keys);
	parser.about("Chapter 5. PhotoTool v1.0.0");

	if (parser.has("help"))
	{
		parser.printMessage();
		return 0;
	}

	String img_file = parser.get<String>(0);
	String light_pattern_file = parser.get<String>(1);
	int method_light = parser.get<int>("lightMethod");
	int method_seg = parser.get<int>("segMethod");

	if (!parser.check())
	{
		parser.printErrors();
		return 0;
	}

	cv::Mat img = imread(img_file, 0);
	if (img.data == NULL)
	{
		cerr << "Error loading image " << img_file << endl;
		return 0;
	}

	// MultipleImageWindow*  miw = new MultipleImageWindow("Main Window", 3, 2, WINDOW_AUTOSIZE);

	cv::Mat img_noisy, img_box_smooth;
	medianBlur(img, img_noisy, 3);
	blur(img, img_box_smooth, Size{ 3, 3 });

	cv::Mat light_pattern = imread(light_pattern_file, 0);
	if (light_pattern.data == NULL)
	{
		light_pattern = calculateLightPattern(img_noisy);
	}
	medianBlur(light_pattern, light_pattern, 3);

	cv::Mat img_no_light;
	img_noisy.copyTo(img_no_light);
	img_no_light = removeLight(img_noisy, light_pattern, method_light);

	cv::Mat img_thr;
	if (method_light != 2)
		threshold(img_no_light, img_thr, 30, 255, THRESH_BINARY);
	else
		threshold(img_no_light, img_thr, 140, 255, THRESH_BINARY_INV);

	// miw->addImage("input", img);
	// miw->addImage("input without noisy", img_noisy);
	// miw->addImage("light pattern", light_pattern);

	imshow("input", img);
	imshow("input without noisy", img_noisy);
	imshow("light pattern", light_pattern);
	imshow("no light", img_no_light);
	imshow("threshold", img_thr);

	// miw->addImage("no light", img_no_light);
	// miw->addImage("threshold", img_thr);

	switch (method_seg)
	{
	case 1:
		ConneectedComponent(img_thr);
		break;
	case 2:
		connectedComponentsStats(img_thr);
		break;
	case 3:
		FindContoursBasic(img_thr);
		break;
	default:
		break;
	}

	// miw->render();
	waitKey(0);

	return 0;
}
