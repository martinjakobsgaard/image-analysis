#ifndef SEGMENT_HPP
#define SEGMENT_HPP

// OpenCV includes
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

// Standard includes
#include <iostream>
#include <string>
#include <regex>
#include <experimental/filesystem>

class Segment
{
public:
    Segment(std::string path);
    void fetch_test_image_paths(std::string);
    void fetch_background(std::string);
private:
    std::vector<std::string> test_image_paths;
    std::vector<double> test_results;
    std::string test_title = "unspecified_test";
};

#endif // SEGMENT_HPP
