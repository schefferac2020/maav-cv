#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <librealsense2/rs.hpp>
#include <iostream>
#include <vector>
#include <chrono>

#include <thread>
#include <atomic>
#include <mutex>

using namespace cv;
using namespace std;

const auto CAPACITY = 5; // allow max latency of 5 frames
rs2::frame_queue color_data(CAPACITY);
rs2::frame_queue depth_data(CAPACITY);

rs2::pipeline pipe;
rs2::config cfg;

rs2::pointcloud original_pc;
rs2::pointcloud filtered_pc;
    
rs2::colorizer color_map;

std::atomic_bool stopped(false);

//Default benchmarking function
//Depth and Color frames printed to screen
void Benchmarking();

//Starts the camera Driver
void driver();

//Pops an image from the queue
//Returns an std::pair of the Color and Depth frames as cv::Mat types
std::pair<cv::Mat,cv::Mat> getImage();