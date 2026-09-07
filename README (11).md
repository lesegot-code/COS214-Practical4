# TaskForge

**COS 214 Practical 4** — a hierarchical work-processing system for a software
development domain, demonstrating Composite, Iterator, State and Decorator
working together as one coherent system.

A `TaskForge Software Project` is built from nested `Backend`/`Frontend`
divisions, each containing features (e.g. `Authentication Feature`,
`Dashboard Feature`), each containing individual development tasks
(`LoginTask`, `PasswordTask`, `UITask`, `ProfileTask`). Tasks move through a
lifecycle (`Pending → InProgress → {Blocked, Done}`), can be traversed in
different ways, and can be wrapped with optional, stackable responsibilities
(code review gating, priority escalation) at runtime.

## Team & File Ownership

| Person | Owns |
|---|---|
| Lesego | Component (`WorkItem`), `CompositeWorkItem`, Iterators (`WorkItemIterator`, `FullTraversalIterator`), `Makefile`, `Dockerfile`, `main.cpp` |
| Melaney | State hierarchy (`WorkState` + concrete states), `FilteredIterator` |
| Ajba | Decorator hierarchy (`WorkItemDecorator` + concrete decorators), `README.md` / `docs/` |

If you don't own a file, discuss changes with its owner first (see
`TaskForge_GitHub_Team_Workflow.md`).

## Requirements

Everything below can be run either directly on a machine with `g++`
(C++11), `make`, `gdb` and `valgrind` installed, **or** inside the provided
Docker image with no host dependencies at all (see [Docker](#docker)).

## Build


make


Compiles with `-std=c++11 -Wall -Wextra -Werror -g` and produces the
`taskforge` executable. To rebuild from scratch:


make clean
make


## Run


./taskforge


Runs the full demo: builds the task hierarchy, then walks through three
scenarios — a login task moving through code review and priority
escalation; a runtime-modification scenario where a task is moved between
groups and has a decorator attached and later removed while the program
is running; and a stacked-decorator scenario where a task is wrapped with
both concrete decorators at once.

## Tests

Two standalone test files live in `tests/` (compiled separately from the
main `taskforge` build, since they define their own `main()`):


cd tests

# State + Iterator tests (Melaney)
g++ -std=c++11 -Wall -Wextra -Werror -I.. state_iterator_test.cpp \
    ../WorkItem.cpp ../CompositeWorkItem.cpp ../LoginTask.cpp \
    ../PasswordTask.cpp ../ProfileTask.cpp ../UITask.cpp \
    ../PendingState.cpp ../InProgressState.cpp ../BlockedState.cpp \
    ../DoneState.cpp ../FilteredIterator.cpp ../FullTraversalIterator.cpp \
    -o state_iterator_test
./state_iterator_test

# Decorator tests (Ajba)
g++ -std=c++11 -Wall -Wextra -Werror -I.. decorator_test.cpp \
    ../WorkItem.cpp ../CompositeWorkItem.cpp ../WorkItemDecorator.cpp \
    ../CodeReviewDecorator.cpp ../PriorityEscalationDecorator.cpp \
    ../LoginTask.cpp ../PasswordTask.cpp ../PendingState.cpp \
    ../InProgressState.cpp ../BlockedState.cpp ../DoneState.cpp \
    ../FilteredIterator.cpp ../FullTraversalIterator.cpp \
    -o decorator_test
./decorator_test


Both print a `[PASS]`/`[FAIL]` line per check and a final `X/Y checks
passed` summary; both currently pass in full.

## Debugging with GDB

Example session, breaking on the code-review gate to inspect state before
and after approval:


g++ -std=c++11 -g -o taskforge *.cpp
gdb ./taskforge

(gdb) break CodeReviewDecorator::complete
(gdb) run
(gdb) print reviewed
(gdb) next
(gdb) continue


Use `bt` for a backtrace and `print *this` inside any method to inspect
the current object's state.

## Valgrind


make valgrind


Equivalent to:


valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./taskforge


Current result: **0 errors, 0 leaks, all heap blocks freed.**

## Coverage


make coverage


Builds with `--coverage`, runs `taskforge`, and generates an HTML report
at `coverage/coverage.html` via `gcovr`.

## Docker

Build and run the whole project with no host dependencies:


docker build -t taskforge .
docker run -it --rm taskforge


Inside the container:

make
./taskforge
make valgrind
gdb ./taskforge




