#include "FilteredIterator.h"
#include "WorkItem.h"

FilteredIterator::FilteredIterator(WorkItem* root, const std::function<bool(const WorkItem*)>& predicate)
    : position(0)
{
    if(root != nullptr){
        collect(root, predicate);
    }
}

void FilteredIterator::collect(WorkItem* node, const std::function<bool(const WorkItem*)>& predicate){
    if(node == nullptr)
        return;

    if(predicate(node)){
        matches.push_back(node);
    }

    for(int i = 0; i < node->getChildCount(); ++i){
        collect(node->getChild(i), predicate);
    }
}

bool FilteredIterator::hasNext() const{
    return position < matches.size();
}

WorkItem* FilteredIterator::next(){
    if(!hasNext())
        return nullptr;

    return matches[position++];
}
