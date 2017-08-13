#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "gl/glut.h"
using namespace cv;

Mat frame;
GLfloat angle = 0.0;
GLuint texture;
VideoCapture camera;

int loadTexture()
{
    if (frame.data == NULL)
        return -1;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, frame.data);

    return 0;
}

void on_opengl(void* param)
{
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, texture);

    glRotatef(angle, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);

    glTexCoord2d(0.0, 0.0);
    glVertex2d(-5.0, -5.0);

    glTexCoord2d(1.0, 0.0);
    glVertex2d(+5.0, -5.0);

    glTexCoord2d(1.0, 1.0);
    glVertex2d(+5.0, +5.0);

    glTexCoord2d(0.0, 1.0);
    glVertex2d(-5.0, +5.0);

    glEnd();
}

int main(int argc, const char** argv)
{
    camera.open(0);
    if (!camera.isOpened())
        return -1;

    namedWindow("OpenGL Camera", WINDOW_OPENGL);

    glEnable(GL_TEXTURE_2D);

    setOpenGlDrawCallback("OpenGL Camera", on_opengl);

    while (waitKey(30) != 'q') {
        camera >> frame;
        loadTexture();
        updateWindow("OpenGL Camera");
        angle += 4;
    }

    destroyWindow("OpenGL Camera");
    return 0;
}
