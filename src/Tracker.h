#pragma once

#include <thread>
#include <chrono>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

class Tracker {

    void tracking_fun();
    std::thread tracking_th;
    std::atomic<bool> do_track;
    dlib::chip_details chip;
    int chip_size;

    class FaceTracker* interface;
    class Ffmpeg* ffmpeg;
    class Emotions* emotions;

    dlib::matrix<dlib::rgb_pixel> face_image;
    dlib::rectangle               face_rect;
    dlib::full_object_detection   face_shape;
    
public:
    Tracker(Ffmpeg* ffmpeg, FaceTracker* interface, int chip_size);
    ~Tracker();
    double fps;
    void getLandmarks(float* landmarks);
    void getFaceChip(unsigned char* faceChip);
	void getEmotions(float* emo);
};