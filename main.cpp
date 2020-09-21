#include <iostream>

using namespace std;

int main( int argc, char** argv )
{
    std::String imageName;
    if( argc > 1)
        imageName = argv[1];

    cv::Mat image;
    image = cv::imread( samples::findFile( imageName ), cv::IMREAD_COLOR );
    if( image.empty() )
    {
        std:.cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    cout << "Hello World!" << endl;
    return 0;
}
