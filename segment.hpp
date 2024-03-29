#ifndef SEGMENT_HPP
#define SEGMENT_HPP

// OpenCV includes
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

// Standard includes
#include <iostream>
#include <string>
#include <regex>
#include <experimental/filesystem>

// Include RealSense Cross Platform API
#include <librealsense2/rs.hpp>
#include <librealsense2/rs_advanced_mode.hpp>
#include <librealsense2/rsutil.h>

// Include pcl
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>

// Some specs
#define HISTEQ true
#define PROC_MASK true
class Segment
{
public:
    Segment(std::string path)
    {
        test_title = "segmentation";
        fetch_test_image_paths(path);
        fetch_test_cloud_paths(path);
        std::sort(test_image_paths.begin(), test_image_paths.end());
        std::sort(test_cloud_paths.begin(), test_cloud_paths.end());
        fetch_background(path);
    }

    void clean_dataset()
    {
        std::ofstream ofs;
        std::ifstream ifs("index.txt");
        std::string line;
        int index = 0;
        if (ifs.is_open())
        {
            while ( getline (ifs,line) )
                 index = std::stoi(line);
            ifs.close();
        }

        int error_count = 0;
        for(int i = index; i<test_image_paths.size(); i++)
        {
            // Pointcloud name
            std::string image_path = test_image_paths[i];
            std::string cloud_path = test_cloud_paths[i];

            // Save paths to clean
            ofs.open("file_pcd.txt",std::ofstream::trunc);
            ofs << cloud_path;
            ofs.close();
            ofs.open("file_png.txt",std::ofstream::trunc);
            ofs << image_path;
            ofs.close();

            std::cout << "Reading image: \"" << image_path << "\"" << std::endl;
            cv::Mat test_image = cv::imread(image_path);

            std::cout << "Reading cloud: \"" << cloud_path << "\"" << std::endl;
            pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
            if (pcl::io::loadPCDFile<pcl::PointXYZ> (cloud_path, *cloud) == -1) //* load the file
            {
                PCL_ERROR ("Couldn't read file.\n");
                error_count++;
                return; // Is grabbed by cleanup script
            }

            ofs.open("index.txt",std::ofstream::trunc);
            ofs << i;
            ofs.close();
        }
        std::cout << "I'm done!" << std::endl;
        std::cout << "I failed " << error_count << " time(s)!" << std::endl;
        std::cout << "(and a few more because i can't count weird segfaults!)" << std::endl;
    }

    void perform_test()
    {
        cv::namedWindow("Segmented image");
        mkdir("./export/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        int export_count = 0;
        for(int i = 11; i<test_image_paths.size(); i++)
        {
            // Get paths
            std::string image_path = test_image_paths[i];
            std::string cloud_path = test_cloud_paths[i];

            // Read data (with fail safe!)
            std::cout << "Reading data:" << std::endl;
            std::cout << "> " << image_path.substr(image_path.find("export"), image_path.length()) << std::endl;
            cv::Mat test_image = cv::imread(image_path);
            if( test_image.empty() )
            {
                std::cout <<  "> Could not open or find the image!" << std::endl ;
                continue;
            }
            std::cout << "> " << cloud_path.substr(cloud_path.find("export"), cloud_path.length()) << std::endl;
            pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
            if (pcl::io::loadPCDFile<pcl::PointXYZ> (cloud_path, *cloud) == -1) //* load the file
            {
                std::cout <<  "> Could not open or find the point cloud!" << std::endl ;
                continue;
            }

            // Generate segmentation mask from background image
            float* background_image_ptr = (float*)background_image.data;
            cv::Mat mask_image(cv::Size(background_image.cols,background_image.rows), CV_8UC1, cv::Scalar(0));
            uchar* mask_image_ptr = mask_image.data;
            size_t index = 0;
            for (const auto& point: *cloud)
            {
                //std::cout << point.z << std::endl;
                if ((point.z) != 0 && (point.z < 1.0) && (background_image_ptr[index]-point.z) > 0.15) // was 0.08
                {
                    mask_image_ptr[index] = 255;
                    //std::cout << background_image_ptr[index]-point.z << std::endl;
                    //std::cout << "I found one!" << std::endl;
                }
                index++;
            }

            // Optional: Erode/dilate mask
            if(PROC_MASK)
            {
                // Create dilation element
                int dilation_type = 0;
                int dilation_size = 2;
                cv::Mat dilation_element = cv::getStructuringElement( dilation_type,
                                     cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                     cv::Point( dilation_size, dilation_size ) );

                // Create erosion element
                int erosion_type = 0;
                int erosion_size = 2;
                cv::Mat erosion_element = cv::getStructuringElement( erosion_type,
                                     cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                     cv::Point( erosion_size, erosion_size ) );

                // Process mask
                cv::dilate( mask_image, mask_image, dilation_element );
                cv::erode( mask_image, mask_image, erosion_element );
            }

            // Find largest mask contour (likely garment)
            cv::Rect bounding_rect;
            std::vector<std::vector<cv::Point>> contours;
            std::vector<cv::Vec4i> hierarchy;
            cv::findContours( mask_image, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

            int largest_contour_area=0;
            int largest_contour_index=0;
            for( int i = 0; i < contours.size(); i++ )
            {
                double contour_area = cv::contourArea(contours[i],false);
                if(contour_area > largest_contour_area)
                {
                    largest_contour_area = contour_area;
                    largest_contour_index = i;
                }
            }

            // Generate new mask from contour
            cv::Mat mask_image_contour(cv::Size(background_image.cols,background_image.rows), CV_8UC1, cv::Scalar(0));
            cv::drawContours( mask_image_contour, contours, largest_contour_index, cv::Scalar(255), CV_FILLED, 8, hierarchy, 0, cv::Point());

            // Create test image. Optional: Equalize image
            cv::Mat segmented_image;
            if(HISTEQ)
            {
                cv::Mat histeq_image = test_image.clone();
                cv::cvtColor( histeq_image, histeq_image, cv::COLOR_BGR2GRAY );
                cv::equalizeHist( histeq_image, segmented_image );
            }
            else
            {
                segmented_image = test_image.clone();
            }

            // Calculate mask alignment
            int image_w_dif = mask_image.cols - test_image.cols;
            int image_h_dif = mask_image.rows - test_image.rows;
            int image_x_disp = -20;
            int image_y_disp = 0;
            std::array<int,2> padding = { (image_h_dif/2)-image_y_disp,   // Top
                                          (image_w_dif/2)+image_x_disp }; // Left
            //padding[0];
            //padding[1];

            // Apply contour mask to image
            for(size_t rows = 0; rows < segmented_image.rows; rows++)
            {
                for(size_t cols = 0; cols < segmented_image.cols; cols++)
                {
                    int mask_value = mask_image_contour.at<uchar>(cv::Point(cols+padding[1],rows+padding[0]));

                    if(mask_value == 0)
                    {
                        if(HISTEQ)
                        {
                            segmented_image.at<uchar>(rows,cols) = 0;
                        }
                        else
                        {
                            cv::Vec3b & color = segmented_image.at<cv::Vec3b>(rows,cols);
                            color[0] = 0;
                            color[1] = 0;
                            color[2] = 0;
                        }
                    }
                }
            }

            // Show image
            cv::imshow("Segmented image", segmented_image);
            std::string export_name = "./export/" + image_path.substr(image_path.find("export"), image_path.length());
            //std::cout << "Writing data:" << std::endl;
            //std::cout << "> " << export_name << std::endl;
            //std::string mask_name = "./export/mask"+std::to_string(export_count)+".jpg";
            //std::string contour_name = "./export/mask"+std::to_string(export_count)+"_contour.jpg";
            //imwrite(mask_name, mask_image);
            //imwrite(contour_name, mask_image_contour);
            export_count++;
            int k = cv::waitKey(0); // Wait for a keystroke in the window
        }
        std::cout << "I'm done!" << std::endl;
    }

    void export_background()
    {
        cv::Mat debugBackGround;
        background_image.convertTo(debugBackGround,CV_8UC1,255.0/1.2);
        cv::imwrite("debug_image.png", debugBackGround);
    }

private:
    // Functions
    void fetch_test_image_paths(std::string path)
    {
        std::cout << "Scanning dataset path \"" << path << "\"..." << std::endl;

        // Generate vector of paths to supported files
        test_image_paths.clear();
        std::vector<std::string> supported_file_extensions = {"png","jpg"};
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
        std::cout << "Found " << test_image_paths.size() << " images(s)!" << std::endl;
    }

    void fetch_test_cloud_paths(std::string path)
    {
        std::cout << "Scanning dataset path \"" << path << "\"..." << std::endl;

        // Generate vector of paths to supported files
        test_cloud_paths.clear();
        std::vector<std::string> supported_file_extensions = {"pcd"};
        for (const auto & entry : std::experimental::filesystem::directory_iterator(path))
        {
            std::string path_string = entry.path();
            std::string file_extension = path_string.substr(path_string.find_last_of(".") + 1);

            for(const auto & supported_file_extension : supported_file_extensions)
            {
                if(file_extension == supported_file_extension)
                {
                    test_cloud_paths.push_back(path_string);
                    break;
                }
            }
        }
        std::cout << "Found " << test_cloud_paths.size() << " cloud(s)!" << std::endl;
    }
    void fetch_background(std::string path)
    {
        std::string background_file_path = path+background_file_name;
        std::cout << "Looking for background file \"" << background_file_name << "\"..." << std::endl;

        // Find and export background image
        cv::FileStorage fsread(background_file_path, cv::FileStorage::READ);
        if( fsread.isOpened())
        {
            std::cout << "Found background file!" << std::endl;
            fsread["backgroundImg"] >> background_image;
            fsread.release();
        }
        else
        {
            std::cerr << "WARNING: Could not find background file!!!" << std::endl;
            std::cerr << "WARNING: I repeat, could not find background file!!!" << std::endl;
            std::cerr << "WARNING: I'm serious, do not spend 45 minutes troubleshooting all black images like me!!!" << std::endl;
        }
    }

    // Variables
    std::vector<std::string> test_image_paths;
    std::vector<std::string> test_cloud_paths;

    std::vector<double> test_results;
    std::string test_title = "segmentation_test";
    std::string background_file_name = "garmentHeightBackground.xml";
    cv::Mat background_image = cv::Mat(cv::Size(848,480), CV_32FC1, cv::Scalar(0));
};

#endif // SEGMENT_HPP
