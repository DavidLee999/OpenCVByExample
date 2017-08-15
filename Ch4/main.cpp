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
	"{ @operation | | Operation }"
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
	{
		showHisto(img);
	}
    waitKey(0);

    return 0;
}
