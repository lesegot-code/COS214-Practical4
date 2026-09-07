#ifndef DONESTATE_H
#define DONESTATE_H

#include "WorkState.h"

/**
 * @brief Concrete State representing a finished work item.
 *
 * Done is a terminal state. No further transitions are valid from here;
 * a completed task cannot be restarted, blocked, or completed again.
*/
class DoneState : public WorkState{
    public:
        /**
         * @brief Rejects starting a task that is already done.
         * @param item The work item (unused, transition is invalid).
         * @return False; Done is terminal.
        */
        bool start(WorkItem& item) override;

        /**
         * @brief Rejects blocking a task that is already done.
         * @param item The work item (unused, transition is invalid).
         * @return False; Done is terminal.
        */
        bool block(WorkItem& item) override;

        /**
         * @brief Rejects completing a task that is already done.
         * @param item The work item (unused, transition is invalid).
         * @return False; Done is terminal.
        */
        bool complete(WorkItem& item) override;

        /**
         * @brief Returns the name of this state.
         * @return "Done".
        */
        std::string getName() const override;
};

#endif // DONESTATE_H
