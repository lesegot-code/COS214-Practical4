#include "BlockedState.h"
#include "InProgressState.h"
#include "WorkItem.h"

bool BlockedState::start(WorkItem& item){
    item.setState(new InProgressState());
    return true;
}

bool BlockedState::block(WorkItem& item){
    (void)item; // transition rejected, item is left unchanged
    return false;
}

bool BlockedState::complete(WorkItem& item){
    (void)item; // transition rejected, item is left unchanged
    return false;
}

std::string BlockedState::getName() const{
    return "Blocked";
}
