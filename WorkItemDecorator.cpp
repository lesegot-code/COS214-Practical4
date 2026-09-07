#include "WorkItemDecorator.h"

WorkItemDecorator::WorkItemDecorator(WorkItem* wrapped)
    : WorkItem(wrapped != nullptr ? wrapped->getName() : "Unnamed"), wrapped(wrapped)
{}

void WorkItemDecorator::execute(){
    if(wrapped != nullptr){
        wrapped->execute();
    }
}

std::string WorkItemDecorator::getDescription() const{
    if(wrapped == nullptr)
        return "";

    return wrapped->getDescription();
}

std::string WorkItemDecorator::getStatus() const{
    if(wrapped == nullptr)
        return "";

    return wrapped->getStatus();
}

bool WorkItemDecorator::start(){
    if(wrapped == nullptr)
        return false;

    return wrapped->start();
}

bool WorkItemDecorator::block(){
    if(wrapped == nullptr)
        return false;

    return wrapped->block();
}

bool WorkItemDecorator::complete(){
    if(wrapped == nullptr)
        return false;

    return wrapped->complete();
}

bool WorkItemDecorator::add(WorkItem* item){
    if(wrapped == nullptr)
        return false;

    return wrapped->add(item);
}

bool WorkItemDecorator::remove(WorkItem* item){
    if(wrapped == nullptr)
        return false;

    return wrapped->remove(item);
}

WorkItem* WorkItemDecorator::detach(WorkItem* item){
    if(wrapped == nullptr)
        return nullptr;

    return wrapped->detach(item);
}

WorkItemIterator* WorkItemDecorator::createIterator(){
    if(wrapped == nullptr)
        return nullptr;

    return wrapped->createIterator();
}

WorkItemIterator* WorkItemDecorator::createFilteredIterator(const std::function<bool(const WorkItem*)>& predicate){
    if(wrapped == nullptr)
        return nullptr;

    return wrapped->createFilteredIterator(predicate);
}

int WorkItemDecorator::getChildCount() const{
    if(wrapped == nullptr)
        return 0;

    return wrapped->getChildCount();
}

WorkItem* WorkItemDecorator::getChild(int pos) const{
    if(wrapped == nullptr)
        return nullptr;

    return wrapped->getChild(pos);
}

int WorkItemDecorator::getChildIndex(const WorkItem* item) const{
    if(wrapped == nullptr)
        return -1;

    return wrapped->getChildIndex(item);
}

WorkItem* WorkItemDecorator::getWrapped() const{
    return wrapped;
}

WorkItem* WorkItemDecorator::release(){
    WorkItem* released = wrapped;
    wrapped = nullptr;
    return released;
}

WorkItemDecorator::~WorkItemDecorator(){
    delete wrapped;
}
