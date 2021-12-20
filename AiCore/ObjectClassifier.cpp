/******************************************************************************** 
 Copyright (C) 2020, LE MANH THANG. All rights reserved.
 Module: ObjectClassifier.cpp
 Author: LE MANH THANG
 Created: 06/01/2021
 Description: 
********************************************************************************/
#include "ObjectClassifier.hpp"
#include "LoadLabel.hpp"
ObjectClassifier::ObjectClassifier():SnpeClassifier()
{

}

ObjectClassifier::~ObjectClassifier()
{

}
bool compare(ObjectTrace a, ObjectTrace b) 
{
    return (a.score > b.score);
}
int ObjectClassifier::initObjectClassifier(std::string framework, std::string objectTarget)
{
    this->framework = framework;
    this->objectTarget = objectTarget;
    if(this->framework == "ncnn")
    {   
        printf("[INFO]-Using framework Ncnn\n");
        if(this->objectTarget == "people")
        {          
            printf("Init people detection successfully\n");
        }
        else if(this->objectTarget == "face_mask")
        {            
            printf("Init face mask detection successfully\n");
        }
    }

    else if(this->framework == "snpe")
    {
        printf("[INFO]-Using framework Ncnn\n");
        zdl::DlSystem::Runtime_t target_device =  zdl::DlSystem::Runtime_t::GPU;
        std::string path_model = DIR_MODEL_OBJECTCLASSIFICATION + this->objectTarget + ".dlc";
        if(initSnpeClassifier( path_model) != 0)
        {
            printf("Initiate object classifier error\n");
            return -1;
        }
        std::string path_label = DIR_LABEL_OBJECTCLASSIFICATION + this->objectTarget + ".txt";
        if(loadLabel(path_label, labels) != 0)
        {
            printf("Initiate network error\n");
            return -1;
        }
        printf("Init people detection successfully\n");
    }
    else if(this->framework == "tflite")
    {
        
    } 
    return 0;
}
int ObjectClassifier::executeObjectClassifier(const cv::Mat& img, std::vector<ObjectTrace>& objects, float threshold)
{
    std::vector<ObjectTrace> classes;
    objects.clear();
    // excute ncnn
    if(this->framework == "ncnn")
    {

    }
    // excute snpe
    else if(this->framework == "snpe")
    {
        //std::string inputraw = "/data/snpe/dataset/red.raw";
        //std::unique_ptr<zdl::DlSystem::ITensor> inputTensor = loadInputTensorByteToFloat(inputraw);
        executeSnpeClassifier( img, this->labels, classes);
    }
    //
    /* only push_back maxscore*/
    std::sort(classes.begin(), classes.end(), compare);
    ObjectTrace obj;
    obj.label = classes[0].label;
    obj.score = classes[0].score;
    objects.push_back(obj);
    return 0;
}