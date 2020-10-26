#include <iostream>

// Include OpenCV API
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

// Include standard
#include <iostream>
#include <string>
#include <regex>

#include "blur.hpp"
#include "segmentation.hpp"

int main( int argc, char** argv )
{
    std::cout << "Started image analysis!" << std::endl;

    if( argc <= 1)
        return -1;

    exportBlurData(argc, argv, "test_data.csv");

    return 0;
}
