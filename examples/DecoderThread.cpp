/*
    Module: DecoderThread.cpp
    Author: LE MANH THANG
    Created: Dec 21th, 2021
*/

#include "DecoderThread.hpp"


DecoderThread::DecoderThread(FrameManager* frameManger)
{
    this->m_frameManager = frameManger;
}

DecoderThread::~DecoderThread()
{
}

void DecoderThread::run()
{
    pthread_create(&this->m_thread, NULL, threadFunc, this);
}

void DecoderThread::stop()
{

    // pthread_cancel(this->m_thread); // LINUX
    pthread_kill(this->m_thread, SIGUSR1); // ANDROID NDK
    if(this->m_decoder) delete this->m_decoder;
}
 
void* DecoderThread::threadFunc(void* args)
{
    DecoderThread* thread = (DecoderThread*) args;
    thread->process();
    pthread_exit(NULL);
}

void DecoderThread::process()
{
    std::cout << "Creating decoder thread\n";
    this->m_decoder = new DecodeFrame();
    const char* video_file = (char*)"/data/thanglmb/videos/road.mp4";
    int ret = this->m_decoder->open(video_file);
    if(ret == 0)
    while(1)
    {     
        cv::Mat frame = this->m_decoder->getFrame();
        std::cout << "Decoded frame: " << frame.cols  << "x" << frame.rows << std::endl;
        if(!frame.empty())
            this->m_frameManager->updateFrame(frame);
        else 
        {
            std::cout << "Frame is empty\n";
            break;
        }        
    }    
}
