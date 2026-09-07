#include "CompositeWorkItem.h"
#include "FullTraversalIterator.h"
#include "FilteredIterator.h"

CompositeWorkItem::CompositeWorkItem(const std::string& name)
    : WorkItem(name)
{}

void CompositeWorkItem::execute(){
    for(auto child : children){
        if(child != nullptr){
            child->execute();
        }
    }
}

std::string CompositeWorkItem::getDescription() const{
    if(children.empty())
        return "";

    std::string output = "CompositeWorkItem: " + getName() + "\n";

    for(auto child : children){
        if(child != nullptr){
            output += child->getDescription();
        }
    }

    return output;
}

std::string CompositeWorkItem::getStatus() const{
    if(children.empty())
        return "Pending";

    bool allDone = true;

    for(auto child : children){
        if(child == nullptr)
            continue;

        std::string status = child->getStatus();

        if(status == "Blocked")
            return "Blocked";

        if(status != "Done")
            allDone = false;
    }

    if(allDone)
        return "Done";

    return "Pending";
}

bool CompositeWorkItem::add(WorkItem* item){
    if(item == nullptr)
        return false;

    if(getChildIndex(item) != -1)
        return false; //item exists

    children.push_back(item);
    return true;
}

bool CompositeWorkItem::remove(WorkItem* item){
    if(item == nullptr)
        return false;

    int index = getChildIndex(item);

    if(index == -1)
        return false; //item does not exist

    delete children[index]; //uses composition hence, deletes the child
    children.erase(children.begin() + index);

    return true;
}

int CompositeWorkItem::getChildCount() const{
    return static_cast<int>(children.size());
}

WorkItem* CompositeWorkItem::getChild(int pos) const{
    if(pos < 0 || pos >= getChildCount())
        return nullptr;

    return children[pos];
}

int CompositeWorkItem::getChildIndex(const WorkItem* item) const{
    if(item == nullptr)
        return -1;

    for(int i = 0; i < getChildCount(); ++i){
        if(children[i] == item){
            return i;
        }
    }

    return -1;
}

WorkItem* CompositeWorkItem::detach(WorkItem* item){
    if (item == nullptr)
        return nullptr;

    int index = getChildIndex(item);

    if(index == -1)
        return nullptr; //item does not exist

    WorkItem* detachedItem = children[index];
    children.erase(children.begin() + index);

    return detachedItem;
}

CompositeWorkItem::~CompositeWorkItem(){
    for(auto child : children)
        delete child;
}

WorkItemIterator* CompositeWorkItem::createIterator(){
    return new FullTraversalIterator(this);
}

WorkItemIterator* CompositeWorkItem::createFilteredIterator(const std::function<bool(const WorkItem*)>& predicate){
    return new FilteredIterator(this, predicate);
}
