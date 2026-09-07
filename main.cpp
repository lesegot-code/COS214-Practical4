// TaskForge - main.cpp (DRAFT for Lesego's review)
//
// Integrates Composite (Lesego), State + Iterator (Melaney/Lesego:
// FilteredIterator + FullTraversalIterator, both created via
// WorkItem::createIterator()/createFilteredIterator()) and Decorator
// (Ajba) into one running demo.
//
// This also proposes one small addition, needed for the runtime-
// modification scenario below: WorkItem::detach() is promoted from a
// CompositeWorkItem-only method to a virtual base method (see
// WorkItem.h/.cpp and the "override" added in CompositeWorkItem.h), and
// forwarded through WorkItemDecorator (Ajba's file). Please review
// before merging - happy to adjust if you'd rather do this differently.
//
// Story: a small software team is shipping an authentication and
// dashboard feature set. Two scenarios run back to back:
//   1) A login task goes through code review and gets escalated.
//   2) A task is moved between groups, and a decorator is attached to,
//      then removed from, a task while the program is running.

#include "WorkItem.h"
#include "CompositeWorkItem.h"
#include "LoginTask.h"
#include "PasswordTask.h"
#include "ProfileTask.h"
#include "UITask.h"
#include "WorkItemDecorator.h"
#include "CodeReviewDecorator.h"
#include "PriorityEscalationDecorator.h"
#include "WorkItemIterator.h"

#include <iostream>
#include <string>

static void printHeading(const std::string& title){
    std::cout << std::endl << "=== " << title << " ===" << std::endl;
}

static bool isBlocked(const WorkItem* item){
    return item->getChildCount() == 0 && item->getStatus() == "Blocked";
}

static bool isDone(const WorkItem* item){
    return item->getChildCount() == 0 && item->getStatus() == "Done";
}

// Prints every leaf task using a full traversal built via the Factory
// Method on WorkItem, rather than constructing FullTraversalIterator
// directly - this is what keeps client code decoupled from which
// concrete iterator a given WorkItem hands back.
static void printAllItems(WorkItem* root, const std::string& label){
    std::cout << label << ":" << std::endl;
    WorkItemIterator* it = root->createIterator();
    while(it->hasNext()){
        WorkItem* item = it->next();
        if(item->getChildCount() == 0){
            std::cout << "  - " << item->getDescription() << " [" << item->getStatus() << "]" << std::endl;
        }
    }
    delete it;
}

// Scenario 1: a login task moves through code review and gets escalated.
static void runReviewAndEscalationScenario(WorkItem* traversalRoot, WorkItem* loginItem, WorkItem* passwordItem){
    printHeading("Scenario 1: Code review and priority escalation");

    CodeReviewDecorator* reviewedLogin = dynamic_cast<CodeReviewDecorator*>(loginItem);

    reviewedLogin->start();
    reviewedLogin->execute();

    std::cout << "Attempting to complete before review is approved..." << std::endl;
    if(!reviewedLogin->complete()){
        std::cout << "  -> blocked, as expected." << std::endl;
    }

    reviewedLogin->approveReview("Melaney");
    std::cout << "Review approved. Completing again..." << std::endl;
    if(reviewedLogin->complete()){
        std::cout << "  -> " << reviewedLogin->getDescription() << " is now " << reviewedLogin->getStatus() << "." << std::endl;
    }

    passwordItem->start();
    passwordItem->block();

    std::cout << std::endl << "Two independent filtered traversals over the same tree, side by side:" << std::endl;

    // Both iterators are built via the Factory Method (createFilteredIterator),
    // each with its own predicate/purpose, and advanced in an interleaved
    // order to show neither depends on the other's traversal state.
    WorkItemIterator* doneIterator = traversalRoot->createFilteredIterator(isDone);
    WorkItemIterator* blockedIterator = traversalRoot->createFilteredIterator(isBlocked);

    while(doneIterator->hasNext() || blockedIterator->hasNext()){
        if(doneIterator->hasNext()){
            std::cout << "  [Done traversal]    " << doneIterator->next()->getDescription() << std::endl;
        }
        if(blockedIterator->hasNext()){
            std::cout << "  [Blocked traversal] " << blockedIterator->next()->getDescription() << std::endl;
        }
    }

    delete doneIterator;
    delete blockedIterator;
}

// Scenario 2: the runtime-modification scenario - a task moves between
// groups, and a decorator is added to, then removed from, a task while
// the program is running (not just at construction time).
//
// Traversal-modification policy in action: FilteredIterator/FullTraversalIterator
// take a snapshot at construction, so a fresh iterator is created after
// each change below specifically to observe the new state - an iterator
// built before a change intentionally keeps showing the old snapshot.
static void runRuntimeModificationScenario(CompositeWorkItem* authFeature, CompositeWorkItem* dashboardFeature, WorkItem* uiItem){
    printHeading("Scenario 2: Runtime modification (move + live decoration)");

    std::cout << "Before: UI task lives under Dashboard Feature." << std::endl;
    printAllItems(dashboardFeature, "  Dashboard Feature contents");

    // --- Move an item between groups at runtime ---
    std::cout << std::endl << "Moving the UI task from Dashboard Feature into Authentication Feature..." << std::endl;
    WorkItem* movedUi = dashboardFeature->detach(uiItem);
    authFeature->add(movedUi);
    printAllItems(authFeature, "  Authentication Feature contents (after move)");

    // --- Add a decorator to an existing item at runtime (not at construction) ---
    std::cout << std::endl << "Escalating the UI task's priority while the system is running..." << std::endl;
    WorkItem* detachedForDecoration = authFeature->detach(movedUi);
    PriorityEscalationDecorator* escalatedUi = new PriorityEscalationDecorator(detachedForDecoration, "Critical");
    authFeature->add(escalatedUi);
    std::cout << "  -> " << escalatedUi->getDescription() << std::endl;

    // --- Remove a decorator from an item at runtime, keeping the item alive ---
    std::cout << std::endl << "Removing the escalation decorator at runtime (task keeps running undecorated)..." << std::endl;
    WorkItem* detachedShell = authFeature->detach(escalatedUi);
    PriorityEscalationDecorator* shellToStrip = dynamic_cast<PriorityEscalationDecorator*>(detachedShell);
    WorkItem* plainUi = shellToStrip->release();  // detach ownership before deleting the shell
    delete shellToStrip;                          // safe: release() already cleared its wrapped pointer
    authFeature->add(plainUi);
    std::cout << "  -> " << plainUi->getDescription() << " (decorator removed, task unaffected)" << std::endl;

    printAllItems(authFeature, "  Authentication Feature contents (final state)");
}

// Scenario 3: stacked decorators - both concrete decorators wrapping the
// same task at once, participating in normal system behaviour (not just
// the isolated decorator_test.cpp). Also exercises the fourth leaf type
// (ProfileTask) and the InProgress/Blocked states on it.
static void runStackedDecoratorScenario(CompositeWorkItem* profileFeature, WorkItem* profileItem){
    printHeading("Scenario 3: Stacked decorators (CodeReview + PriorityEscalation)");

    WorkItem* detachedProfile = profileFeature->detach(profileItem);
    CodeReviewDecorator* reviewedProfile = new CodeReviewDecorator(detachedProfile);
    PriorityEscalationDecorator* stackedProfile = new PriorityEscalationDecorator(reviewedProfile, "High");
    profileFeature->add(stackedProfile);

    std::cout << "  -> " << stackedProfile->getDescription() << std::endl;

    stackedProfile->start();
    std::cout << "Attempting to complete a stacked, unreviewed task..." << std::endl;
    if(!stackedProfile->complete()){
        std::cout << "  -> blocked by the inner CodeReviewDecorator layer, as expected." << std::endl;
    }

    reviewedProfile->approveReview("Lesego");
    if(stackedProfile->complete()){
        std::cout << "  -> " << stackedProfile->getDescription() << " is now " << stackedProfile->getStatus() << "." << std::endl;
    }
}

int main(){
    // --- Build the hierarchy: Project > Division > Feature > Task ---
    // Composite pattern (Lesego): CompositeWorkItem groups leaves and
    // other composites uniformly under the shared WorkItem interface.
    // Three levels of nesting below the root, mixing individual tasks
    // and nested groups, per the practical's structural requirement.
    CompositeWorkItem* project = new CompositeWorkItem("TaskForge Software Project");

    CompositeWorkItem* backend = new CompositeWorkItem("Backend");
    CompositeWorkItem* authFeature = new CompositeWorkItem("Authentication Feature");

    CompositeWorkItem* frontend = new CompositeWorkItem("Frontend");
    CompositeWorkItem* dashboardFeature = new CompositeWorkItem("Dashboard Feature");
    CompositeWorkItem* profileFeature = new CompositeWorkItem("Profile Feature");

    // Leaves (Lesego): LoginTask, PasswordTask, UITask, ProfileTask.
    // State pattern (Melaney): every leaf's start()/block()/complete()
    // is driven by its internal WorkState, exercised throughout below.
    LoginTask* loginTask = new LoginTask("Implement OAuth Login", "OAuth2", true);

    // Decorator pattern (Ajba): CodeReviewDecorator wraps the login task
    // from construction time onward.
    WorkItem* loginItem = new CodeReviewDecorator(loginTask);

    WorkItem* passwordItem = new PasswordTask("Implement Password Reset", "bcrypt", 8, true);
    WorkItem* uiItem = new UITask("Build Dashboard UI", "React", "Dashboard", true);
    WorkItem* profileItem = new ProfileTask("Build Profile Page", 6, true, true);

    authFeature->add(loginItem);
    authFeature->add(passwordItem);
    backend->add(authFeature);

    dashboardFeature->add(uiItem);
    frontend->add(dashboardFeature);
    profileFeature->add(profileItem);
    frontend->add(profileFeature);

    project->add(backend);
    project->add(frontend);

    printHeading("Initial hierarchy");
    printAllItems(project, "All tasks");

    runReviewAndEscalationScenario(project, loginItem, passwordItem);
    runRuntimeModificationScenario(authFeature, dashboardFeature, uiItem);
    runStackedDecoratorScenario(profileFeature, profileItem);

    printHeading("Final state");
    printAllItems(project, "All tasks");

    delete project; // CompositeWorkItem owns and destroys its children recursively
    return 0;
}
