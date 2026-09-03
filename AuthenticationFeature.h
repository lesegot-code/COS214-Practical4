#ifndef AUTHENTICATIONFEATURE_H
#define AUTHENTICATIONFEATURE_H

#include "WorkItem.h"

#include <string>
#include <vector>

/**
 * @brief Represents the AuthenticationFeature group (composite) in the software project hierarchy.
 *
 * A AuthenticationFeature can contain individual work items and nested groups.
*/
class AuthenticationFeature : public WorkItem{
    private:
        /** 
         * @brief Work items contained directly in this AuthenticationFeature group. 
        */
        std::vector<WorkItem*> children;

    public:
        /**
         * @brief Creates a AuthenticationFeature group.
         * @param name The name of the AuthenticationFeature group.
        */
        explicit AuthenticationFeature(const std::string& name);

        /**
         * @brief Executes all work items contained in the AuthenticationFeature group.
        */
        void execute() override;

        /**
         * @brief Returns a description of the AuthenticationFeature group.
         * @return A description of the AuthenticationFeature group.
        */
        std::string getDescription() const override;

        /**
         * @brief Returns the status of the AuthenticationFeature group.
         * @return The current status of the AuthenticationFeature group.
        */
        std::string getStatus() const override;

        /**
         * @brief Adds a work item to the AuthenticationFeature group.
         * @param item The work item to add.
         * @return True if the item was added, otherwise false.
        */
        bool add(WorkItem* item) override;

        /**
         * @brief Removes a work item from the AuthenticationFeature group.
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
         * @brief Destroys the AuthenticationFeature group and its owned children.
        */
        ~AuthenticationFeature() override;
};

#endif // AUTHENTICATIONFEATURE_H
