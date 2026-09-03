#include "UITask.h"

#include <iostream>

UITask::UITask(const std::string& name, const std::string& framework, const std::string& screen, bool requiresResponsiveDesign)
    : WorkItem(name), framework(framework), screen(screen), requiresResponsiveDesign(requiresResponsiveDesign)
{}

void UITask::execute(){
    std::cout << "Executing UI task: " << getName() << std::endl;
    std::cout << "Framework: " << framework << std::endl;
    std::cout << "Screen: " << screen << std::endl;

    if(requiresResponsiveDesign){
        std::cout << "Responsive design required." << std::endl;
    }
}

std::string UITask::getDescription() const{
    std::string description = "UI task: " + getName() + " (framework: " + framework + ", screen: " + screen;

    if(requiresResponsiveDesign){
        description += ", responsive design required";
    }

    description += ")";

    return description;
}

std::string UITask::getStatus() const{
    return state->getName(); //requires State pattern
}

bool UITask::start(){
    return state->start(*this); //requires State pattern
}

bool UITask::block(){
    return state->block(*this); //requires State pattern
}

bool UITask::complete(){
    return state->complete(*this); //requires State pattern
}
