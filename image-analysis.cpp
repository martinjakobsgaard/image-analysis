// Standard includes
#include <iostream>
#include <vector>
#include <string>

// Test method includes
#include "BlurTest.hpp"
#include "DynamicRangeTest.hpp"

int main( int argc, char** argv )
{
    // Fetch input file path
    std::string path = "./";
    if( argc < 2)
        return -1; // Error

    for(int i = 1; i<argc; i++)
    {
        path = argv[i];
        std::cout << "Dataset path: \"" << path << "\"" << std::endl;

        // Do something with files
        BlurTest            test1(path);
        test1.perform_test();
        test1.export_results("blur_data_" + std::to_string(i-1) + ".csv");

        DynamicRangeTest    test2(path);
        test2.perform_test();
        test2.export_results("dr_data_" + std::to_string(i-1) + ".csv");
    }

    return 0;
}
