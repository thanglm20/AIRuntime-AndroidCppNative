

/*
    Module: FrameManger.cpp
    Author: LE MANH THANG
    Created: Dec 21th, 2021
*/
#ifndef FrameManager_hpp
#define FrameManager_hpp

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"

class FrameManager
{
private:
    /* data */
    cv::Mat m_frame;
public:
    FrameManager();
    ~FrameManager();
    void updateFrame(const cv::Mat& frame);
    cv::Mat getFrame();
};

FrameManager::FrameManager(){;}
FrameManager::~FrameManager(){;}

void FrameManager::updateFrame(const cv::Mat& frame)
{
    frame.copyTo(this->m_frame);
}
cv::Mat FrameManager::getFrame()
{
    return this->m_frame;
}

#endif