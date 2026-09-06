#include "ProfileTask.h"
#include "WorkState.h"
#include "FullTraversalIterator.h"
#include "FilteredIterator.h"

#include <iostream>

ProfileTask::ProfileTask(const std::string& name, int profileFields, bool supportsAvatar, bool requiresPrivacySettings)
    : WorkItem(name), profileFields(profileFields), supportsAvatar(supportsAvatar), requiresPrivacySettings(requiresPrivacySettings)
{}

void ProfileTask::execute(){
    std::cout << "Executing profile task: " << getName() << std::endl;
    std::cout << "Profile fields: " << profileFields << std::endl;

    if(supportsAvatar){
        std::cout << "Avatar functionality required." << std::endl;
    }

    if(requiresPrivacySettings){
        std::cout << "Privacy settings required." << std::endl;
    }
}

std::string ProfileTask::getDescription() const{
    std::string description = "Profile task: " + getName() + " (profile fields: " + std::to_string(profileFields);

    if(supportsAvatar){
        description += ", avatar required";
    }

    if(requiresPrivacySettings){
        description += ", privacy settings required";
    }

    description += ")";

    return description;
}

std::string ProfileTask::getStatus() const{
    return state->getName(); //requires State pattern
}

bool ProfileTask::start(){
    return state->start(*this); //requires State pattern
}

bool ProfileTask::block(){
    return state->block(*this); //requires State pattern
}

bool ProfileTask::complete(){
    return state->complete(*this); //requires State pattern
}

WorkItemIterator* ProfileTask::createIterator(){
    return new FullTraversalIterator(this);
}

WorkItemIterator* ProfileTask::createFilteredIterator(const std::function<bool(const WorkItem*)>& predicate){
    return new FilteredIterator(this, predicate);
}
