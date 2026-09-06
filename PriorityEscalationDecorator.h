#ifndef PRIORITYESCALATIONDECORATOR_H
#define PRIORITYESCALATIONDECORATOR_H

#include "WorkItemDecorator.h"

#include <string>

/**
 * @brief Concrete Decorator adding priority escalation to a work item.
 *
 * A PriorityEscalationDecorator wraps a work item and marks it with a
 * priority level (e.g. "High", "Critical"). Escalating an item changes how
 * it is described and how execution is reported, without altering the
 * underlying leaf or composite class. Can be stacked with other decorators
 * such as CodeReviewDecorator.
*/
class PriorityEscalationDecorator : public WorkItemDecorator{
    private:
        /**
         * @brief Current priority level assigned to the wrapped item.
        */
        std::string priorityLevel;

    public:
        /**
         * @brief Wraps a work item with an initial priority level.
         * @param wrapped The work item to decorate. Must not be null.
         * @param priorityLevel The initial priority level (e.g. "High").
        */
        PriorityEscalationDecorator(WorkItem* wrapped, const std::string& priorityLevel);

        /**
         * @brief Escalates (or changes) the priority level at runtime.
         * @param newPriorityLevel The new priority level.
        */
        void escalate(const std::string& newPriorityLevel);

        /**
         * @brief Returns the current priority level.
         * @return The priority level.
        */
        std::string getPriorityLevel() const;

        /**
         * @brief Executes the wrapped item, announcing its priority level first.
        */
        void execute() override;

        /**
         * @brief Returns the wrapped item's description annotated with priority.
         * @return The annotated description.
        */
        std::string getDescription() const override;

        /**
         * @brief Destroys the decorator.
        */
        ~PriorityEscalationDecorator() override = default;
};

#endif // PRIORITYESCALATIONDECORATOR_H
