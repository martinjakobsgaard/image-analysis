// Standard includes
#include <iostream>
#include <vector>
#include <string>
#include <experimental/filesystem>

// Test method includes
#include "BlurTest.hpp"
#include "DynamicRangeTest.hpp"

int main( int argc, char** argv )
{
    // Fetch input file path
    std::string path = "./";
    if( argc == 2)
        path = argv[1];
    std::cout << "Dataset path: \"" << path << "\"" << std::endl;
    
    // Generate vector of paths to supported files
    std::vector<std::string> image_paths;
    std::vector<std::string> supported_file_extensions = {"jpg", "png"};
    for (const auto & entry : std::experimental::filesystem::directory_iterator(path))
    {
        std::string path_string = entry.path();
        std::string file_extension = path_string.substr(path_string.find_last_of(".") + 1); 
        
        for(const auto & supported_file_extension : supported_file_extensions)
        {
            if(file_extension == supported_file_extension)
            {
                image_paths.push_back(path_string);
                break;
            }
        }
    }
    std::cout << "Found " << image_paths.size() << " image(s)!" << std::endl;
    
    // Do something with files
    BlurTest            test1(image_paths);
    DynamicRangeTest    test2(image_paths);

    test1.perform_test();
    test2.perform_test();

    test1.export_results("blur_data.csv");
    test2.export_results("dr_data.csv");
    return 0;
}
