#ifndef BLOCKEDSTATE_H
#define BLOCKEDSTATE_H

#include "WorkState.h"

/**
 * @brief Concrete State representing a work item that is stalled.
 *
 * A Blocked task may be resumed via start(), which returns it to
 * InProgress. It cannot be blocked again or completed while blocked.
*/
class BlockedState : public WorkState{
    public:
        /**
         * @brief Resumes the task, moving it back to InProgress.
         * @param item The work item to transition.
         * @return True; resuming a blocked task is always valid.
        */
        bool start(WorkItem& item) override;

        /**
         * @brief Rejects blocking a task that is already blocked.
         * @param item The work item (unused, transition is invalid).
         * @return False; a blocked task is already blocked.
        */
        bool block(WorkItem& item) override;

        /**
         * @brief Rejects completing a task while it is blocked.
         * @param item The work item (unused, transition is invalid).
         * @return False; a blocked task must be resumed before it can complete.
        */
        bool complete(WorkItem& item) override;

        /**
         * @brief Returns the name of this state.
         * @return "Blocked".
        */
        std::string getName() const override;
};

#endif // BLOCKEDSTATE_H
