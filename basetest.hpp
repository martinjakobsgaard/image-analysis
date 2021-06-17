#ifndef BASETEST_HPP
#define BASETEST_HPP

// OpenCV includes
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

// Standard includes
#include <iostream>
#include <string>
#include <regex>
#include <experimental/filesystem>

#define USE_ROI true

class BaseTest
{
    public:
        BaseTest()
        {
        }
        virtual void perform_test() = 0;    // Needs to fill "test_results" according to images
        virtual void export_results(std::string file_name)
        {
            if(test_results.empty())
            {
                std::cerr << "No results to export..." << std::endl;
                return;
            }
            if(test_image_paths.empty())
            {
                std::cerr << "No images!" << std::endl;
                return;
            }
            std::cout << "Exporting data to \"" << file_name << "\"..." << std::endl;

            std::fstream fout;
            fout.open(file_name, std::ios::out);

            fout << "index,exposure,result\n";

            for(int i = 0; i < test_image_paths.size(); i++)
            {
                std::string image_name = test_image_paths[i];
                std::vector<int> image_data;
                get_data(image_data,image_name, {"_i", "_e", ".jpg"});

                for(int j = 0; j < image_data.size(); j++)
                {
                    fout << image_data[j] << ',';
                }
                fout << test_results[i] << '\n';
            }
        }
    protected:
        void get_data(std::vector<int> &image_data, std::string image_name, std::vector<std::string> delimiters = {"_s", "_i", "_z", "_e", ".png"})
        {
            for(int j = 0; j < delimiters.size()-1; j++)
            {
                std::regex base_regex(delimiters[j] + "(.*)" + delimiters[j+1]);
                std::smatch base_match;
                std::regex_search(image_name, base_match, base_regex);
                //std::cout << delimiters[j] << ": " << base_match[1].str() << std::endl;
                image_data.push_back(std::stoi(base_match[1].str()));
            }
        }
        
        void fetch_test_image_paths(std::string path)
        {
            std::cout << "Scanning dataset path \"" << path << "\"..." << std::endl;
            
            // Generate vector of paths to supported files
            test_image_paths.clear();
            std::vector<std::string> supported_file_extensions = {"jpg", "png"};
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
            std::cout << "Found " << test_image_paths.size() << " image(s)!" << std::endl;
        }

        std::vector<std::string> test_image_paths;
        std::vector<double> test_results;
        std::string test_title = "unspecified_test";
};
#endif // BASETEST_HPP
