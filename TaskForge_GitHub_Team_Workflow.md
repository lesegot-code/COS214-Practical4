# TaskForge — Git & GitHub Team Workflow

**Team:** No Prior Declaration  
**Project:** COS214 Practical 4

## 1. Branch Structure

- `branch/main` — final submission-ready code.
- `branch/dev` — integration branch.
- `feat/ajba`, `feat/lesego`, `feat/melaney` — individual working branches.

### Rules

- Never push directly to `main` or `dev`.
- Work on your own branch.
- **Never merge locally.**
- **All merges must happen through GitHub Pull Requests (PRs).**
- `main` is updated from `dev` only at the end.

## 2. GitHub Issue Workflow

Every task must have a GitHub Issue.

`Open → In Progress → Completed → Closed`

When starting:
- Assign the issue to yourself.
- Mark it as `In Progress`.

When finished:
- Test the work.
- Reference the issue in the commit/PR.
- Close the issue after the PR is successfully merged.

If the issue is not fully resolved, **do not close it**. Use labels such as:

- `has-bug` — a known bug still exists.
- `in-progress` — work is still being done.
- `blocked` — waiting for another task/person.
- `needs-review` — ready for review.

Create a new issue for unrelated bugs.

## 3. Daily Git Workflow

`Own Branch → Pull dev → Work → Test → Pull dev AGAIN → Commit → Push → PR`

Example:

    git checkout lesego
    git pull origin dev

    # Work on your task...

    # ALWAYS pull before committing
    git pull origin dev

    git add .
    git commit -m "feat: implement Component interface"
    git push origin lesego

Replace `lesego` with your own branch.

> **Golden Rule: Always pull before committing.**

## 4. Commit Convention

Format:

`type: short description`

| Type | Use |
|---|---|
| `feat` | New functionality |
| `fix` | Bug fix |
| `docs` | Documentation/UML/README |
| `refactor` | Code restructuring |
| `test` | Tests |
| `chore` | Build/Docker/Makefile |

Examples:

    feat: implement Composite class
    fix: prevent invalid state transition
    docs: update UML class diagram
    chore: add Dockerfile

## 5. Pull Request Workflow

**All merging is done on GitHub. Never merge locally.**

When an issue is complete:

1. Pull the latest `dev`.
2. Test your changes.
3. Commit and push your branch.
4. Create a PR on GitHub.
5. Set **Base:** `dev` and **Compare:** your branch.
6. Link the relevant GitHub Issue.
7. Request a teammate review.
8. Fix review comments if necessary.
9. Get approval.
10. **Merge the PR on GitHub.**
11. Close the issue.

`Your Branch → GitHub PR → dev`

## 6. PR Review Rules

Reviewers should check:

- Code works correctly.
- Implementation matches the agreed design/UML.
- No obvious bugs or memory issues exist.
- No unnecessary changes to another person's files.
- Code is understandable and appropriate for the project.

If problems exist, request changes instead of approving.

## 7. File Ownership

| Owner | Main Files |
|---|---|
| Lesego | Component, Composite, Iterator, Makefile, Dockerfile, main |
| Melaney | State, Concrete States, FilteredIterator |
| Ajba | Decorator, Concrete Decorators, README/docs |

> **If you do not own a file, discuss changes with its owner first.**

## 8. Conflict Prevention

- Pull `dev` before starting work.
- **Always pull before committing.**
- Keep commits small and logical.
- Avoid modifying another person's files.
- Push completed work regularly.
- Create PRs regularly.
- **Never merge branches locally.**

If a conflict occurs, communicate with the affected teammate before resolving it.

## 9. Final Integration

All individual work must first be merged into `dev` through GitHub PRs.

`Individual Branches → PRs → dev → Final Testing → PR → main`

### Final `dev → main`

1. Ensure all completed work is merged into `dev`.
2. Test the complete project.
3. Run Valgrind and fix problems.
4. Check UML and documentation.
5. Create a PR with **Base:** `main` and **Compare:** `dev`.
6. All three members review and approve.
7. Merge the PR **on GitHub**.
8. Tag the final version.

    git checkout main
    git pull origin main
    git tag v1.0
    git push origin v1.0

## 10. Quick Reference

`GitHub Issue → Assign/In Progress → Own Branch → Pull dev → Work → Test → Pull dev AGAIN → Commit → Push → GitHub PR → Review → Merge PR → Close Issue`

### Golden Rules

1. **Pull before committing.**
2. **Never merge locally.**
3. **All merges happen through GitHub PRs.**
4. **Never push directly to `main` or `dev`.**
5. **Keep GitHub Issues updated.**
6. **Close issues only when their work is actually complete.**
7. **Use `has-bug`, `blocked`, or `in-progress` when necessary.**
8. **Review PRs promptly.**
9. **Test before committing and before merging to `main`.**
