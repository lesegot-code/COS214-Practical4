#ifndef WORKITEMDECORATOR_H
#define WORKITEMDECORATOR_H

#include "WorkItem.h"

#include <string>
#include <functional>

class WorkItemIterator;

/**
 * @brief Abstract Decorator in the Decorator pattern.
 *
 * Wraps a WorkItem (leaf or composite) and forwards every operation to it
 * by default, so a decorated item remains usable anywhere a plain WorkItem
 * is expected. Concrete decorators override only the operations they need
 * in order to add a responsibility, and decorators may be stacked by
 * wrapping one decorator inside another.
 *
 * Ownership: a WorkItemDecorator owns the WorkItem it wraps and destroys it
 * when the decorator itself is destroyed.
*/
class WorkItemDecorator : public WorkItem{
    protected:
        /**
         * @brief The work item being decorated. Owned by this decorator.
        */
        WorkItem* wrapped;

    public:
        /**
         * @brief Wraps an existing work item, adopting its name.
         * @param wrapped The work item to decorate. Must not be null.
        */
        explicit WorkItemDecorator(WorkItem* wrapped);

        /**
         * @brief Forwards execution to the wrapped item.
        */
        void execute() override;

        /**
         * @brief Forwards the description request to the wrapped item.
         * @return The wrapped item's description.
        */
        std::string getDescription() const override;

        /**
         * @brief Forwards the status request to the wrapped item.
         * @return The wrapped item's current lifecycle status.
        */
        std::string getStatus() const override;

        /**
         * @brief Forwards the start transition to the wrapped item.
         * @return True if the wrapped item was successfully started.
        */
        bool start() override;

        /**
         * @brief Forwards the block transition to the wrapped item.
         * @return True if the wrapped item was successfully blocked.
        */
        bool block() override;

        /**
         * @brief Forwards the complete transition to the wrapped item.
         * @return True if the wrapped item was successfully completed.
        */
        bool complete() override;

        /**
         * @brief Forwards add() to the wrapped item (only meaningful if it is a composite).
         * @param item The work item to add.
         * @return True if the wrapped item accepted the child.
        */
        bool add(WorkItem* item) override;

        /**
         * @brief Forwards remove() to the wrapped item (only meaningful if it is a composite).
         * @param item The work item to remove.
         * @return True if the wrapped item removed the child.
        */
        bool remove(WorkItem* item) override;

        /**
         * @brief Forwards the child count request to the wrapped item.
         * @return The wrapped item's number of direct children.
        */
        int getChildCount() const override;

        /**
         * @brief Forwards the child lookup to the wrapped item.
         * @param pos The position of the child.
         * @return The child at the given position, or nullptr if invalid.
        */
        WorkItem* getChild(int pos) const override;

        /**
         * @brief Forwards the child index lookup to the wrapped item.
         * @param item The child to find.
         * @return The child's position, or -1 if not found.
        */
        int getChildIndex(const WorkItem* item) const override;

        /**
         * @brief Returns the work item this decorator wraps.
         *
         * Used to reach the innermost component through a stack of
         * decorators, e.g. for identity checks.
         * @return The wrapped work item.
        */
        WorkItem* getWrapped() const;

        /**
         * @brief Creates a full traversal iterator for this hierarchy.
         *
         * @return A new iterator that traverses the composite hierarchy.
        */
        virtual WorkItemIterator* createIterator() override;

        /**
         * @brief Creates a filtered iterator for this hierarchy.
         *
         * @param predicate The condition that determines which items are included.
         * @return A new iterator that visits matching work items.
        */
        virtual WorkItemIterator* createFilteredIterator(const std::function<bool(const WorkItem*)>& predicate) override;

        /**
         * @brief Destroys the decorator and the work item it wraps.
        */
        ~WorkItemDecorator() override;
};

#endif // WORKITEMDECORATOR_H
