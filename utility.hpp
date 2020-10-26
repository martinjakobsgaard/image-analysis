#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <experimental/filesystem>

void get_filenames(std::vector<std::string> &filenames, std::experimental::filesystem::path path )
{
    const std::experimental::filesystem::directory_iterator end{};

    for( std::experimental::filesystem::directory_iterator iter{path} ; iter != end ; ++iter )
    {
        if( std::experimental::filesystem::is_regular_file(*iter) )
            filenames.push_back( iter->path().string() ) ;
    }
}

#endif // UTILITY_HPP
