#include "segment.hpp"

Segment::Segment(std::string path)
{
    test_title = "segmentation";
    fetch_test_image_paths(path);
}

void Segment::fetch_test_image_paths(std::string path)
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

void Segment::fetch_background(std::string path)
{
    std::cout << "Scanning dataset path \"" << path << "\"..." << std::endl;


}
