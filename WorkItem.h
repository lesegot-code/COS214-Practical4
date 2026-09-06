#ifndef WORKITEM_H
#define WORKITEM_H

#include <string>

class WorkState; // forward declaration to avoid a circular include with WorkState.h
class WorkItemIterator;

/**
 * @brief Abstract Component in the Composite pattern.
 *
 * Represents either an individual work item or a group of work items
 * in the software development hierarchy.
*/
class WorkItem{
    private:
        /** 
         * @brief Next ID available for a new work item.
        */
        static int nextId;

    protected:
        /** 
         * @brief Unique identifier for this work item.
        */
        const int id;

        /** 
         * @brief Name of the work item.
        */
        std::string name;

        /**
         * @brief Current lifecycle state of the work item (State pattern).
         *
         * Only meaningful for leaves; composites do not own a state of
         * their own and derive their status from their children instead.
        */
        WorkState* state;

    public:
        /**
         * @brief Creates a work item with a unique ID and name.
         * @param name The name of the work item.
        */
        explicit WorkItem(const std::string& name);

        /**
         * @brief Returns the work item's ID.
         * @return The work item's unique identifier.
        */
        int getId() const;

        /**
         * @brief Returns the work item's name.
         * @return The work item's name.
        */
        std::string getName() const;

        /**
         * @brief Executes the work item.
        */
        virtual void execute() = 0;

        /**
         * @brief Returns a description of the work item.
         * @return The work item's description.
        */
        virtual std::string getDescription() const = 0;

        /**
         * @brief Returns the current lifecycle status.
         * @return The current status.
        */
        virtual std::string getStatus() const = 0;

        /**
         * @brief Starts the work item.
         * @return True if the work item was successfully started.
        */
        virtual bool start();

        /**
         * @brief Blocks the work item.
         * @return True if the work item was successfully blocked.
        */
        virtual bool block();

        /**
         * @brief Completes the work item.
         * @return True if the work item was successfully completed.
        */
        virtual bool complete();

        /**
         * @brief Adds a child work item.
         * @param item The work item to add.
         * @return False because leaves do not support children.
        */
        virtual bool add(WorkItem* item);

        /**
         * @brief Removes a child work item.
         * @param item The work item to remove.
         * @return False because leaves do not support children.
        */
        virtual bool remove(WorkItem* item);

        /**
         * @brief Returns the number of direct children.
         * @return Zero because leaves do not contain children.
        */
        virtual int getChildCount() const;

        /**
         * @brief Returns a direct child at the given position.
         * @param pos The position of the child.
         * @return The child at the position, or nullptr if invalid.
        */
        virtual WorkItem* getChild(int pos) const;

        /**
         * @brief Finds the position of a direct child.
         * @param item The child to find.
         * @return The child's position, or -1 if it is not found.
        */
        virtual int getChildIndex(const WorkItem* item) const;

        /**
         * @brief Replaces the work item's current state, deleting the old one.
         *
         * Called by WorkState subclasses to drive a valid transition. The
         * work item owns its state; this method is where that ownership is
         * enforced.
         * @param newState The state to transition into.
        */
        void setState(WorkState* newState);

        /**
         * @brief Creates an iterator for traversing this work item hierarchy.
         *
         * @return A concrete iterator for this work item.
        */
        virtual WorkItemIterator* createIterator();

        /**
         * @brief Destroys the work item and its owned state, if any.
        */
        virtual ~WorkItem();
};

#endif // WORKITEM_H
