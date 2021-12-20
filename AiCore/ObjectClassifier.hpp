/******************************************************************************** 
 Module: ObjectClassifier.cpp
 Author: LE MANH THANG
 Created: 06/01/2021
 Description: 
********************************************************************************/
#ifndef OBJECTCLASSIFIER_H
#define OBJECTCLASSIFIER_H

#include "snpe/classifier/SnpeClassifier.hpp"
#include "AiTypeData.hpp"

class ObjectClassifier : SnpeClassifier
{
    private:
        std::vector<std::string> labels;
        std::string framework;
        std::string objectTarget;
    public:
        ObjectClassifier();
        ~ObjectClassifier();
        int initObjectClassifier(std::string framework, std::string objectTarget);
        int executeObjectClassifier(const cv::Mat& img, std::vector<ObjectTrace>& objects, float threshold);
};

#endif