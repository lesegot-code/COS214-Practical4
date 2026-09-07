#ifndef FILTEREDITERATOR_H
#define FILTEREDITERATOR_H

#include "WorkItemIterator.h"

#include <functional>
#include <vector>

class WorkItem;

/**
 * @brief Concrete Iterator that visits only WorkItems matching a predicate.
 *
 * Differs from a full traversal in selection rule: FilteredIterator walks the
 * entire hierarchy but only yields items for which the predicate holds (e.g.
 * items currently in the Blocked state). This is typically used to surface
 * items that need attention, rather than the whole tree.
 *
 * Traversal-modification policy: this iterator takes a SNAPSHOT of matching
 * items at construction time. Structural or state changes made to the
 * hierarchy after the iterator is created do not affect an iteration already
 * in progress - the iterator keeps yielding whatever matched at the moment
 * it was built. This avoids dangling pointers and unpredictable behaviour if
 * the hierarchy is modified mid-traversal.
*/
class FilteredIterator : public WorkItemIterator{
    private:
        /**
         * @brief The work items that matched the predicate, captured at construction.
        */
        std::vector<WorkItem*> matches;

        /**
         * @brief Current position of the traversal within matches.
        */
        std::size_t position;

        /**
         * @brief Recursively walks the hierarchy collecting matching items.
         *
         * Uses only the public WorkItem interface (getChildCount/getChild),
         * never the internal container, so client construction of this
         * iterator never needs to see how a composite stores its children.
         * @param node The node currently being visited.
         * @param predicate The condition an item must satisfy to be included.
        */
        void collect(WorkItem* node, const std::function<bool(const WorkItem*)>& predicate);

    public:
        /**
         * @brief Builds a snapshot traversal over items matching a predicate.
         * @param root The root of the hierarchy to search (may be a leaf or a group).
         * @param predicate The condition an item must satisfy to be yielded.
        */
        FilteredIterator(WorkItem* root, const std::function<bool(const WorkItem*)>& predicate);

        /**
         * @brief Checks whether there is another matching item left to visit.
         * @return True if next() can still be called.
        */
        bool hasNext() const override;

        /**
         * @brief Returns the next matching item and advances.
         * @return The next matching work item, or nullptr if none remain.
        */
        WorkItem* next() override;
};

#endif // FILTEREDITERATOR_H
