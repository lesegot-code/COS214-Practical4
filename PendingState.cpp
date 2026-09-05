#include "PendingState.h"
#include "InProgressState.h"
#include "WorkItem.h"

bool PendingState::start(WorkItem& item){
    item.setState(new InProgressState());
    return true;
}

bool PendingState::block(WorkItem& item){
    (void)item; // transition rejected, item is left unchanged
    return false;
}

bool PendingState::complete(WorkItem& item){
    (void)item; // transition rejected, item is left unchanged
    return false;
}

std::string PendingState::getName() const{
    return "Pending";
}
