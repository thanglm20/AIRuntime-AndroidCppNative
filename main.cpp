/*
    Module: main.cpp
    Author: LE MANH THANG
    Created: Dec 21th, 2021
*/


#include <iostream>
#include "AiCore/AIUserFactory.hpp"

bool SetAdspLibraryPath(std::string nativeLibPath) {
    std::stringstream path;
    path << nativeLibPath << ";/system/lib/rfsa/adsp;/system/vendor/lib/rfsa/adsp;/dsp";
    return setenv("ADSP_LIBRARY_PATH", path.str().c_str(), 1 /*override*/) == 0;
}

int main(int argc, char** args){


    std::cout << "=============>Main<==================\n";

    SetAdspLibraryPath("/data/snpe/dsp/lib");
    airuntime::AIUserFactory* executor = new  airuntime::AIUserFactory(
                                            airuntime::ExecutorType::SNPE, 
                                            airuntime::DeviceType::DSP,
                                            airuntime::AlgTypeAI::DETECT,
                                            "/data/thanglmb/models/snpe/AnprDetect.txt",
                                            "/data/thanglmb/models/snpe/AnprDetect.dlc"
                                            );
    executor->init();
   

    return 0;
}