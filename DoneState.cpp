#include "DoneState.h"
#include "WorkItem.h"

bool DoneState::start(WorkItem& item){
    (void)item; // transition rejected, Done is terminal
    return false;
}

bool DoneState::block(WorkItem& item){
    (void)item; // transition rejected, Done is terminal
    return false;
}

bool DoneState::complete(WorkItem& item){
    (void)item; // transition rejected, Done is terminal
    return false;
}

std::string DoneState::getName() const{
    return "Done";
}
