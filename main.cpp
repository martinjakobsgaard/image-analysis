#include <iostream>

// Include OpenCV API
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include <iostream>
#include <string>

int getBlurValueLaplacian(cv::Mat input)
{
    //https://stackoverflow.com/questions/24080123/opencv-with-laplacian-formula-to-detect-image-is-blur-or-not-in-ios
    cv::Mat matImageGrey;
    cv::cvtColor(input, matImageGrey, CV_BGR2GRAY);

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

int main( int argc, char** argv )
{
    std::string imageName;
    if( argc > 1)
        imageName = argv[1];

    cv::Mat image;
    image = cv::imread(imageName, cv::IMREAD_COLOR );
    if( image.empty() )
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    std::cout << "Blur: " << getBlurValueLaplacian(image) << std::endl;

    return 0;
}
