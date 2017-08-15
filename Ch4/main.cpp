#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;

const char* keys =
{
	"{ help h usage ? | | print this message }"
	"{ @image | | Image to process }"
	"{ @operation | | Operation: hist, equal, lomo, cart }"
};

void showHisto(const cv::Mat& img)
{
    vector<Mat> bgr;
    split(img, bgr);

    int numbins = 256;

    float range[] = {0, 256};
    const float* histRange = { range };

    Mat b_hist, g_hist, r_hist;
	int channels[] = { 0, 1 };
    calcHist(&bgr[0], 1, 0, Mat(), b_hist, 1, &numbins, &histRange);
    calcHist(&bgr[1], 1, 0, Mat(), g_hist, 1, &numbins, &histRange);
    calcHist(&bgr[2], 1, 0, Mat(), r_hist, 1, &numbins, &histRange);

    int width = 512;
    int height = 300;

    Mat histImage(height, width, CV_8UC3, Scalar{20, 20, 20});

    normalize(b_hist, b_hist, 0, height, NORM_MINMAX);
    normalize(g_hist, g_hist, 0, height, NORM_MINMAX);
    normalize(r_hist, r_hist, 0, height, NORM_MINMAX);

    int binStep = cvRound((float)width / (float)numbins);
    for (int i = 1; i < numbins; i++)
    {
        line(histImage,
			Point(binStep * (i - 1), height - cvRound(b_hist.at<float>(i - 1))),
                Point(binStep * i, height - cvRound(b_hist.at<float>(i))),
                Scalar(255, 0, 0));
        line(histImage,
                Point(binStep * (i - 1), height - cvRound(g_hist.at<float>(i-1))),
                Point(binStep * i, height - cvRound(g_hist.at<float>(i))),
                Scalar(0, 255, 0));
        line(histImage,
                Point(binStep * (i - 1), height - cvRound(r_hist.at<float>(i-1))),
                Point(binStep * i, height - cvRound(r_hist.at<float>(i))),
                Scalar(0, 0, 255));
    }
	imshow("Histogram", histImage);
}

void equalizeCallback(const cv::Mat& img)
{
	Mat result;

	Mat ycrcb;
	cvtColor(img, ycrcb, COLOR_BGR2YCrCb);

	vector<Mat> channels;
	split(ycrcb, channels);

	equalizeHist(channels[0], channels[0]);
	merge(channels, ycrcb);

	cvtColor(ycrcb, result, COLOR_YCrCb2BGR);

	imshow("Equalized", result);
}

void lomoCallback(const cv::Mat& img)
{
	Mat result;

	const double exponential_e = std::exp(1.0);

	Mat lut(1, 256, CV_8UC1);
	for (int i = 0; i < 256; ++i)
	{
		float x = (float)i / 256.0;
		lut.at<uchar>(i) = cvRound(256 * (1 / (1 + pow(exponential_e, -((x - 0.5) / 0.1)))));
	}

	vector<Mat> bgr;
	split(img, bgr);
	LUT(bgr[2], lut, bgr[2]);

	merge(bgr, result);

	Mat halo{ img.rows, img.cols, CV_32FC3, Scalar{ 0.3, 0.3, 0.3 } };

	circle(halo, Point{ img.cols / 2, img.rows / 2 }, img.cols / 3, Scalar{ 1, 1, 1 }, -1);
	blur(halo, halo, Size{ img.cols / 3, img.cols / 3 });

	Mat resultf;
	result.convertTo(resultf, CV_32FC3);

	multiply(resultf, halo, resultf);
	resultf.convertTo(result, CV_8UC3);

	imshow("Lomograpy", result);
}

void cartoonCallback(const cv::Mat& img)
{
	Mat imgMedian;
	medianBlur(img, imgMedian, 7);

	Mat imgCanny;
	Canny(imgMedian, imgCanny, 50, 150);

	Mat kernel = getStructuringElement(MORPH_RECT, Size{ 2, 2 });
	dilate(imgCanny, imgCanny, kernel);

	imgCanny = imgCanny / 255;
	imgCanny = 1 - imgCanny;

	Mat imgCannyf;
	imgCanny.convertTo(imgCannyf, CV_32FC3);

	blur(imgCannyf, imgCannyf, Size{ 5, 5 });

	Mat imgBF;
	bilateralFilter(img, imgBF, 9, 150.0, 150.0);

	Mat result = imgBF / 25;
	result *= 25;

	Mat imgCanny3c;
	Mat cannyChannels[] = { imgCannyf, imgCannyf, imgCannyf };
	merge(cannyChannels, 3, imgCanny3c);

	Mat resultf;
	result.convertTo(resultf, CV_32FC3);

	multiply(resultf, imgCanny3c, resultf);

	resultf.convertTo(result, CV_8UC3);

	imshow("Cartoon", result);
}

int main(int argc, const char** argv)
{
    CommandLineParser parser(argc, argv, keys);
    parser.about("Chapter 4. PhotoTool v1.0.0");

    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    String imgFile = parser.get<String>(0);
	String op = parser.get<String>(1);
	// string op = "hist";
    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }

    cv::Mat img = imread(imgFile);
    namedWindow("Input");

    // createButton("Show histogram", showHistoCallBack, NULL, QT_PUSH_BUTTON, 0);
    // createButton("Equalize histogram",equalizeCallBack, NULL, QT_PUSH_BUTTON, 0);
    // createButton("Lomography effect", lomoCallBack, NULL, QT_PUSH_BUTTON, 0);
    // createButton("Cartonize effect", cartoonCallBack, NULL, QT_PUSH_BUTTON, 0);

    imshow("Input", img);

	if (op == "hist")
		showHisto(img);
	else if (op == "equal")
		equalizeCallback(img);
	else if (op == "lomo")
		lomoCallback(img);
	else if (op == "cart")
		cartoonCallback(img);

    waitKey(0);

    return 0;
}
