#ifndef WORKSTATE_H
#define WORKSTATE_H

#include <string>

class WorkItem; // forward declaration to avoid a circular include with WorkItem.h

/**
 * @brief Abstract State in the State pattern.
 *
 * Represents one lifecycle stage of a WorkItem leaf (e.g. LoginTask,
 * PasswordTask). Concrete states decide what happens on start/block/complete
 * and whether a transition is valid, and hand control to the WorkItem to
 * move it into its next state.
*/
class WorkState{
    public:
        /**
         * @brief Attempts to start the work item from this state.
         * @param item The work item whose state is transitioning.
         * @return True if the transition was valid and applied, false otherwise.
        */
        virtual bool start(WorkItem& item) = 0;

        /**
         * @brief Attempts to block the work item from this state.
         * @param item The work item whose state is transitioning.
         * @return True if the transition was valid and applied, false otherwise.
        */
        virtual bool block(WorkItem& item) = 0;

        /**
         * @brief Attempts to complete the work item from this state.
         * @param item The work item whose state is transitioning.
         * @return True if the transition was valid and applied, false otherwise.
        */
        virtual bool complete(WorkItem& item) = 0;

        /**
         * @brief Returns the human-readable name of this state.
         * @return The state's name (e.g. "Pending", "InProgress").
        */
        virtual std::string getName() const = 0;

        /**
         * @brief Destroys the state.
        */
        virtual ~WorkState() = default;
};

#endif // WORKSTATE_H
