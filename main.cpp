#include <iostream>

// Include OpenCV API
#include <opencv2/opencv.hpp>

using namespace std;

int getBlurValue(cv::Mat input)
{
    //https://stackoverflow.com/questions/24080123/opencv-with-laplacian-formula-to-detect-image-is-blur-or-not-in-ios
    cv::Mat matImageGrey;
    cv::cvtColor(input, matImageGrey, CV_BGRA2GRAY);

    cv::Mat laplacianImage;
    //cv::Laplacian(matImageGrey, laplacianImage, CV_8U);

    //cv::Mat laplacianImage8bit;
    //laplacianImage.convertTo(laplacianImage8bit, CV_8UC1);

    cv::Laplacian(matImageGrey, laplacianImage, CV_64F);
    cv::Scalar mean, stddev; // 0:1st channel, 1:2nd channel and 2:3rd channel
    cv::meanStdDev(laplacianImage, mean, stddev, cv::Mat());
    double variance = stddev.val[0] * stddev.val[0];
    return variance;
}

int main()
{



    return 0;
}
