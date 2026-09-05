#include "PasswordTask.h"
#include "WorkState.h"
#include <iostream>

PasswordTask::PasswordTask(const std::string& name, const std::string& hashAlgorithm, int minimumLength, bool requiresSpecialCharacter)
    : WorkItem(name), hashAlgorithm(hashAlgorithm), minimumLength(minimumLength), requiresSpecialCharacter(requiresSpecialCharacter)
{}

void PasswordTask::execute(){
    std::cout << "Executing password task: " << getName() << std::endl;
    std::cout << "Hash algorithm: " << hashAlgorithm << std::endl;
    std::cout << "Minimum password length: " << minimumLength << std::endl;

    if(requiresSpecialCharacter)
        std::cout << "Special character required." << std::endl;
}

std::string PasswordTask::getDescription() const{
    std::string description = "Password task: " + getName();

    description += " (minimum length: " + std::to_string(minimumLength) + ", hash algorithm: " + hashAlgorithm;

    if(requiresSpecialCharacter){
        description += ", special character required";
    }

    description += ")";

    return description;
}

std::string PasswordTask::getStatus() const{
    return state->getName(); //requires State pattern
}

bool PasswordTask::start(){
    return state->start(*this); //requires State pattern
}

bool PasswordTask::block(){
    return state->block(*this); //requires State pattern
}

bool PasswordTask::complete(){
    return state->complete(*this); //requires State pattern
}
