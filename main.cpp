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

// Scenario 4: coverage completeness - deliberately triggers the invalid-
// transition rejections and less-common forwarding paths that the story-
// driven scenarios above don't naturally hit (e.g. blocking an already-
// blocked task, or calling add()/getChild() through a decorator wrapping
// a composite). Kept separate from the narrative scenarios so it's clear
// this section exists for testing thoroughness rather than the demo story.
static void runCoverageCompletenessChecks(CompositeWorkItem* frontend, CompositeWorkItem* authFeature, CompositeWorkItem* dashboardFeature, CompositeWorkItem* profileFeature, WorkItem* loginItem, WorkItem* passwordItem, WorkItem* uiItem){
    printHeading("Scenario 4: Verifying invalid transitions and forwarding paths");

    // --- Done is terminal: every transition from here must be rejected ---
    std::cout << "Done task rejects every further transition:" << std::endl;
    std::cout << "  start()    -> " << (loginItem->start() ? "accepted (unexpected)" : "rejected, as expected") << std::endl;
    std::cout << "  block()    -> " << (loginItem->block() ? "accepted (unexpected)" : "rejected, as expected") << std::endl;
    std::cout << "  complete() -> " << (loginItem->complete() ? "accepted (unexpected)" : "rejected, as expected") << std::endl;

    // --- Blocked cannot be blocked again or completed, but can resume ---
    std::cout << std::endl << "Blocked task rejects a repeat block() and a complete():" << std::endl;
    std::cout << "  block()    -> " << (passwordItem->block() ? "accepted (unexpected)" : "rejected, as expected") << std::endl;
    std::cout << "  complete() -> " << (passwordItem->complete() ? "accepted (unexpected)" : "rejected, as expected") << std::endl;
    std::cout << "  authFeature status while a child is Blocked -> " << authFeature->getStatus() << std::endl;
    std::cout << "  start()    -> " << (passwordItem->start() ? "accepted, resumed to InProgress" : "rejected (unexpected)") << std::endl;
    std::cout << "  start() again while InProgress -> " << (passwordItem->start() ? "accepted (unexpected)" : "rejected, as expected") << std::endl;

    // --- Pending cannot be blocked or completed before it has started ---
    std::cout << std::endl << "Pending task rejects block() and complete() before start():" << std::endl;
    std::cout << "  block()    -> " << (uiItem->block() ? "accepted (unexpected)" : "rejected, as expected") << std::endl;
    std::cout << "  complete() -> " << (uiItem->complete() ? "accepted (unexpected)" : "rejected, as expected") << std::endl;

    // --- WorkItem's default leaf behaviour for composite-only operations ---
    std::cout << std::endl << "Default leaf behaviour for composite-only operations on a task:" << std::endl;
    std::cout << "  add()         -> " << (uiItem->add(nullptr) ? "accepted (unexpected)" : "rejected, no-op for a leaf") << std::endl;
    std::cout << "  remove()      -> " << (uiItem->remove(nullptr) ? "accepted (unexpected)" : "rejected, no-op for a leaf") << std::endl;
    std::cout << "  getChild(0)   -> " << (uiItem->getChild(0) == nullptr ? "nullptr, as expected" : "unexpected value") << std::endl;
    std::cout << "  getChildIndex -> " << uiItem->getChildIndex(nullptr) << " (expected -1)" << std::endl;
    std::cout << "  detach()      -> " << (uiItem->detach(nullptr) == nullptr ? "nullptr, as expected" : "unexpected value") << std::endl;

    // --- CompositeWorkItem paths nothing else exercises: execute(), add() rejections, getStatus() aggregation ---
    std::cout << std::endl << "Composite-specific behaviour:" << std::endl;
    std::cout << "  add(nullptr)         -> " << (frontend->add(nullptr) ? "accepted (unexpected)" : "rejected, as expected") << std::endl;
    std::cout << "  add(existing child)  -> " << (frontend->add(dashboardFeature) ? "accepted (unexpected)" : "rejected, already a child") << std::endl;
    std::cout << "  empty group status   -> " << dashboardFeature->getStatus() << " (expected Pending)" << std::endl;
    std::cout << "  mixed group status   -> " << frontend->getStatus() << std::endl;
    std::cout << "  all-done group status -> " << profileFeature->getStatus() << " (expected Done)" << std::endl;
    std::cout << "  execute() on a group forwards to every child:" << std::endl;
    frontend->execute();
    std::cout << "  frontend->getDescription() -> " << frontend->getDescription() << std::endl;

    // --- Decorator forwarding for composite operations, wrapping a group instead of a leaf ---
    std::cout << std::endl << "Decorator wrapping a composite group forwards composite operations too:" << std::endl;
    dashboardFeature->remove(nullptr); // dashboardFeature is empty after Scenario 2's move; exercises the not-found path
    WorkItem* detachedDashboard = frontend->detach(dashboardFeature); // frontend no longer owns it, so the decorator safely can
    WorkItem* decoratedGroup = new PriorityEscalationDecorator(detachedDashboard, "Low");
    PriorityEscalationDecorator* escalatableGroup = dynamic_cast<PriorityEscalationDecorator*>(decoratedGroup);
    std::cout << "  Priority before escalate() -> " << escalatableGroup->getPriorityLevel() << std::endl;
    escalatableGroup->escalate("Medium");
    std::cout << "  Priority after escalate()  -> " << escalatableGroup->getPriorityLevel() << std::endl;
    escalatableGroup->execute();
    std::cout << "  Decorated group has " << decoratedGroup->getChildCount() << " children before add()" << std::endl;
    WorkItem* filler = new UITask("Coverage filler task", "Vue", "Settings", false);
    decoratedGroup->add(filler);
    std::cout << "  Decorated group has " << decoratedGroup->getChildCount() << " children after add()" << std::endl;
    std::cout << "  getChild(99)    -> " << (decoratedGroup->getChild(99) == nullptr ? "nullptr, out of range" : "unexpected value") << std::endl;
    std::cout << "  getChildIndex   -> " << decoratedGroup->getChildIndex(filler) << std::endl;
    std::cout << "  getChildIndex(nullptr) -> " << decoratedGroup->getChildIndex(nullptr) << " (expected -1)" << std::endl;
    std::cout << "  detach(nullptr) -> " << (decoratedGroup->detach(nullptr) == nullptr ? "nullptr, as expected" : "unexpected value") << std::endl;
    std::cout << "  detach(non-child) -> " << (frontend->detach(loginItem) == nullptr ? "nullptr, not a direct child" : "unexpected value") << std::endl;
    std::cout << "  remove(non-child) -> " << (frontend->remove(loginItem) ? "unexpected acceptance" : "rejected, not a direct child") << std::endl;
    WorkItemIterator* groupIterator = decoratedGroup->createIterator();
    std::cout << "  createIterator() -> hasNext() is " << (groupIterator->hasNext() ? "true" : "false") << std::endl;
    delete groupIterator;
    std::cout << "  remove(filler)  -> " << (decoratedGroup->remove(filler) ? "accepted, filler deleted" : "unexpected rejection") << std::endl;

    PriorityEscalationDecorator* plainDecoratedGroup = dynamic_cast<PriorityEscalationDecorator*>(decoratedGroup);
    WorkItem* plainGroup = plainDecoratedGroup->release();
    delete plainDecoratedGroup;
    delete plainGroup; // deletes dashboardFeature; it is empty and no longer referenced elsewhere
}

int main(){
    // --- Build the hierarchy: Project > Division > Feature > Task ---
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
    runCoverageCompletenessChecks(frontend, authFeature, dashboardFeature, profileFeature, loginItem, passwordItem, uiItem);

    printHeading("Final state");
    printAllItems(project, "All tasks");

    delete project; // CompositeWorkItem owns and destroys its children recursively
    return 0;
}