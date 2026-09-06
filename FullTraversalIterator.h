#ifndef FULLTRAVERSALITERATOR_H
#define FULLTRAVERSALITERATOR_H

#include "WorkItemIterator.h"

#include <vector>

class WorkItem;

/**
 * @brief Concrete Iterator that visits every WorkItem in the hierarchy.
 *
 * Performs a depth-first traversal of the WorkItem hierarchy and captures
 * a snapshot of all items at construction time.
 *
 * Structural changes or state changes made to the hierarchy after the
 * iterator is created do not affect the traversal already in progress.
*/
class FullTraversalIterator: public WorkItemIterator{
    private:
        /**
         * @brief All work items captured during construction.
        */
        std::vector<WorkItem*> items;

        /**
         * @brief Current position within the traversal.
        */
        std::size_t position;

        /**
         * @brief Recursively walks the hierarchy and collects every item.
         *
         * @param node The node currently being visited.
        */
        void collect(WorkItem* node);

    public:
        /**
         * @brief Builds a snapshot traversal over the entire hierarchy.
         *
         * @param root The root of the hierarchy to traverse.
        */
        explicit FullTraversalIterator(WorkItem* root);

        /**
         * @brief Checks whether another item remains.
         *
         * @return True if next() can still be called.
        */
        bool hasNext() const override;

        /**
         * @brief Returns the next item and advances the iterator.
         *
         * @return The next work item, or nullptr if none remain.
        */
        WorkItem* next() override;
};

#endif // FULLTRAVERSALITERATOR_H
