// OpenCV includes
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

// Standard includes
#include <iostream>
#include <string>
#include <cmath>

// Base class
#include "BaseTest.hpp"

class DynamicRangeTest : public BaseTest
{
    public:
        DynamicRangeTest(std::vector<std::string> image_paths)
        {
            test_title = "dynamic_range_test";
            test_image_paths = image_paths;
        }
        
        void perform_test() override
        {
            if(test_image_paths.empty())
            {
                std::cout << "No image data..." << std::endl;
                return;
            }

            std::cout << "Performing dynamic range test..." << std::endl;
            test_results.clear();
            
            
            for(int i = 0; i < test_image_paths.size(); i++)
            {
                cv::Mat image = cv::imread(test_image_paths[i], cv::IMREAD_COLOR );
                if( image.empty() )
                {
                    std::cout <<  "Could not open or find the image" << std::endl ;
                    return;
                }

                test_results.push_back(getDynamicRange(image));

                // Progress
                std::cout << "\rProgress: " << (i+1) << "/" << test_image_paths.size() << std::flush;
            }
            
            std::cout << std::endl;
            std::cout << "Done!" << std::endl;
        }
        
    private:
        double getDynamicRange(cv::Mat image_rgb)
        {
            // Convert image to hls
            cv::Mat image_hls;
            cv::cvtColor(image_rgb, image_hls, cv::COLOR_RGB2HLS);

            // Get min and max lightness
            int pixel_l_max = 0;
            int pixel_l_min = 255;
            for(size_t row = 0; row < image_hls.rows; row++)
            {
                for(size_t col = 0; col < image_hls.cols; col++)
                {
                    cv::Vec3b pixel_hls = image_hls.at<cv::Vec3b>(row,col);
                    if(pixel_hls[1] > pixel_l_max)
                        pixel_l_max = pixel_hls[1];
                    if(pixel_hls[1] < pixel_l_min)
                        pixel_l_min = pixel_hls[1];
                }
            }

            // Return dynamic range
            return std::log2(pixel_l_max-pixel_l_min);
        }
};
