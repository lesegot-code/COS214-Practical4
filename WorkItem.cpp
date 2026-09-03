#include "WorkItem.h"

int WorkItem::nextId = 101;

WorkItem::WorkItem(const std::string& name)
    : id(nextId++), name(name)
{}

int WorkItem::getId() const{
    return this->id;
}

std::string WorkItem::getName() const{
    return this->name;
}

bool WorkItem::start(){
    //empty for composites, only leaves will override it
    return false;
}

bool WorkItem::block(){
    //empty for composites, only leaves will override it
    return false;
}

bool WorkItem::complete(){
    //empty for composites, only leaves will override it
    return false;
}

bool WorkItem::add(WorkItem* item){
    //empty for leaves, only composites will override it
    return false;
}

bool WorkItem::remove(WorkItem* item){
    //empty for leaves, only composites will override it
    return false;
}

int WorkItem::getChildCount() const{
    //empty for leaves, only composites will override it
    return 0;
}

WorkItem* WorkItem::getChild(int pos) const{
    //empty for leaves, only composites will override it
    return nullptr;
}

int WorkItem::getChildIndex(const WorkItem* item) const{
    //empty for leaves, only composites will override it
    return -1;
}
