#ifndef PROFILETASK_H
#define PROFILETASK_H

#include "WorkItem.h"

#include <string>
#include <functional>

class WorkItemIterator;

/**
 * @brief Represents an individual user profile development task.
 *
 * ProfileTask is a leaf in the Composite pattern and stores
 * requirements related to user profile functionality.
*/
class ProfileTask : public WorkItem{
    private:
        /**
         * @brief Number of profile fields to be implemented.
        */
        int profileFields;

        /**
         * @brief Indicates whether avatar functionality is required.
        */
        bool supportsAvatar;

        /**
         * @brief Indicates whether privacy settings are required.
        */
        bool requiresPrivacySettings;

    public:
        /**
         * @brief Creates a profile development task.
         * @param name Name of the task.
         * @param profileFields Number of profile fields to implement.
         * @param supportsAvatar Whether avatar functionality is required.
         * @param requiresPrivacySettings Whether privacy settings are required.
        */
        ProfileTask(const std::string& name, int profileFields, bool supportsAvatar, bool requiresPrivacySettings);

        /**
         * @brief Executes the profile development task.
        */
        void execute() override;

        /**
         * @brief Returns a description of the profile task.
         * @return The task description.
        */
        std::string getDescription() const override;

        /**
         * @brief Returns the current lifecycle status.
         * @return The task status.
        */
        std::string getStatus() const override;

        /**
         * @brief Starts the profile task.
         * @return True if the task is started.
        */
        bool start() override;

        /**
         * @brief Blocks the profile task.
         * @return True if the task is blocked.
        */
        bool block() override;

        /**
         * @brief Completes the profile task.
         * @return True if the task is completed.
        */
        bool complete() override;

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
         * @brief Destroys the profile task.
        */
        ~ProfileTask() override = default;
};

#endif // PROFILETASK_H
