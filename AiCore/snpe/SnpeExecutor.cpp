/*
    Module: SnpeExecutor.cpp
    Author: LE MANH THANG
    Created: Dec 21th, 2021
*/
#include "SnpeExecutor.hpp"


SnpeExecutor::SnpeExecutor(airuntime::DeviceType device, 
                            airuntime::AlgTypeAI algType,
                            std::string pathLabel,
                            std::string modelWeight
                            ) 
    : airuntime::AIExecutor(airuntime::ExecutorType::SNPE, device, algType, pathLabel, modelWeight)
    {
        cout << "==============> SNPE Executor <=================\n";
        cout << "Model weight: " << getModelWeight() << endl;
        cout << "Model param: " << getModelParam() << endl;
        cout << "Label: " << getPathLabel() << endl;
        cout << "Target device: " << (int)getDeviceType() << endl;
        cout << "================================================\n";
    }

SnpeExecutor::~SnpeExecutor()
{
    cout << "Called SnpeExecutor destructor\n";
}

STATUS SnpeExecutor::init()
{
    if(getAlgType() == airuntime::AlgTypeAI::DETECT)
    {
        this->m_detector = new SnpeMobilenetSSD();
        int net = this->m_detector->initSnpeMobilenetSSD(getModelWeight(), getDeviceType());
        if( net != STATUS::SUCCESS )
        {
            LOG_FAIL("Init Mobilenet SSD failed");
            return STATUS::FAIL;
        } 
    }
    cout << "Initiated SNPE Executor successfully\n";
    return STATUS::SUCCESS;
}

STATUS SnpeExecutor::run(const Mat& img, 
                            vector<ObjectTrace>& objects,
                            float threshold)
{

    return STATUS::SUCCESS;
}

STATUS SnpeExecutor::release(){

    cout << "Released SNPE Executor successfully\n";
    return STATUS::SUCCESS;
}

