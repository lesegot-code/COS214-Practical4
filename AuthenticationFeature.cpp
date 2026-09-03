#include "AuthenticationFeature.h"

AuthenticationFeature::AuthenticationFeature(const std::string& name)
    : WorkItem(name)
{}

void AuthenticationFeature::execute(){
    for(auto child : children){
        if(child != nullptr){
            child->execute();
        }
    }
}

std::string AuthenticationFeature::getDescription() const{
    std::string output = "AuthenticationFeature: " + getName() + "\n";

    for(auto child : children){
        if(child != nullptr){
            output += child->getDescription();
        }
    }

    return output;
}

std::string AuthenticationFeature::getStatus() const{
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

bool AuthenticationFeature::add(WorkItem* item){
    if(item == nullptr)
        return false;

    if(getChildIndex(item) != -1)
        return false; //item exists

    children.push_back(item);
    return true;
}

bool AuthenticationFeature::remove(WorkItem* item){
    if(item == nullptr)
        return false;

    int index = getChildIndex(item);

    if(index == -1)
        return false; //item does not exist

    delete children[index]; //uses composition hence, deletes the child
    children.erase(children.begin() + index);

    return true;
}

int AuthenticationFeature::getChildCount() const{
    return static_cast<int>(children.size());
}

WorkItem* AuthenticationFeature::getChild(int pos) const{
    if(pos < 0 || pos >= getChildCount())
        return nullptr;

    return children[pos];
}

int AuthenticationFeature::getChildIndex(const WorkItem* item) const{
    if(item == nullptr)
        return -1;

    for(int i = 0; i < getChildCount(); ++i){
        if(children[i] == item){
            return i;
        }
    }

    return -1;
}

AuthenticationFeature::~AuthenticationFeature(){
    for(auto child : children)
        delete child;
}
