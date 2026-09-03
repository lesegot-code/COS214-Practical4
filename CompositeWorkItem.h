#ifndef BACKEND_H
#define BACKEND_H

#include "WorkItem.h"

#include <string>
#include <vector>

/**
 * @brief Represents the backend group (composite) in the software project hierarchy.
 *
 * A Backend can contain individual work items and nested groups.
*/
class Backend : public WorkItem{
    private:
        /** 
         * @brief Work items contained directly in this backend group. 
        */
        std::vector<WorkItem*> children;

    public:
        /**
         * @brief Creates a backend group.
         * @param name The name of the backend group.
        */
        explicit Backend(const std::string& name);

        /**
         * @brief Executes all work items contained in the backend group.
        */
        void execute() override;

        /**
         * @brief Returns a description of the backend group.
         * @return A description of the backend group.
        */
        std::string getDescription() const override;

        /**
         * @brief Returns the status of the backend group.
         * @return The current status of the backend group.
        */
        std::string getStatus() const override;

        /**
         * @brief Adds a work item to the backend group.
         * @param item The work item to add.
         * @return True if the item was added, otherwise false.
        */
        bool add(WorkItem* item) override;

        /**
         * @brief Removes a work item from the backend group.
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
         * @brief Destroys the backend group and its owned children.
        */
        ~Backend() override;
};

#endif // BACKEND_H
