/*
    Module: AIUserFactory.cpp
    Author: LE MANH THANG
    Created: Dec 21th, 2021
*/

#include "AIUserFactory.hpp"
#include "snpe/SnpeExecutor.hpp"
#include "ncnn/NcnnExecutor.hpp"

namespace airuntime{

AIExecutor* AIExecutor::create(airuntime::ExecutorType executor, 
                                airuntime::DeviceType device,
                                airuntime::AlgTypeAI algType,
                                std::string pathLabel,
                                std::string modelWeight,
                                std::string modelParam
                                )
{
    AIExecutor* exe = nullptr;
    switch ((int)executor)
    {

        case 0:
        {
            exe = new SnpeExecutor(device, algType, pathLabel, modelWeight);
            break;
        }

        case 1:
        {
            exe = new NcnnExecutor(device, algType,pathLabel, modelWeight, modelParam );
            break;
        }

        default:
        {
            exe = nullptr;
            break;
        }  
    }
    return exe;
}

AIUserFactory::AIUserFactory(airuntime::ExecutorType executor,
                    airuntime::DeviceType device,
                    airuntime::AlgTypeAI algType,
                    std::string pathLabel,
                    std::string modelWeight,
                    std::string modelParam) {
    this->m_executor = AIExecutor::create(executor, device, algType, pathLabel, modelWeight, modelParam);
    
     
}    
AIUserFactory::~AIUserFactory()
{
    cout << "Called AIUserFactory destructor\n";
}

STATUS AIUserFactory::init()
{
    int net = this->m_executor->init();
    if(net != STATUS::SUCCESS)
    {
        cout << "Init AI User Factory failed\n";
        return STATUS::FAIL;
    }
    return STATUS::SUCCESS;
}

STATUS AIUserFactory::run(const Mat& img, 
                            vector<ObjectTrace>& objects,
                            float threshold)
{

    return STATUS::SUCCESS;
}

STATUS AIUserFactory::release()
{
    return STATUS::SUCCESS;
}

}
