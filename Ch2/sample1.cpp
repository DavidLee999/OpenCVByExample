#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;

int main(int argc, const char** argv)
{
    Mat color = imread(R"(.\lena.jpg)");
    Mat gray = imread(R"(.\lena.jpg)", 0);

    imwrite("lenaGray.jpg", gray);

    int myRow = color.cols - 1;
    int myCol = color.rows - 1;
    Vec3b pixel = color.at<Vec3b>(myRow, myCol);

    cout << "Pixel value (B, G, R): ( " << (int)pixel[0] << ", " << (int)pixel[1] << ", " <<
        (int)pixel[2] << " )" << endl;

    imshow("Lena BGR", color);
    imshow("Lena Gray", gray);

    waitKey(0);

    return 0;
}
