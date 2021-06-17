// OpenCV includes
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

// Standard includes
#include <iostream>
#include <string>

// Base class
#include "basetest.hpp"

#define USE_MASK false

class BlurTest : public BaseTest
{
    public:
        BlurTest(std::string path)
        {
            test_title = "blur_test";
            fetch_test_image_paths(path);
            std::sort(test_image_paths.begin(), test_image_paths.end());
        }
        
        void perform_test() override
        {
            if(test_image_paths.empty())
            {
                std::cout << "No image data..." << std::endl;
                return;
            }

            std::cout << "Performing test \"" << test_title << "\"..." << std::endl;
            test_results.clear();
            
            
            for(int i = 0; i < test_image_paths.size(); i++)
            {
                cv::Mat image = cv::imread(test_image_paths[i], cv::IMREAD_COLOR );
                if( image.empty() )
                {
                    std::cout <<  "Could not open or find the image" << std::endl ;
                    return;
                }

                //int image_original_size = image.rows*image.cols;

                if(USE_ROI)
                {
                    cv::Rect crop(448, 115, 400, 120);
                    image = image(crop);
                }

                if(USE_MASK)
                {
                    // SPOOKY MASK SHIT
                    cv::Mat1b mask_image(image.rows, image.cols, uchar(0));
                    cv::Mat grey_image;
                    cv::cvtColor(image, grey_image, CV_BGR2GRAY);
                    for(int col = 0; col < image.cols; col++)
                    {
                        for(int row = 0; row < image.rows; row++)
                        {
                            int pixel = grey_image.at<uchar>(row,col);

                            if(pixel != 0)
                                mask_image.at<uchar>(row,col) = 255;
                        }
                    }
                    cv::Rect box = findMinRect(~mask_image);
                    //cv::Scalar rectColor(0, 255, 0);
                    //cv::rectangle(image, box, rectColor, 2);
                    //cv::imshow("heyho", image);
                    //cv::imshow("heyho", mask_image);
                    //cv::waitKey(0);

                    //cv::Rect crop(448, 115, 400, 120);
                    image = image(box);
                    //cv::imshow("heyho", image);
                    //cv::waitKey(0);
                }

                //int image_new_size = image.rows*image.cols;


                float result = getBlurValueLaplacian(image);//*((image_original_size*1.0)/image_new_size);
                test_results.push_back(result);

                std::cout << test_image_paths[i] << " -> " << result << std::endl;

                //cv::imshow("Dummy", image);
                //cv::waitKey(0);

                // Progress
                //std::cout << "\rProgress: " << (i+1) << "/" << test_image_paths.size() << std::flush;
            }
            
            std::cout << std::endl;
            std::cout << "Done!" << std::endl;
        }
        
    private:
        double getBlurValueLaplacian(cv::Mat input)
        {
            //https://stackoverflow.com/questions/24080123/opencv-with-laplacian-formula-to-detect-image-is-blur-or-not-in-ios
            cv::Mat matImageGrey;
            cv::cvtColor(input, matImageGrey, CV_BGR2GRAY);

            //equalizeHist( matImageGrey, matImageGrey );

            cv::Mat laplacianImage;
            //cv::Laplacian(matImageGrey, laplacianImage, CV_8U);

            //cv::Mat laplacianImage8bit;
            //laplacianImage.convertTo(laplacianImage8bit, CV_8UC1);

            cv::Laplacian(matImageGrey, laplacianImage, CV_64F);
            cv::Scalar mean, stddev; // 0:1st channel, 1:2nd channel and 2:3rd channel
            cv::meanStdDev(laplacianImage, mean, stddev, cv::Mat());
            double variance = stddev.val[0] * stddev.val[0];

            return variance;
        }

        int getBlurValueVert(cv::Mat input)
        {
            int blurVal = 0;

            cv::Mat matImageGrey;
            cv::cvtColor(input, matImageGrey, CV_BGR2GRAY);

            for(int i = 1; i < matImageGrey.size().height-1; i++)
            {
                for(int j = 0; j < matImageGrey.size().width; j++)
                {
                    int pixelAbove = matImageGrey.at<uchar>(i-1,j);
                    int pixel = matImageGrey.at<uchar>(i,j);
                    int pixelBelow = matImageGrey.at<uchar>(i+1,j);

                    blurVal += (pixelAbove*1) + (pixel*-2) + (pixelBelow*1);
                }
            }

            return blurVal;
        }

        int getBlurValueHor(cv::Mat input)
        {
            int blurVal = 0;

            cv::Mat matImageGrey;
            cv::cvtColor(input, matImageGrey, CV_BGR2GRAY);

            for(int j = 1; j < matImageGrey.size().width-1; j++)
            {
                for(int i = 0; i < matImageGrey.size().height; i++)
                {
                    int pixelLeft = matImageGrey.at<uchar>(i,j-1);
                    int pixel = matImageGrey.at<uchar>(i,j);
                    int pixelRight = matImageGrey.at<uchar>(i,j+1);

                    blurVal += (pixelLeft*1) + (pixel*-2) + (pixelRight*1);
                }
            }

            return blurVal;
        }

        cv::Mat convertHistogramEqualizer(cv::Mat input)
        {
            cv::Mat hist_equalized_image;
            cv::cvtColor(input, hist_equalized_image, cv::COLOR_BGR2YCrCb);

            //Split the image into 3 channels; Y, Cr and Cb channels respectively and store it in a std::vector
            std::vector<cv::Mat> vec_channels;
            cv::split(hist_equalized_image, vec_channels);

            //Equalize the histogram of only the Y channel
            cv::equalizeHist(vec_channels[0], vec_channels[0]);

            //Merge 3 channels in the vector to form the color image in YCrCB color space.
            cv::merge(vec_channels, hist_equalized_image);

            //Convert the histogram equalized image from YCrCb to BGR color space again
            cv::cvtColor(hist_equalized_image, hist_equalized_image, cv::COLOR_YCrCb2BGR);

            return hist_equalized_image;
        }

        double occupance(cv::Mat input)
        {
            cv::cvtColor(input, input, CV_BGR2GRAY);

            int occupied = 0;
            for(int col = 0; col < input.cols; col++)
            {
                for(int row = 0; row < input.rows; row++)
                {
                    int pixel = input.at<uchar>(row,col);

                    if(pixel != 0)
                        occupied++;
                }
            }
            return ((occupied*1.0)/(input.cols*input.rows));
        }

        cv::Rect findMinRect(const cv::Mat1b& src)
        {
            cv::Mat1f W(src.rows, src.cols, float(0));
            cv::Mat1f H(src.rows, src.cols, float(0));

            cv::Rect maxRect(0, 0, 0, 0);
            float maxArea = 0.f;

            for (int r = 0; r < src.rows; ++r)
            {
                for (int c = 0; c < src.cols; ++c)
                {
                    if (src(r, c) == 0)
                    {
                        H(r, c) = 1.f + ((r>0) ? H(r - 1, c) : 0);
                        W(r, c) = 1.f + ((c>0) ? W(r, c - 1) : 0);
                    }

                    float minw = W(r, c);
                    for (int h = 0; h < H(r, c); ++h)
                    {
                        minw = std::min(minw, W(r - h, c));
                        float area = (h + 1) * minw;
                        if (area > maxArea)
                        {
                            maxArea = area;
                            maxRect = cv::Rect(cv::Point(c - minw + 1, r - h), cv::Point(c + 1, r + 1));
                        }
                    }
                }
            }

            return maxRect;
        }


        struct lessVec3b
        {
            bool operator()(const cv::Vec3b& lhs, const cv::Vec3b& rhs)
            {
                return (lhs[0] != rhs[0]) ? (lhs[0] < rhs[0]) : ((lhs[1] != rhs[1]) ? (lhs[1] < rhs[1]) : (lhs[2] < rhs[2]));
            }
        };
};
