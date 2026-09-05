#ifndef INPROGRESSSTATE_H
#define INPROGRESSSTATE_H

#include "WorkState.h"

/**
 * @brief Concrete State representing a work item that is actively being worked on.
 *
 * An InProgress task may be blocked or completed. It cannot be "started"
 * again, since it is already running.
*/
class InProgressState : public WorkState{
    public:
        /**
         * @brief Rejects starting a task that is already in progress.
         * @param item The work item (unused, transition is invalid).
         * @return False; an in-progress task is already started.
        */
        bool start(WorkItem& item) override;

        /**
         * @brief Blocks the task, moving it to Blocked.
         * @param item The work item to transition.
         * @return True; blocking an in-progress task is always valid.
        */
        bool block(WorkItem& item) override;

        /**
         * @brief Completes the task, moving it to Done.
         * @param item The work item to transition.
         * @return True; completing an in-progress task is always valid.
        */
        bool complete(WorkItem& item) override;

        /**
         * @brief Returns the name of this state.
         * @return "InProgress".
        */
        std::string getName() const override;
};

#endif // INPROGRESSSTATE_H
