#ifndef BASETEST_HPP
#define BASETEST_HPP

// OpenCV includes
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

// Standard includes
#include <iostream>
#include <string>
#include <regex>

class BaseTest
{
    public:
        BaseTest()
        {
        }
        virtual void perform_test() = 0;    // Needs to fill "test_results" according to images
        void export_results(std::string file_name = ("data.csv"))
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

            std::array<std::string,5> delimiter = {"_i", "_s", "_z", "_e", ".jpg"};

            fout << "index" << ',' << "sequence" << ',' << "robotz" << ',' << "exposure" << ',' << "result" << '\n';

            for(int i = 0; i < test_image_paths.size(); i++)
            {
                std::string image_name = test_image_paths[i];

                for(int j = 0; j < delimiter.size()-1; j++)
                {
                    std::regex base_regex(delimiter[j] + "(.*)" + delimiter[j+1]);
                    std::smatch base_match;
                    std::regex_search(image_name, base_match, base_regex);
                    fout << base_match[1].str() << ',';
                }
                fout << test_results[i] << '\n';
            }
        }
    protected:
        std::vector<std::string> test_image_paths;
        std::vector<int> test_results;
        std::string test_title = "unspecified_test";
};
#endif // BASETEST_HPP
