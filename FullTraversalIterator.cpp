#include "FullTraversalIterator.h"
#include "WorkItem.h"

FullTraversalIterator::FullTraversalIterator(WorkItem* root)
    : position(0)
{
    if(root != nullptr){
        collect(root);
    }
}

void FullTraversalIterator::collect(WorkItem* node){
    if(node == nullptr)
        return;

    //Add every node — no predicate/filter.
    items.push_back(node);

    //Recursively visit children.
    for(int i = 0; i < node->getChildCount(); ++i){
        collect(node->getChild(i));
    }
}

bool FullTraversalIterator::hasNext() const{
    return position < items.size();
}

WorkItem* FullTraversalIterator::next(){
    if(!hasNext())
        return nullptr;

    return items[position++];
}
