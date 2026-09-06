#ifndef COMPOSITEWORKITEM_H
#define COMPOSITEWORKITEM_H

#include "WorkItem.h"

#include <string>
#include <vector>
#include <functional>

class WorkItemIterator;

/**
 * @brief Represents the CompositeWorkItem group (composite) in the software project hierarchy.
 *
 * A CompositeWorkItem can contain individual work items and nested groups.
*/
class CompositeWorkItem : public WorkItem{
    private:
        /** 
         * @brief Work items contained directly in this CompositeWorkItem group. 
        */
        std::vector<WorkItem*> children;

    public:
        /**
         * @brief Creates a CompositeWorkItem group.
         * @param name The name of the CompositeWorkItem group.
        */
        explicit CompositeWorkItem(const std::string& name);

        /**
         * @brief Executes all work items contained in the CompositeWorkItem group.
        */
        void execute() override;

        /**
         * @brief Returns a description of the CompositeWorkItem group.
         * @return A description of the CompositeWorkItem group.
        */
        std::string getDescription() const override;

        /**
         * @brief Returns the status of the CompositeWorkItem group.
         * @return The current status of the CompositeWorkItem group.
        */
        std::string getStatus() const override;

        /**
         * @brief Adds a work item to the CompositeWorkItem group.
         * @param item The work item to add.
         * @return True if the item was added, otherwise false.
        */
        bool add(WorkItem* item) override;

        /**
         * @brief Removes a work item from the CompositeWorkItem group.
         * @param item The work item to remove.
         * @return True if the item was removed, otherwise false.
        */
        bool remove(WorkItem* item) override;

        /**
         * @brief Returns the number of direct children.
         * @return The number of direct children.
        */
        int getChildCount() const override;

        /**
         * @brief Returns a direct child by position.
         * @param pos The position of the child.
         * @return The child at the given position, or nullptr if invalid.
        */
        WorkItem* getChild(int pos) const override;

        /**
         * @brief Finds the position of a direct child.
         * @param item The child to find.
         * @return The child's position, or -1 if not found.
        */
        int getChildIndex(const WorkItem* item) const override;

        /**
         * @brief Removes a child without destroying it.
         *
         * This supports transferring ownership between composites.
        */
        WorkItem* detach(WorkItem* item);

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
        virtual WorkItemIterator* createFilteredIterator(const std::function<bool(const WorkItem*)>& predicate);
    
        /**
         * @brief Destroys the CompositeWorkItem group and its owned children.
        */
        ~CompositeWorkItem() override;
};

#endif // COMPOSITEWORKITEM_H
