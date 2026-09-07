// Standalone tester for the State pattern (WorkState hierarchy) and the
// FilteredIterator. Not part of the team's main taskforge build - compile
// and run this directly, see the command at the bottom of this file.

#include "../CompositeWorkItem.h"
#include "../LoginTask.h"
#include "../PasswordTask.h"
#include "../ProfileTask.h"
#include "../UITask.h"
#include "../FilteredIterator.h"
#include "../WorkItemIterator.h"

#include <cassert>
#include <iostream>

static int checksRun = 0;
static int checksPassed = 0;

void check(bool condition, const std::string& description){
    ++checksRun;
    if(condition){
        ++checksPassed;
        std::cout << "  [PASS] " << description << std::endl;
    } else {
        std::cout << "  [FAIL] " << description << std::endl;
    }
}

int main(){
    std::cout << "=== State transition tests ===" << std::endl;

    LoginTask* login = new LoginTask("Implement OAuth login", "OAuth2", true);

    check(login->getStatus() == "Pending", "new task starts Pending");
    check(login->block() == false, "cannot block a Pending task");
    check(login->complete() == false, "cannot complete a Pending task");

    check(login->start() == true, "Pending -> InProgress via start()");
    check(login->getStatus() == "InProgress", "status is now InProgress");
    check(login->start() == false, "cannot start an already InProgress task");

    check(login->block() == true, "InProgress -> Blocked via block()");
    check(login->getStatus() == "Blocked", "status is now Blocked");
    check(login->complete() == false, "cannot complete while Blocked");
    check(login->block() == false, "cannot block an already Blocked task");

    check(login->start() == true, "Blocked -> InProgress via start() (resume)");
    check(login->getStatus() == "InProgress", "status is back to InProgress after resume");

    check(login->complete() == true, "InProgress -> Done via complete()");
    check(login->getStatus() == "Done", "status is now Done");
    check(login->start() == false, "cannot start a Done task (terminal)");
    check(login->block() == false, "cannot block a Done task (terminal)");
    check(login->complete() == false, "cannot complete a Done task again");

    std::cout << std::endl << "=== Building a nested hierarchy (3+ levels) ===" << std::endl;

    // Root: Project
    //   -> Module: Auth   (Division-level composite)
    //        -> Feature: Login  (Team-level composite)
    //             -> LoginTask, PasswordTask   (leaves)
    //        -> Feature: Profile
    //             -> ProfileTask, UITask       (leaves)
    CompositeWorkItem* project = new CompositeWorkItem("Project: TaskForge App");
    CompositeWorkItem* authModule = new CompositeWorkItem("Module: Auth");
    CompositeWorkItem* loginFeature = new CompositeWorkItem("Feature: Login");
    CompositeWorkItem* profileFeature = new CompositeWorkItem("Feature: Profile");

    PasswordTask* password = new PasswordTask("Hash and validate passwords", "bcrypt", 8, true);
    ProfileTask* profile = new ProfileTask("Build profile editor", 6, true, true);
    UITask* ui = new UITask("Build profile screen", "React", "ProfileScreen", true);

    loginFeature->add(login);
    loginFeature->add(password);
    profileFeature->add(profile);
    profileFeature->add(ui);

    authModule->add(loginFeature);
    authModule->add(profileFeature);
    project->add(authModule);

    check(project->getChildCount() == 1, "project has one direct child (authModule)");
    check(authModule->getChildCount() == 2, "authModule has two direct children (features)");
    check(loginFeature->getChildCount() == 2, "loginFeature has two leaf children");

    // Put a couple of tasks into Blocked so the FilteredIterator has something to find.
    password->start();
    password->block();

    profile->start();
    profile->block();

    ui->start(); // left InProgress on purpose, should NOT be picked up by the Blocked filter

    std::cout << std::endl << "=== FilteredIterator: only Blocked items ===" << std::endl;

    // Note: CompositeWorkItem::getStatus() bubbles "Blocked" up to any group
    // containing a Blocked child, so filtering on status alone would also
    // match groups. Restricting to leaves (childCount == 0) gives us just
    // the individual Blocked tasks, which is what we actually want here.
    auto isBlocked = [](const WorkItem* item){
        return item->getChildCount() == 0 && item->getStatus() == "Blocked";
    };
    WorkItemIterator* blockedIterator = project->createFilteredIterator(isBlocked);

    int blockedCount = 0;
    while(blockedIterator->hasNext()){
        WorkItem* item = blockedIterator->next();
        std::cout << "  Blocked: " << item->getName() << std::endl;
        ++blockedCount;
    }
    check(blockedCount == 2, "FilteredIterator found exactly the 2 Blocked tasks");

    std::cout << std::endl << "=== Snapshot policy: iterator unaffected by later changes ===" << std::endl;

    WorkItemIterator* snapshotIterator = project->createFilteredIterator(isBlocked);
    check(snapshotIterator->hasNext(), "snapshot iterator has matches before any change");

    // Structural/state change AFTER the iterator was built: resume password mid-traversal.
    password->start();
    check(password->getStatus() == "InProgress", "password task is no longer Blocked");

    int snapshotCount = 0;
    while(snapshotIterator->hasNext()){
        WorkItem* item = snapshotIterator->next();
        std::cout << "  Snapshot still yields: " << item->getName()
                   << " (live status now: " << item->getStatus() << ")" << std::endl;
        ++snapshotCount;
    }
    check(snapshotCount == 2, "snapshot iterator still yields both original matches despite the later change");

    std::cout << std::endl << checksPassed << "/" << checksRun << " checks passed." << std::endl;

    delete project; // composite owns and deletes authModule, features, and all leaves

    return (checksPassed == checksRun) ? 0 : 1;
}
