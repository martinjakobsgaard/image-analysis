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

    std::cout << path << "Hello darkness my old friend" << std::endl;

    return 0;
}
