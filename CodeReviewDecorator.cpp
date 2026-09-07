#include "CodeReviewDecorator.h"

#include <iostream>

CodeReviewDecorator::CodeReviewDecorator(WorkItem* wrapped)
    : WorkItemDecorator(wrapped), reviewed(false), reviewer("")
{}

void CodeReviewDecorator::approveReview(const std::string& reviewer){
    this->reviewed = true;
    this->reviewer = reviewer;
}

bool CodeReviewDecorator::isReviewed() const{
    return reviewed;
}

void CodeReviewDecorator::execute(){
    if(!reviewed){
        std::cout << "[CodeReview] Warning: executing \"" << getName()
                   << "\" before review has been approved." << std::endl;
    }

    WorkItemDecorator::execute();
}

std::string CodeReviewDecorator::getDescription() const{
    std::string description = WorkItemDecorator::getDescription();

    if(reviewed){
        description += " [Reviewed by " + reviewer + "]";
    } else {
        description += " [Needs Code Review]";
    }

    return description;
}

bool CodeReviewDecorator::complete(){
    if(!reviewed){
        std::cout << "[CodeReview] Cannot complete \"" << getName()
                   << "\": review has not been approved yet." << std::endl;
        return false;
    }

    return WorkItemDecorator::complete();
}
