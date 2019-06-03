#ifndef TRAINING_H
#define TRAINING_H

#include <QObject>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QTime>
#include <QDebug>
#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class Training
{
public:
    Training();
    Mat readImages(int opt);
    Mat readLabels(int opt);
    void knnTrain();
    void testMnist();
    int reverseDigit(int num);
};

#endif // TRAINING_H
