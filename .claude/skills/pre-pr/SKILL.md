# Pre-PR Checklist

Run this checklist before creating a pull request. Execute each step sequentially, stopping on any failure.

**Target branch**: `$ARGUMENTS` (default: `main`). If no argument is provided, diff against `main`.

## 1. Branch Hygiene

Verify the branch is up to date with the target branch:

```sh
git fetch origin <target-branch>
git merge-base --is-ancestor origin/<target-branch> HEAD
# Exit 0 = origin/<target-branch> is an ancestor of HEAD (up to date)
# Exit 1 = HEAD is missing commits from origin/<target-branch> (behind)
```

If the command exits non-zero, **stop and report** that a rebase or merge is needed before continuing.

Check for uncommitted changes (staged, unstaged, and untracked):

```sh
git status --porcelain
```

- Lines starting with `M ` or `A ` = staged changes
- Lines starting with ` M` = unstaged modifications to tracked files
- Lines starting with `??` = untracked files

If any are found, report them grouped by category and ask the user whether to commit, stash, gitignore, or leave them before proceeding.

## 2. Build and Test

Run the full cmake workflow (configure + build + test):

```sh
# Windows
cmake --workflow --preset windows-release

# Linux
cmake --workflow --preset linux-release
```

Use the preset matching the current platform. If the build or any test fails, **stop and report the failure**. Do not continue to step 3.

If the branch diff includes files under `app/qml/`, also run the QML workflow:

```sh
# Windows
cmake --workflow --preset windows-qml-release

# Linux
cmake --workflow --preset linux-qml-release
```

Skip the QML workflow if Qt 6.8+ is not available (the configure step will fail immediately — report that QML tests were skipped due to missing Qt and continue).

## 3. Format Check

Verify all changed `.cpp` and `.h` files are properly formatted:

```sh
git diff --name-only <target-branch>...HEAD -- '*.cpp' '*.h'
```

For each file returned, run `clang-format --dry-run --Werror <file>`. If any file fails, run `clang-format -i` on it and report which files were fixed (they will need to be committed).

## 4. Debug Code Check

Find files changed on this branch (excluding `main.cpp` and `streaming.cpp` where console output is legitimate):

```sh
git diff --name-only <target-branch>...HEAD -- '*.cpp' '*.h'
```

For each changed file (except `main.cpp` and `streaming.cpp`), search the **working tree copy** for debug or temporary code:

```sh
grep -nE '(std::cout|std::cerr|printf|#if 0|// TODO|// FIXME|// HACK|// XXX)' <file>
```

This gives real source line numbers. Cross-reference against the branch diff to confirm the match is in a newly added or modified line (not pre-existing code). Report matches with file path and line number. Ask the user whether to remove them.

## 5. Conventional Commit Messages

Validate that all commits on the branch follow the format defined in `.claude/skills/conventional-commit-msg.md`:

```sh
# Two dots: commits on HEAD not on target (branch-only commits)
# Three dots is correct for `git diff` (diff against merge base) but wrong
# for `git log` where it means symmetric difference (includes both sides)
git log --format='%H %s' <target-branch>..HEAD
```

Check each commit subject against the rules in that skill file. Report any non-conforming commits. If found, ask the user whether to proceed anyway or squash/reword them first.

## 6. Test Coverage for New Functionality

Get the diff against the target branch:

```sh
git diff --stat <target-branch>...HEAD
git diff <target-branch>...HEAD -- src/
```

For each new or substantially changed source file:
- Check that corresponding test scenarios exist in `test/`
- If new public functions, methods, or game states were added, verify they have test coverage
- Report any gaps and ask the user whether to write the missing tests now

## 7. Documentation

Check that documentation reflects the current state of the code:

- **`test/README.md`** — If any `test/*.cpp` file was added or modified, verify the README's scenario tables and line numbers are accurate
- **`docs/design/`** — If architecture, game logic, or design decisions changed, check that `architecture.md`, `game-logic.md`, or `decisions.md` are still correct
- **`CLAUDE.md`** — If build commands, test commands, or architecture descriptions changed, verify CLAUDE.md is current
- **`docs/architecture.excalidraw`** — Before invoking the `/excalidraw` skill, read `docs/architecture.excalidraw` if it exists and provide its contents as context so the skill updates the existing architecture rather than generating from scratch. If the file does not exist, generate a new diagram from codebase analysis and save it directly to `docs/architecture.excalidraw` (no comparison needed). When the file does exist, generate to a temporary file (e.g. `docs/architecture_new.excalidraw`). Parse both JSON files and compare elements using only these structural fields: `type`, `text`/`originalText` (label content), `boundElements` (shape-to-text bindings), `containerId` (text-to-shape bindings), `startBinding`/`endBinding` (arrow-to-shape bindings), and the count of elements. Ignore layout fields (`x`, `y`, `width`, `height`, `points`) and auto-generated fields (`id`, `seed`, `version`, `versionNonce`, `updated`). If the structural fields differ (new/removed components, changed labels, changed bindings), replace the old file with the new one. If structurally equivalent, delete the temporary file. Report whether the diagram was updated.

Report anything stale and ask the user whether to update it now.

## 8. Summary

After all steps complete, print a summary table:

| Check             | Result                    |
|-------------------|---------------------------|
| Branch hygiene    | OK / BEHIND (needs rebase)|
| Staged changes    | CLEAN / FOUND (list)      |
| Untracked files   | CLEAN / FOUND (list)      |
| Build             | PASS / FAIL               |
| Tests             | PASS / FAIL (N ran)       |
| QML Build         | PASS / FAIL / SKIPPED     |
| QML Tests         | PASS / FAIL / SKIPPED     |
| Format            | PASS / FIXED (list)       |
| Debug code        | CLEAN / FOUND (list)      |
| Commit messages   | PASS / FAIL (list)        |
| Test coverage     | PASS / GAPS (list)        |
| Documentation     | CURRENT / STALE (list)    |
| Architecture      | CURRENT / UPDATED         |

Ask the user if they are ready to create the PR.
