// OpenCV includes
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

// Standard includes
#include <iostream>
#include <string>
#include <sys/types.h> 
#include <sys/stat.h> 

// Base class
#include "BaseTest.hpp"

class EqualizationTest : public BaseTest
{
    public:
        EqualizationTest(std::vector<std::string> image_paths)
        {
            test_title = "equalization_test";
            test_image_paths = image_paths;
        }
        
        void perform_test() override
        {
            if(test_image_paths.empty())
            {
                std::cout << "No image data..." << std::endl;
                return;
            }

            std::cout << "Performing equalization test..." << std::endl;
            export_images.clear();
            
            
            for(int i = 0; i < test_image_paths.size(); i++)
            {
                cv::Mat image = cv::imread(test_image_paths[i], cv::IMREAD_COLOR );
                if( image.empty() )
                {
                    std::cout <<  "Could not open or find the image" << std::endl ;
                    return;
                }

                export_images.push_back(convertHistogramEqualizer(image));

                // Progress
                std::cout << "\rProgress: " << (i+1) << "/" << test_image_paths.size() << std::flush;
            }
            
            std::cout << std::endl;
            std::cout << "Done!" << std::endl;

            customExport("./equ_images/");
        }
        

    private:
        cv::Mat convertHistogramEqualizer(cv::Mat input)
        {
            cv::Mat hist_equalized_image;
            cv::cvtColor(input, hist_equalized_image, cv::COLOR_BGR2YCrCb);

            //Split the image into 3 channels; Y, Cr and Cb channels respectively and store it in a std::vector
            std::vector<cv::Mat> vec_channels;
            cv::split(hist_equalized_image, vec_channels);

            //Equalize the histogram of only the Y channel
            cv::equalizeHist(vec_channels[0], vec_channels[0]);

            //Merge 3 channels in the vector to form the color image in YCrCB color space.
            cv::merge(vec_channels, hist_equalized_image);

            //Convert the histogram equalized image from YCrCb to BGR color space again
            cv::cvtColor(hist_equalized_image, hist_equalized_image, cv::COLOR_YCrCb2BGR);

            return hist_equalized_image;
        }
        
        void custom_export(std::string file_name)
        {
            std::string export_directory = file_name;

            if(test_image_paths.empty())
            {
                std::cerr << "No images!" << std::endl;
                return;
            }
            
            const char *c_file_name = file_name.c_str();
            if(mkdir(c_file_name,0755) == 0)
            {
                std::cout << "Exporting equalized data to \"" << export_directory << "\"..." << std::endl;

                for(int i = 0; i < test_image_paths.size(); i++)
                {
                    std::string image_name = test_image_paths[i];
                    std::string export_name = export_directory + "equ_" + std::to_string(i) + ".jpg";
                    cv::imwrite(export_name, export_images[i]);
                    std::cout << export_directory + "equ_" + std::to_string(i) + ".jpg" << std::endl;
                }
            }
            else
                std::cerr << "Could not create directory..." << std::endl;
        }

        std::vector<cv::Mat> export_images;
};
