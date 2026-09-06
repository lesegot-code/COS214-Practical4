#ifndef CODEREVIEWDECORATOR_H
#define CODEREVIEWDECORATOR_H

#include "WorkItemDecorator.h"

#include <string>

/**
 * @brief Concrete Decorator adding a mandatory code review gate.
 *
 * A CodeReviewDecorator wraps a work item and refuses to complete() it
 * until the item has been explicitly approved via approveReview(). This is
 * the "validation" responsibility from the practical spec, and it can be
 * stacked with other decorators (e.g. PriorityEscalationDecorator) without
 * either one knowing about the other.
*/
class CodeReviewDecorator : public WorkItemDecorator{
    private:
        /**
         * @brief Whether a reviewer has approved the wrapped item.
        */
        bool reviewed;

        /**
         * @brief Name of the reviewer who approved the item, if any.
        */
        std::string reviewer;

    public:
        /**
         * @brief Wraps a work item with a mandatory review requirement.
         * @param wrapped The work item to decorate. Must not be null.
        */
        explicit CodeReviewDecorator(WorkItem* wrapped);

        /**
         * @brief Records that a reviewer has approved the wrapped item.
         * @param reviewer The name of the approving reviewer.
        */
        void approveReview(const std::string& reviewer);

        /**
         * @brief Checks whether the wrapped item has been reviewed.
         * @return True if approveReview() has been called.
        */
        bool isReviewed() const;

        /**
         * @brief Executes the wrapped item, warning first if unreviewed.
        */
        void execute() override;

        /**
         * @brief Returns the wrapped item's description annotated with review status.
         * @return The annotated description.
        */
        std::string getDescription() const override;

        /**
         * @brief Completes the wrapped item only if it has been reviewed.
         * @return True if reviewed and the wrapped item completed successfully,
         *         false if review is still outstanding or the transition was invalid.
        */
        bool complete() override;

        /**
         * @brief Destroys the decorator.
        */
        ~CodeReviewDecorator() override = default;
};

#endif // CODEREVIEWDECORATOR_H
