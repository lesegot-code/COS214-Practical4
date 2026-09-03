#ifndef UITASK_H
#define UITASK_H

#include "WorkItem.h"

#include <string>

/**
 * @brief Represents an individual user-interface development task.
*/
class UITask : public WorkItem {
    private:
        /**
         * @brief UI framework used by the task.
        */
        std::string framework;

        /**
         * @brief Screen or component being developed.
        */
        std::string screen;

        /**
         * @brief Indicates whether responsive behaviour is required.
        */
        bool requiresResponsiveDesign;

    public:
        /**
         * @brief Creates a UI development task.
         * @param name Name of the task.
         * @param framework UI framework used.
         * @param screen Screen or component being developed.
         * @param requiresResponsiveDesign Whether responsive design is required.
        */
        UITask(const std::string& name, const std::string& framework, const std::string& screen, bool requiresResponsiveDesign);

        /**
         * @brief Executes the UI development task.
        */
        void execute() override;

        /**
         * @brief Returns a description of the UI task.
         * @return The task description.
        */
        std::string getDescription() const override;

        /**
         * @brief Returns the current lifecycle status.
         * @return The task status.
        */
        std::string getStatus() const override;

        /**
         * @brief Starts the UI task.
         * @return True if the task is started.
        */
        bool start() override;

        /**
         * @brief Blocks the UI task.
         * @return True if the task is blocked.
        */
        bool block() override;

        /**
         * @brief Completes the UI task.
         * @return True if the task is completed.
        */
        bool complete() override;

        /**
         * @brief Destroys the UI task.
        */
        ~UITask() override = default;
};

#endif
