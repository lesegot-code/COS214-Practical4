#ifndef WORKITEMITERATOR_H
#define WORKITEMITERATOR_H

class WorkItem;

/**
 * @brief Abstract Iterator in the Iterator pattern.
 *
 * Shared by every traversal over a WorkItem hierarchy (e.g. FullTraversalIterator,
 * FilteredIterator) so client code can traverse without knowing which concrete
 * traversal it holds, and without ever touching the aggregate's internal container.
*/
class WorkItemIterator{
    public:
        /**
         * @brief Checks whether there is another item left to visit.
         * @return True if next() can still be called.
        */
        virtual bool hasNext() const = 0;

        /**
         * @brief Returns the next item in the traversal and advances.
         * @return The next work item, or nullptr if none remain.
        */
        virtual WorkItem* next() = 0;

        /**
         * @brief Destroys the iterator.
        */
        virtual ~WorkItemIterator() = default;
};

#endif // WORKITEMITERATOR_H
