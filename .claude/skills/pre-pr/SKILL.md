# Pre-PR Checklist

Run this checklist before creating a pull request. Execute each step sequentially, stopping on any failure.

**Target branch**: `$ARGUMENTS` (default: `main`). If no argument is provided, diff against `main`.

## 1. Branch Hygiene

Verify the branch is up to date with the target branch:

```sh
git fetch origin <target-branch>
git merge-base --is-ancestor origin/<target-branch> HEAD
```

If the target branch has commits not in the current branch, **stop and report** that a rebase or merge is needed before continuing.

Check for untracked files that may need to be committed or gitignored:

```sh
git status --porcelain
```

If there are untracked files, report them and ask the user whether to commit, gitignore, or leave them.

## 2. Build and Test

Run the full cmake workflow (configure + build + test):

```sh
# Windows
cmake --workflow --preset windows-release

# Linux
cmake --workflow --preset linux-release
```

Use the preset matching the current platform. If the build or any test fails, **stop and report the failure**. Do not continue to step 3.

## 3. Format Check

Verify all changed `.cpp` and `.h` files are properly formatted:

```sh
git diff --name-only <target-branch> -- '*.cpp' '*.h'
```

For each file returned, run `clang-format --dry-run --Werror <file>`. If any file fails, run `clang-format -i` on it and report which files were fixed (they will need to be committed).

## 4. Debug Code Check

Scan the branch diff for leftover debug or temporary code:

```sh
git diff <target-branch> -- '*.cpp' '*.h' | grep -nE '^\+.*(std::cout|std::cerr|printf|#if 0|// TODO|// FIXME|// HACK|// XXX)'
```

If any matches are found in added lines (lines starting with `+`), report them with file names and line content. Ask the user whether to remove them.

## 5. Conventional Commit Messages

Validate that all commits on the branch follow the format defined in `.claude/skills/conventional-commit-msg.md`:

```sh
git log --format='%H %s' <target-branch>..HEAD
```

Check each commit subject against the rules in that skill file. Report any non-conforming commits. If found, ask the user whether to proceed anyway or squash/reword them first.

## 6. Test Coverage for New Functionality

Get the diff against the target branch:

```sh
git diff --stat <target-branch>
git diff <target-branch> -- 'src/*.cpp' 'src/*.h'
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
- **`docs/architecture.excalidraw`** — Before invoking the `/excalidraw` skill, read `docs/architecture.excalidraw` (if it exists) and provide its contents as context so the skill updates the existing architecture rather than generating from scratch. Generate to a temporary file (e.g. `docs/architecture_new.excalidraw`). Parse both JSON files and compare their element arrays (type, text labels, arrow bindings). If components, labels, or relationships differ, replace the old file with the new one. If structurally equivalent, delete the temporary file. Report whether the diagram was updated.

Report anything stale and ask the user whether to update it now.

## 8. Summary

After all steps complete, print a summary table:

| Check             | Result                    |
|-------------------|---------------------------|
| Branch hygiene    | OK / BEHIND (needs rebase)|
| Untracked files   | CLEAN / FOUND (list)      |
| Build             | PASS / FAIL               |
| Tests             | PASS / FAIL (N ran)       |
| Format            | PASS / FIXED (list)       |
| Debug code        | CLEAN / FOUND (list)      |
| Commit messages   | PASS / FAIL (list)        |
| Test coverage     | PASS / GAPS (list)        |
| Documentation     | CURRENT / STALE (list)    |
| Architecture      | CURRENT / UPDATED         |

Ask the user if they are ready to create the PR.
