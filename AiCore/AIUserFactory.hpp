/*
    Module: AIUserFactory.cpp
    Author: LE MANH THANG
    Created: Dec 21th, 2021
*/

#ifndef AIUserFactory_hpp
#define AIUserFactory_hpp

#include "AIExecutor.hpp"

namespace airuntime{
class AIUserFactory
{
private:
    /* data */
    airuntime::AIExecutor* m_executor;
public:
    explicit AIUserFactory(airuntime::ExecutorType executor,
                    airuntime::DeviceType device,
                    airuntime::AlgTypeAI algType,
                    std::string pathLabel,
                    std::string modelWeight,
                    std::string modelParam = ""
                    );
                
    ~AIUserFactory();
    
    STATUS init();
    STATUS run(const Mat& img, 
                vector<ObjectTrace>& objects,
                float threshold);
    STATUS release();
};
}





#endif
