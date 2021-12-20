/*
    Module: AnprRecognizer.cpp
    Author: ThangLmb
    Date: June 24, 2021
*/
#include <omp.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "fastcv.h"
#include "AnprRecognizer.hpp"
#include "AnprDetector.hpp"
#include "ObjectDetector.hpp"
#include "CrosslineDetector.hpp"
#include "DecodeFrame.hpp"
#include "MotionDetector.hpp"
#include "ImageProcessor.hpp"
#include "scrfd.hpp"


bool SetAdspLibraryPath(std::string nativeLibPath) {
    std::stringstream path;
    path << nativeLibPath << ";/system/lib/rfsa/adsp;/system/vendor/lib/rfsa/adsp;/dsp";
    return setenv("ADSP_LIBRARY_PATH", path.str().c_str(), 1 /*override*/) == 0;
}

#define TARGET_W 1920
#define TARGET_H 1080
AnprRecognizer* anpr = new AnprRecognizer();
ObjectDetector* obj_detector = new ObjectDetector();
CrosslineDetector* crossline = new CrosslineDetector();
ObjectDetector* person  = new ObjectDetector();
MotionDetector motion;


void testSRCFD()
{
    // export env for DSP
    SetAdspLibraryPath("/data/snpe/dsp/lib");
    snpeBuilders* snpeMobilenetSSD = new snpeBuilders;
	std::vector<std::string> outputLayers;
    snpeMobilenetSSD->runtime = zdl::DlSystem::Runtime_t::DSP;
    // snpeMobilenetSSD->snpe = setBuilderSNPE("/data/thanglmb/models/snpe/lprnet.dlc", outputLayers, snpeMobilenetSSD->runtime);
    snpeMobilenetSSD->snpe = setBuilderSNPE("/data/thanglmb/models/snpe/scrfd_500m_bnkps_shape640x640.dlc", outputLayers, snpeMobilenetSSD->runtime);

   // SnpeScrfd fd;
    zdl::DlSystem::Runtime_t rt = zdl::DlSystem::Runtime_t::DSP;
    std::string containerPath = "/data/thanglmb/models/snpe/scrfd_500m_bnkps_shape640x640.dlc";
    //const cv::Mat img = cv::imread("/data/local/tmp/images/19.jpeg");
    // fd.initSnpeScrfd(containerPath, rt);
    int countLoop = 0;
    while(countLoop++ < 100)
    {
        
        const cv::Mat img = cv::imread("/data/local/tmp/images/19.jpeg"); 
        cv::Mat imgResized;  
        cv::resize(img, imgResized, cv::Size(640, 640));
        // TO DO
        auto start = std::chrono::high_resolution_clock::now(); 
        std::unique_ptr<zdl::DlSystem::ITensor> input;
        // input = convertMat2BgrFloat1(snpeMobilenetSSD->snpe);
        input = convertMat2BgrFloat(snpeMobilenetSSD->snpe, imgResized);

        auto end1 = std::chrono::high_resolution_clock::now();    
        auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start);
        cout << "Time convert: " <<  duration1.count() << endl;

        zdl::DlSystem::TensorMap outputTensorMap;
        int exeStatus  = snpeMobilenetSSD->snpe->execute(input.get(), outputTensorMap);
        if(exeStatus != true)
        {
            printf("Error while executing the MobilenetSSD network \n");
            return ;
        }

        // std::vector<FaceSCRFD> face_objects;
        // fd.executeSnpeScrfd(img, face_objects);
        // cout << "Size detect: " << face_objects.size()  << endl;
    
        auto end2 = std::chrono::high_resolution_clock::now();    
        auto duration12 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start);
        cout << "Time total: " <<  duration12.count() << endl;
    }
    

}

void testDSP()
{	
    // export env for DSP
    SetAdspLibraryPath("/data/snpe/dsp/lib");
	snpeBuilders* snpeMobilenetSSD = new snpeBuilders;
	std::vector<std::string> outputLayers;
    // stride 8
    outputLayers.push_back("Sigmoid_166");
    outputLayers.push_back("Reshape_169");
    outputLayers.push_back("Reshape_172");
    // stride 16
    outputLayers.push_back("Sigmoid_145");
    outputLayers.push_back("Reshape_148");
    outputLayers.push_back("Reshape_151");
    // stride 32
    outputLayers.push_back("Sigmoid_124");
    outputLayers.push_back("Reshape_127");
    outputLayers.push_back("Reshape_130");
    snpeMobilenetSSD->runtime = zdl::DlSystem::Runtime_t::DSP;
    // snpeMobilenetSSD->snpe = setBuilderSNPE("/data/thanglmb/models/snpe/lprnet.dlc", outputLayers, snpeMobilenetSSD->runtime);
    snpeMobilenetSSD->snpe = setBuilderSNPE("/data/thanglmb/models/snpe/scrfd_500m_bnkps_shape640x640.dlc", outputLayers, snpeMobilenetSSD->runtime);
    
    if (snpeMobilenetSSD->snpe  == nullptr)
    {
       std::cerr << "Error while building SNPE object." << std::endl;
       //return nullptr;
       return ;
    }
	// open video test
    const char* video_file = (char*)"/data/thanglmb/videos/person.mp4";
    DecodeFrame decoder;
    decoder.open(video_file);

	cv::Mat frame;

    int w = 640;
    int h = 640;
    int c = 3;
    char* file_name = (char*)"/data/thanglmb/imgs/640.raw";
    FILE* pfile;  
    pfile = fopen(file_name, "rb");
    uint8_t* data = (uint8_t*)malloc(w * h * c * sizeof(uint8_t));
    fread(data, sizeof(uint8_t), w * h * c, pfile);
    fclose(pfile);

	while(1)
	{
		// frame = decoder.getFrame();
		// if (frame.empty()) 
        //     break;
		//frame = imread("/data/local/tmp/images/19.jpeg");
		// TO DO
		auto start = std::chrono::high_resolution_clock::now(); 
		std::vector<ObjectTrace> detected;
        
		//detector->executeObjectDetector(frame, detected, 0.5);
		std::unique_ptr<zdl::DlSystem::ITensor> input;
        // input = convertMat2BgrFloat(snpeMobilenetSSD->snpe, frame);
        input =  creatTensorBGR(snpeMobilenetSSD->snpe, data);

        auto end1 = std::chrono::high_resolution_clock::now();    
        auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start);
        cout << "Time convert: " <<  duration1.count() << endl;

        zdl::DlSystem::TensorMap outputTensorMap;
        int exeStatus  = snpeMobilenetSSD->snpe->execute(input.get(), outputTensorMap);
        if(exeStatus != true)
        {
            printf("Error while executing the MobilenetSSD network \n");
            return ;
        }

        auto end = std::chrono::high_resolution_clock::now();    
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        cout << "Time detect: " <<  duration.count() << endl;
	}
}

void testObject()
{
    // open video test
    const char* video_file = (char*)"/data/thanglmb/videos/person.mp4";
    DecodeFrame decoder;
    decoder.open(video_file);
    
    // export env for DSP
    SetAdspLibraryPath("/data/snpe/dsp/lib");

    int fl_init = 0;
    fl_init =  person->initObjectDetector("snpe", "AnprDetect_Small", "DSP");

    VideoWriter writer;          
    int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');    
   // int codec = VideoWriter::fourcc(*'MP4V'); //MP4V'
    double fps = 30.0;    
    std::string filename = "/data/thanglmb/out/person.avi";    
    writer.open(filename, codec, fps, cv::Size(TARGET_W, TARGET_H));   
    
    while(1)
    {
        printf("--------------------------------------------\n");
        cv::Mat srcimg;
        // srcimg = decoder.getFrame();
        srcimg = imread("/data/thanglmb/imgs/12.png");

        // const uint8_t* rawData = (uint8_t*)loadYUV("/data/thanglmb/imgs/300.yuv", 300, 300);
        // srcimg = cv::imread("/data/thanglmb/imgs/300.png");
        // auto start = std::chrono::high_resolution_clock::now();    
        // vector<ObjectTrace> detected;
        // person->executeObjectDetector(rawData, 300, 300, detected, 0.5);
    
        
        auto start = std::chrono::high_resolution_clock::now();    
        vector<ObjectTrace> detected;
        person->executeObjectDetector(srcimg, detected, 0.5);

        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        cout << "Inferencing: " << time.count() << "ms" << " = " << 1000 / (time.count() + 1) << "FPS\n";
        cout << "Detected object: " << detected.size() << "\n";
        if(detected.size() > 0)
            for(const auto &d : detected)
                cv::rectangle(srcimg, d.rect, cv::Scalar(255, 0, 0), 1, 8);
            
        
        cv::resize(srcimg, srcimg, cv::Size(1920, 1080));
        // writer.write(srcimg);
        // imwrite("/data/thanglmb/out/out.jpg", srcimg);

        // int SRC_WIDTH = 300;
        // int SRC_HEIGHT = 300;
        // int SRC_DEPTH = 3;
        // uint32_t* __restrict dstData = new uint32_t[SRC_WIDTH * SRC_HEIGHT * SRC_DEPTH];
        // // fcvColorYUV420toRGB8888u8(rawData, SRC_WIDTH, SRC_HEIGHT, (uint32_t*)dstData);
        // // fcvColorYCrCb420PseudoPlanarToRGB8888u8(rawData, SRC_WIDTH, SRC_HEIGHT, 0, 0, dstData, 0);
        // // fcvScaleDownBy2Gaussian3x3u8
        // cv::Mat imageFastCV = cv::Mat(SRC_HEIGHT, SRC_WIDTH, CV_8UC4, (void*)dstData);
        // imwrite("/data/thanglmb/out/yuv.jpg", imageFastCV);
        writer.write(srcimg);
    }
}
void testAnpr()
{
    // export env for DSP
    SetAdspLibraryPath("/data/snpe/dsp/lib");

   
    Nations nation = Nations::VN;
    int fl_init = anpr->init(nation);

    // open video test
    const char* video_file = (char*)"/data/thanglmb/videos/road.mp4";
    DecodeFrame decoder;
    decoder.open(video_file);
    VideoWriter writer;          
    int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');    
   // int codec = VideoWriter::fourcc(*'MP4V'); //MP4V'
    double fps = 30.0;    
    std::string filename = "/data/thanglmb/out/out.avi";    
    writer.open(filename, codec, fps, cv::Size(TARGET_W, TARGET_H));   
    while(1)
    {
        printf("--------------------------------------------\n");
        cv::Mat image = decoder.getFrame();
        cv::Mat srcimg;
        cv::resize(image, srcimg, cv::Size(1920, 1080));
        // anpr->recognize(srcimg, plates);
        if(fl_init == STATUS_SUCCESS)
        {
            auto start = std::chrono::high_resolution_clock::now();    
            std::vector<PlateInfor> plates;
            anpr->trackAnpr(srcimg, plates);
            //anpr->recognize(srcimg, plates);
            auto end = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            cout << "Inferencing: " << time.count() << "ms" << " = " << 1000 / time.count() << "FPS\n";
            for(int i = 0; i < plates.size(); i++)
            {
                if(plates[i].isNewEvent == true)
                {
                    printf("-------------\n");
                    std::cout << "isEvent: " << plates[i].isNewEvent << std::endl;
                    std::cout << "ID: " << plates[i].track_id << std::endl;
                    std::cout << "License: " << plates[i].license << std::endl;
                    std::cout << "Type: " << plates[i].typePlate << std::endl;
                    std::cout << "Score: " << plates[i].score << std::endl;
                }
                cv::rectangle(srcimg, plates[i].rect, cv::Scalar(255, 0, 0), 1, 8);
                char text_draw[100];
                sprintf(text_draw, "ID:%d [%s]", plates[i].track_id, plates[i].license.c_str());
                cv::putText(srcimg, text_draw, cv::Point(plates[i].rect.x, plates[i].rect.y), 
                    cv::FONT_HERSHEY_TRIPLEX, srcimg.rows * 0.001, cv::Scalar(0, 255, 0), 1);
            }
        }
        writer.write(srcimg);
    }

}
void testMotion()
{
    // export env for DSP
    SetAdspLibraryPath("/data/snpe/dsp/lib");

    // open video test
    const char* video_file = (char*)"/data/thanglmb/videos/person.mp4";
    DecodeFrame decoder;
    decoder.open(video_file);
    VideoWriter writer;          
    int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');    
    double fps = 30.0;    
    std::string filename = "/data/thanglmb/out/out.avi";    
    writer.open(filename, codec, fps, cv::Size(TARGET_W, TARGET_H));  

    while(1)
    {
        
        cv::Mat image = decoder.getFrame();
        cv::Mat srcimg;
        cv::resize(image, srcimg, cv::Size(300, 300));
        cv::Rect rectMotion;

        auto start = std::chrono::high_resolution_clock::now();    

        int isMotion = motion.isMotionDetected(srcimg, rectMotion);

        auto end = std::chrono::high_resolution_clock::now();    
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        cout << "Time detect: " <<  duration.count() << endl;
        
        if(isMotion)
        {
            cv::rectangle(srcimg, rectMotion, cv::Scalar(0, 255, 0), 1, 8);
            cv::Mat imgMotion = srcimg(rectMotion).clone();

        }    
        
        //writer.write(srcimg);
        
    }
}

int main(int argc, char** argv)
{   
    // testAnpr();
    testObject();
    // testDSP();
    // testSRCFD();
   // omp_set_num_threads(8);



    // auto start  = std::chrono::high_resolution_clock::now();
    // // #pragma omp parallel for
    // // #pragma omp parallel for num_threads(8)
    // #pragma omp parallel
    // {
    //     #pragma omp for
    //     for (int i = 1; i < 10000; i++)
    //     {
    //         // printf("i = %d, I am Thread %d\n", i, omp_get_thread_num());
    //         printf("%d: %f\n", i, (double)sqrt(i));
    //     }
    // }
    // auto end = std::chrono::high_resolution_clock::now();    
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // cout << "Time detect: " <<  duration.count() << endl;
    
        
    
    return 0;
}


