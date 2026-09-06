#include "WorkItem.h"
#include "PendingState.h"
#include "FullTraversalIterator.h"

int WorkItem::nextId = 101;

WorkItem::WorkItem(const std::string& name)
    : id(nextId++), name(name), state(new PendingState())
{}

void WorkItem::setState(WorkState* newState){
    delete state;
    state = newState;
}

WorkItem::~WorkItem(){
    delete state;
}

int WorkItem::getId() const{
    return this->id;
}

std::string WorkItem::getName() const{
    return this->name;
}

bool WorkItem::start(){
    //empty for composites only leaves will override it
    return false;
}

bool WorkItem::block(){
    //empty for composites only leaves will override it
    return false;
}

bool WorkItem::complete(){
    //empty for composites only leaves will override it
    return false;
}

bool WorkItem::add(WorkItem* item){
    //empty for leaves only composites will override it
    (void)item;
    return false;
}

bool WorkItem::remove(WorkItem* item){
    //empty for leaves only composites will override it
    (void)item;
    return false;
}

int WorkItem::getChildCount() const{
    //empty for leaves only composites will override it
    return 0;
}

WorkItem* WorkItem::getChild(int pos) const{
    //empty for leaves, only composites will override it
    (void)pos;
    return nullptr;
}

int WorkItem::getChildIndex(const WorkItem* item) const{
    //empty for leaves only composites will override it
    (void)item;
    return -1;
}

WorkItemIterator* WorkItem::createIterator(){
    //only composites will override it
    return new FullTraversalIterator(this);
}