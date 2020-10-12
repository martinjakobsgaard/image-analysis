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

int getBlurValueVert(cv::Mat input)
{
    int blurVal = 0;

    cv::Mat matImageGrey;
    cv::cvtColor(input, matImageGrey, CV_BGR2GRAY);

    for(int i = 1; i < matImageGrey.size().height-1; i++)
    {
        for(int j = 0; j < matImageGrey.size().width; j++)
        {
            int pixelAbove = matImageGrey.at<uchar>(i-1,j);
            int pixel = matImageGrey.at<uchar>(i,j);
            int pixelBelow = matImageGrey.at<uchar>(i+1,j);

            blurVal += (pixelAbove*1) + (pixel*-2) + (pixelBelow*1);
        }
    }

    return blurVal;
}

int getBlurValueHor(cv::Mat input)
{
    int blurVal = 0;

    cv::Mat matImageGrey;
    cv::cvtColor(input, matImageGrey, CV_BGR2GRAY);

    for(int j = 1; j < matImageGrey.size().width-1; j++)
    {
        for(int i = 0; i < matImageGrey.size().height; i++)
        {
            int pixelLeft = matImageGrey.at<uchar>(i,j-1);
            int pixel = matImageGrey.at<uchar>(i,j);
            int pixelRight = matImageGrey.at<uchar>(i,j+1);

            blurVal += (pixelLeft*1) + (pixel*-2) + (pixelRight*1);
        }
    }

    return blurVal;
}

void convertHistogramEqualizer(cv::Mat input)
{
    cv::cvtColor(input, input, cv::COLOR_BGR2GRAY );
    cv::Mat dst;
    cv::equalizeHist(input, dst);
    cv::imshow( "Equalized Image", dst);
    cv::waitKey();
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

    //std::cout << "Blur vertical: " << getBlurValueVert(image) << " Blur Horizontal: " << getBlurValueHor(image) << std::endl;
    convertHistogramEqualizer((image));

    return 0;
}
