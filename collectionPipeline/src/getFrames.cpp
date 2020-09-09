#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <fstream>
#include <iostream>
#include <sstream>  // string stream for dynamic string changing (create string obj and write too it and then .str())
#include <iomanip>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


/*  References
 *   https://intelrealsense.github.io/librealsense/doxygen/classrs2_1_1pipeline__profile.html
 *   https://dev.intelrealsense.com/docs/rs-align-advanced
 *   https://github.com/IntelRealSense/librealsense/blob/master/wrappers/opencv/imshow/rs-imshow.cpp
*/


int iter = 0; // global var for frame count

int main(int argc, char** argv) try {

    rs2::context ctx;
    auto list = ctx.query_devices(); // get a snapshot of currently connected devices
    if (list.size() == 0)
        throw std::runtime_error("No device detected. Is it plugged in?");
    rs2::device dev = list.front(); 
    // use later to enable multiple cameras


    rs2::colorizer color_map; // use to convert csv decimal depth values to a color map
    rs2::pipeline pipe;
    rs2::pipeline_profile profile = pipe.start();

    // POSSIBLY RELEVANT FOR REALSENSE DEVELOPMENT IN THE FUTURE:
    // pipeline 'profile' - profile holds all configurations etc
    // pipe.start(params) may be useful for other cameras
   
    // auto depth_stream = profile.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();
    // auto color_stream = profile.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();
    // rs2::align align_to(RS2_STREAM_DEPTH);
    // Pipeline could choose a device that does not have a color stream
    // If there is no color stream, choose to align depth to another stream
    // rs2_stream align_to = find_stream_to_align(profile.get_streams());

    // Create a rs2::align object: rs2::align allows us to perform alignment of depth frames to others frames (color frames etc)
    // The "align_to" is the stream type to which we plan to align depth frames: rs2::align align(align_to);

    // auto depth_units = dev.query_sensors().front().get_option(RS2_OPTION_DEPTH_UNITS); Crashes!

    using namespace cv;

    // C++ clock lib
    std::clock_t start;
    double duration;
    start = std::clock();
 
    rs2::frame c_frame;
    rs2::frame d_frame;
    rs2::frameset frameset;
    time_t my_time;

    while(true){
        // Note problem with this lib - takes the system time when last conencted to wifi ~ try to use different lib
        my_time = time(NULL);

        duration = (std::clock() - start ) / double(CLOCKS_PER_SEC);

        std::cout << duration << std::endl; // seconds between frames
        std::cout << ctime(&my_time);       // actual timezone time

        // block program/while loop until pipe object returns a frameset
        // frameset should contain set of aligned frames (colour and depth)
        frameset = pipe.wait_for_frames();        
            
        if(duration > 3.0){     // change this if you need more time between frame captures
            // frames are n x m matrices of pixels
            c_frame = frameset.get_color_frame();
            d_frame = frameset.get_depth_frame().apply_filter(color_map);

            // if one of them is unavailable, next loop iteration
            if (!c_frame || !d_frame) {
                continue;
            }

            // Create OpenCV matrix of size (w,h) from the colorized depth data
            // get_data returns a pointer to the first frame of group
            cv::Mat c_image(Size(640, 480), CV_8UC3, (void*)c_frame.get_data(), Mat::AUTO_STEP);
            cv::Mat d_image(Size(640, 480), CV_8UC3, (void*)d_frame.get_data(), Mat::AUTO_STEP);

            // Use this if you need to save to a folder:

            // std::ostringstream data_folder;        
            // data_folder << "data/" << ctime(&my_time) << "_" << iter << "/";    // write all the extra stuff to the ostringstream object
            // mkdir(data_folder.str().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);    //.c_str() to convert std::string into c type array pointer to char array
            // The extra params are permission bits

            // Dynamic strings in C++
            std::ostringstream depthString;
            std::ostringstream colorString;

            depthString << "data/Depth_" << ctime(&my_time) << "__" << iter << ".jpg";
            colorString << "data/Color_" << ctime(&my_time) << "__" << iter << ".jpg";

            cv::imwrite(colorString.str(), c_image);
            cv::imwrite(depthString.str(), d_image);
            ++iter; //global variable for keeping frames together

            std::cout << "Clipped" << std::endl;
            start = std::clock();

            // clear string object buffers
            depthString.clear();
            colorString.clear();
            //data_folder.clear();

            } // END OF IF DURATION STATEMENT

        // WAYS TO GET REALSENSE FRAME INFORMATION
        // std::cout << c_frame.get_data_size() << std::endl;
        // std::cout << c_frame.get_timestamp() << std::endl;
        // std::cout << c_frame.get_frame_number() << std::endl;

        // std::cout << d_frame.get_data_size() << std::endl;
        // std::cout << d_frame.get_timestamp() << std::endl;
        // std::cout << d_frame.get_frame_number() << std::endl;

        // std::cout << "Got frames" << std::endl;
        // std::cout << std::endl;

        // Query frame size (width and height)
        // const int wColor = c_frame.as<rs2::video_frame>().get_width();
        // const int hColor = c_frame.as<rs2::video_frame>().get_height();
        // const int wDepth = d_frame.as<rs2::depth_frame>().get_width();
        // const int hDepth = d_frame.as<rs2::depth_frame>().get_height();

        // cv::imshow(color_name, c_image);
        // cv::imshow(depth_name, d_image);

        sleep(1);
    } // END OF WHILE LOOP

    return EXIT_SUCCESS;

} catch (const rs2::error & e){
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
} catch (const std::exception& e){
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}

