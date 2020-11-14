#include "Driver.h"

using namespace cv;
using namespace std;

void update_data(rs2::frame_queue& data, rs2::frame& depth, rs2::colorizer& color_map);
void show_image(Mat color, Mat depth);
void CameraDriver();

int main(int argc, char** argv){
    //Start the camera driver
    driver();
    
    //Main Benchmarking Thread
    Benchmarking();
}

void driver(){
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
    pipe.start(cfg);

    //Camera Driver Thread
    std::thread camera_driver([&]() { //Gets frames pushes to queue
        CameraDriver();
    });
    camera_driver.detach();
}

void CameraDriver() {
    while (!stopped){
        rs2::frameset data = pipe.wait_for_frames();
        rs2::frame color_frame = data.get_color_frame();
        rs2::depth_frame depth_fr = data.get_depth_frame();
        

        color_data.enqueue(color_frame);
        depth_data.enqueue(depth_fr);     
    }
}

void Benchmarking() {
    std::pair<Mat, Mat> frame_pair;
    rs2::frame color_frame;
    rs2::frame depth_frame;
    while (!stopped){ //pulls from queue and does processing
        
        frame_pair = getImage();
        Mat color = std::get<0>(frame_pair);
        Mat depth = std::get<1>(frame_pair);
        show_image(color, depth);
        
    }
}




void show_image(Mat color, Mat depth){

    imshow("Color Image", color);
    moveWindow("Color Image", 200,200);
    imshow("Depth Image", depth);
    waitKey(1);
    
    destroyAllWindows();
}


std::pair<cv::Mat,cv::Mat> getImage() {
    rs2::frame color_frame;
    rs2::frame depth_frame;

    Mat color;
    Mat depth;
    while (1){
        if (color_data.poll_for_frame(&color_frame)){
            //cout << "got color frame" << endl;
            color = Mat(Size(640, 480), CV_8UC3, (void*)color_frame.get_data(), Mat::AUTO_STEP);
            break;
        }
    }

    while (1){
        if (depth_data.poll_for_frame(&depth_frame)){
            //cout << "got depth frame" << endl;
            depth = Mat(Size(640, 480), CV_16UC1, (void*)depth_frame.get_data(), Mat::AUTO_STEP);
            depth.convertTo(depth, CV_8UC1, 15 / 256.0);
            break;
        }
    }

    return std::make_pair(color, depth);
}

/*
void update_data(rs2::frame_queue& data, rs2::frame& depth, rs2::colorizer& color_map){
    rs2::frame f;
    if (data.poll_for_frame(&f)){
        depth = f;
    }
}
*/


//Check for exit
        /*
        if((char)cv::waitKey(1) == 27){
            cout << "esc pressed" << endl;
            stopped = true;
            return 0;
        }
        */