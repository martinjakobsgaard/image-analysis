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
    Segment(std::string path)
    {
        test_title = "segmentation";
        fetch_test_image_paths(path);
        fetch_background(path);
    }

    void fetch_test_image_paths(std::string path)
    {
        std::cout << "Scanning dataset path \"" << path << "\"..." << std::endl;

        // Generate vector of paths to supported files
        test_image_paths.clear();
        std::vector<std::string> supported_file_extensions = {"pcd"};
        for (const auto & entry : std::experimental::filesystem::directory_iterator(path))
        {
            std::string path_string = entry.path();
            std::string file_extension = path_string.substr(path_string.find_last_of(".") + 1);

            for(const auto & supported_file_extension : supported_file_extensions)
            {
                if(file_extension == supported_file_extension)
                {
                    test_image_paths.push_back(path_string);
                    break;
                }
            }
        }
        std::cout << "Found " << test_image_paths.size() << " pointcloud(s)!" << std::endl;
    }
    void fetch_background(std::string path)
    {
        std::cout << "Scanning dataset path \"" << path << "\"..." << std::endl;
        std::cout << "This function is yet to be implemented!" << std::endl;
    }
private:
    std::vector<std::string> test_image_paths;
    std::vector<double> test_results;
    std::string test_title = "unspecified_test";
};

#endif // SEGMENT_HPP
