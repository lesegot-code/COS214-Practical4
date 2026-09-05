#include "InProgressState.h"
#include "BlockedState.h"
#include "DoneState.h"
#include "WorkItem.h"

bool InProgressState::start(WorkItem& item){
    (void)item; // transition rejected, item is left unchanged
    return false;
}

bool InProgressState::block(WorkItem& item){
    item.setState(new BlockedState());
    return true;
}

bool InProgressState::complete(WorkItem& item){
    item.setState(new DoneState());
    return true;
}

std::string InProgressState::getName() const{
    return "InProgress";
}
