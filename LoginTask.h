#ifndef LOGINTASK_H
#define LOGINTASK_H

#include "WorkItem.h"

#include <string>

/**
 * @brief Represents an individual login development task.
 *
 * LoginTask is a leaf in the Composite pattern and stores
 * information specific to implementing user authentication.
*/
class LoginTask: public WorkItem{
    private:
        /** 
         * @brief Authentication method used by the login system.
        */
        std::string authenticationMethod;

        /** 
         * @brief Indicates whether two-factor authentication is required.
        */
        bool requiresTwoFactor;

    public:
        /**
         * @brief Creates a login development task.
         * @param name The name of the task.
         * @param authenticationMethod The authentication method used.
         * @param requiresTwoFactor Whether two-factor authentication is required.
        */
        LoginTask(const std::string& name, const std::string& authenticationMethod, bool requiresTwoFactor);

        /**
         * @brief Executes the login development task.
        */
        void execute() override;

        /**
         * @brief Returns a description of the login task.
         * @return The task description.
        */
        std::string getDescription() const override;

        /**
         * @brief Returns the current lifecycle status of the task.
         * @return The current task status.
        */
        std::string getStatus() const override;

        /**
         * @brief Starts the login task.
         * @return True if the task was successfully started.
        */
        bool start() override;

        /**
         * @brief Blocks the login task.
         * @return True if the task was successfully blocked.
        */
        bool block() override;

        /**
         * @brief Completes the login task.
         * @return True if the task was successfully completed.
        */
        bool complete() override;

        /**
         * @brief Destroys the login task.
        */
        ~LoginTask() override = default;
};

#endif // LOGINTASK_H
