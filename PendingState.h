#ifndef PENDINGSTATE_H
#define PENDINGSTATE_H

#include "WorkState.h"

/**
 * @brief Concrete State representing a work item that has not started yet.
 *
 * A Pending task may only be started. Blocking or completing a task
 * before it has started is not a meaningful transition.
*/
class PendingState : public WorkState{
    public:
        /**
         * @brief Starts the task, moving it to InProgress.
         * @param item The work item to transition.
         * @return True; starting a pending task is always valid.
        */
        bool start(WorkItem& item) override;

        /**
         * @brief Rejects blocking a task that has not started.
         * @param item The work item (unused, transition is invalid).
         * @return False; a pending task cannot be blocked directly.
        */
        bool block(WorkItem& item) override;

        /**
         * @brief Rejects completing a task that has not started.
         * @param item The work item (unused, transition is invalid).
         * @return False; a pending task cannot be completed directly.
        */
        bool complete(WorkItem& item) override;

        /**
         * @brief Returns the name of this state.
         * @return "Pending".
        */
        std::string getName() const override;
};

#endif // PENDINGSTATE_H
