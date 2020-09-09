#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>


// https://intelrealsense.github.io/librealsense/doxygen/classrs2_1_1pipeline__profile.html
// https://dev.intelrealsense.com/docs/rs-align-advanced


int main(int argc, char** argv) try {

    using namespace cv;


    rs2::context ctx;
    auto list = ctx.query_devices(); // Get a snapshot of currently connected devices
    if (list.size() == 0) 
        throw std::runtime_error("No device detected. Is it plugged in?");
    rs2::device dev = list.front();
    // use later for multiple devices.

    // const auto color_window_name = "Color Image";
    // namedWindow(color_window_name, WINDOW_AUTOSIZE);
    // const auto depth_window_name = "Depth Image";
    // namedWindow(depth_window_name, WINDOW_AUTOSIZE);
    const auto window_name = "Display Image";
    namedWindow(window_name, WINDOW_AUTOSIZE);
    rs2::colorizer color_map;

    rs2::pipeline pipe;
    // pipeline 'profile' - profile holds all configurations etc
    rs2::pipeline_profile profile = pipe.start();
    // pipe.start(params) may be useful for other cameras

    auto depth_stream = profile.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();
    //auto color_stream = profile.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();

    // %@$!$!@# rs2::align align_to(RS2_STREAM_DEPTH);
    //Pipeline could choose a device that does not have a color stream
    //If there is no color stream, choose to align depth to another stream
    // rs2_stream align_to = find_stream_to_align(profile.get_streams());

    // Create a rs2::align object.
    // rs2::align allows us to perform alignment of depth frames to others frames
    //The "align_to" is the stream type to which we plan to align depth frames.
    //rs2::align align(align_to);

      
    while(getWindowProperty(window_name, WND_PROP_AUTOSIZE) >= 0){
        // block propgram until align object returns a frameset
        // frameset should contain set of aligned frames
        rs2::frameset frameset = pipe.wait_for_frames();
        // aligning the depth frames with other frames previously initialized for the align class object        
        // %$#$# auto processed = align_to.process(frameset);
        // get colour and aligned depth frames
        std::cout << std::endl;

        // !#@!rs2::frame d_frame = processed.get_depth_frame().apply_filter(color_map);
        rs2::frame c_frame = frameset.get_color_frame();

        // rs2::video_frame color_frame = frameset.get_color_frame();
        // std::cout << color_frame.get_data_size() << std::endl;
        // std::cout << color_frame.get_timestamp() << std::endl;
        // std::cout << color_frame.get_frame_number() << std::endl << std::endl;

        // rs2::depth_frame depth_frame = frameset.get_depth_frame();
        // std::cout << depth_frame.get_data_size() << std::endl;
        // std::cout << depth_frame.get_timestamp() << std::endl;
        // std::cout << depth_frame.get_frame_number() << std::endl << std::endl;

        std::cout << std::endl;

        std::cout << "Gotcha frames" << std::endl;
        // if one of them is unavailable, next loop iteration
        // if (!color_frame || !depth_frame) {
        //     continue;
        // }

        
        // Query frame size (width and height)
        const int w = c_frame.as<rs2::video_frame>().get_width();
        const int h = c_frame.as<rs2::video_frame>().get_height();
        std::cout << w << " " << h << std::endl;
        // const int wColor = color_frame.as<rs2::video_frame>().get_width();
        // const int hColor = color_frame.as<rs2::video_frame>().get_height();
        // const int wDepth = depth_frame.as<rs2::depth_frame>().get_width();
        // const int hDepth = depth_frame.as<rs2::depth_frame>().get_height();

        // Create OpenCV matrix of size (w,h) from the colorized depth data
        cv::Mat image(Size(w, h), CV_8UC3, (void*)c_frame.get_data(), Mat::AUTO_STEP);

        // cv::Mat imageColor(Size(wColor, hColor), CV_8UC3, (void*)color_frame.get_data(), Mat::AUTO_STEP);
        // cv::Mat imageDepth(Size(wDepth, hDepth), CV_8UC3, (void*)depth_frame.get_data(), Mat::AUTO_STEP);
        // cv::imshow(color_window_name, imageColor);
        // cv::imshow(depth_window_name, imageDepth);
        imshow(window_name, image);

    }

    return EXIT_SUCCESS;

} catch (const rs2::error & e){
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
} catch (const std::exception& e){
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}