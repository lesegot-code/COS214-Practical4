#include "PriorityEscalationDecorator.h"

#include <iostream>

PriorityEscalationDecorator::PriorityEscalationDecorator(WorkItem* wrapped, const std::string& priorityLevel)
    : WorkItemDecorator(wrapped), priorityLevel(priorityLevel)
{}

void PriorityEscalationDecorator::escalate(const std::string& newPriorityLevel){
    this->priorityLevel = newPriorityLevel;
}

std::string PriorityEscalationDecorator::getPriorityLevel() const{
    return priorityLevel;
}

void PriorityEscalationDecorator::execute(){
    std::cout << "[Priority: " << priorityLevel << "] Escalated task \"" << getName()
               << "\" is being worked on." << std::endl;

    WorkItemDecorator::execute();
}

std::string PriorityEscalationDecorator::getDescription() const{
    return "[PRIORITY: " + priorityLevel + "] " + WorkItemDecorator::getDescription();
}
