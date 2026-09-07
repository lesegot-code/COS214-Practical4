#include "LoginTask.h"
#include "WorkState.h"
#include "FullTraversalIterator.h"
#include "FilteredIterator.h"

#include <iostream>

LoginTask::LoginTask(const std::string& name, const std::string& authenticationMethod, bool requiresTwoFactor)
    : WorkItem(name), authenticationMethod(authenticationMethod), requiresTwoFactor(requiresTwoFactor)
{}

void LoginTask::execute(){
    std::cout << "Executing login task: " << getName() << std::endl;
    std::cout << "Authentication method: " << authenticationMethod << std::endl;

    if(requiresTwoFactor){
        std::cout << "2FA configuration required." << std::endl;
    }
}

std::string LoginTask::getDescription() const{
    std::string description = "Login task: " + getName();

    if(requiresTwoFactor){
        description += " (2FA required)";
    }

    return description;
}

std::string LoginTask::getStatus() const{
    return state->getName(); //needs the State DP
}

bool LoginTask::start(){
    return state->start(*this); //needs the State DP
}

bool LoginTask::block(){
    return state->block(*this); //needs the State DP
}

bool LoginTask::complete(){
    return state->complete(*this); //needs the State DP
}

WorkItemIterator* LoginTask::createIterator(){
    return new FullTraversalIterator(this);
}

WorkItemIterator* LoginTask::createFilteredIterator(const std::function<bool(const WorkItem*)>& predicate){
    return new FilteredIterator(this, predicate);
}
