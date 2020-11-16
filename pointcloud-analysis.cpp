// Standard includes
#include <iostream>
#include <vector>
#include <string>

#include "segment.hpp"

int main( int argc, char** argv )
{
    // Fetch input file path
    std::string path = "./";
    if( argc > 1)
        path = argv[1];

    Segment SegmentationTest(path);
    //SegmentationTest.export_background();
    SegmentationTest.perform_test();
    //SegmentationTest.clean_dataset();

    return 0;
}
