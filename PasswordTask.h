#ifndef PASSWORD_H
#define PASSWORD_H

#include "WorkItem.h"

#include <string>
#include <functional>

class WorkItemIterator;

/**
 * @brief Represents an individual password development task.
 *
 * Password is a leaf in the Composite pattern and stores
 * requirements related to password security and validation.
*/
class PasswordTask : public WorkItem{
    private:
        /**
         * @brief Hashing algorithm used to securely store passwords.
        */
        std::string hashAlgorithm;

        /**
         * @brief Minimum number of characters required for a password.
        */
        int minimumLength;

        /**
         * @brief Indicates whether a special character is required.
        */
        bool requiresSpecialCharacter;

    public:
        /**
         * @brief Creates a password development task.
         * @param name The name of the task.
         * @param hashAlgorithm The hashing algorithm used for passwords.
         * @param minimumLength The minimum required password length.
         * @param requiresSpecialCharacter Whether a special character is required.
        */
        PasswordTask(const std::string& name, const std::string& hashAlgorithm, int minimumLength, bool requiresSpecialCharacter);

        /**
         * @brief Executes the password development task.
        */
        void execute() override;

        /**
         * @brief Returns a description of the password task.
         * @return A description containing the password requirements.
        */
        std::string getDescription() const override;

        /**
         * @brief Returns the current lifecycle status of the task.
         * @return The current task status.
        */
        std::string getStatus() const override;

        /**
         * @brief Starts the password task.
         * @return True if the task was successfully started.
        */
        bool start() override;

        /**
         * @brief Blocks the password task.
         * @return True if the task was successfully blocked.
        */
        bool block() override;

        /**
         * @brief Completes the password task.
         * @return True if the task was successfully completed.
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
         * @brief Destroys the password task.
        */
        ~PasswordTask() override = default;
};

#endif // PASSWORD_H
