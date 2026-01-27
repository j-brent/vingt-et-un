# Software Testing Principles

Core testing principles derived from "The Art of Software Testing" (Myers, Badgett, Sandler).

## Definition of Testing

Testing is the process of executing a program with the intent of finding errors. A successful test is one that discovers a previously unknown error. A test that reveals no errors has not necessarily demonstrated correctness -- it has simply failed to find faults.

## Ten Fundamental Principles

1. **A necessary part of a test case is a definition of the expected output or result.** If expected results are not predefined, plausible but erroneous results may be accepted as correct.

2. **A programmer should avoid attempting to test his or her own program.** The developer has a psychological investment in demonstrating correctness rather than finding errors. A separate tester (or automated test with independently-derived expectations) is more effective.

3. **A programming organization should not test its own programs.** The same bias that affects individuals can affect teams. Independent testing groups or cross-team review adds objectivity.

4. **Thoroughly inspect the results of each test.** Many errors are missed not because tests fail to expose them but because the tester fails to carefully examine the output. Automate assertions; do not rely on visual inspection.

5. **Test cases must be written for invalid and unexpected input conditions as well as valid and expected ones.** There is a natural tendency to test only the "happy path." Errors found in unexpected conditions are often the most severe.

6. **Examining a program to see if it does not do what it is supposed to do is only half the battle; the other half is seeing whether the program does what it is not supposed to do.** Test for unintended side effects, undesired outputs, and unwanted state changes.

7. **Avoid throwaway test cases unless the program is truly a throwaway program.** Preserve test cases for regression testing. This is the rationale for automated test suites.

8. **Do not plan a testing effort under the tacit assumption that no errors will be found.** This is a sign of using an incorrect definition of testing. Assume errors exist and plan to find them.

9. **The probability of the existence of more errors in a section of a program is proportional to the number of errors already found in that section.** Errors tend to cluster. If a module has many bugs, test it even more thoroughly.

10. **Testing is an extremely creative and intellectually challenging task.** It requires creativity to design inputs that expose non-obvious faults, especially at boundaries and intersections of features.

## The Psychology of Testing

- **Destructive mindset**: Testing is fundamentally destructive -- the goal is to break things. Adopt this mindset deliberately.
- **Goal orientation**: Humans are goal-oriented. If the goal is "show it works," testers subconsciously avoid scenarios that might fail. Set the goal as "find errors."
- **Successful vs. unsuccessful tests**: A test that finds a bug is "successful." A test that finds no bugs is "unsuccessful" (it cost time but added no value). Reframe how you evaluate test outcomes.
- **Ego and ownership**: Developers are psychologically attached to their code. This attachment makes self-testing less effective. Use independent verification where possible.

## The Economics of Testing

- **Exhaustive testing is impossible.** Even a trivially simple program (the triangle classifier) has more test cases than can feasibly be executed. Testing must be strategic.
- **Black-box exhaustive testing** would require testing every possible input permutation -- infeasible for any non-trivial program.
- **White-box exhaustive testing** would require exercising every possible path through the program -- even a small program with loops can have billions of unique paths.
- **Testing is about risk management.** Select test cases that have the highest probability of finding errors. Use the techniques in test-case-design.md to maximize bug-finding per test case.
- **The Pareto principle applies:** Roughly 80% of errors are found in 20% of modules. Focus testing effort where errors cluster.

## Error Categories (by severity)

When classifying found errors, consider:
1. **Missing function** -- Required feature is absent
2. **Wrong function** -- Feature exists but produces incorrect results
3. **Extra function** -- Feature not in spec that causes side effects
4. **Interface error** -- Modules don't communicate correctly
5. **Data structure error** -- Incorrect data organization or access
6. **Performance error** -- Does not meet timing/resource requirements
7. **Initialization error** -- Variables/state not properly set up
8. **Control flow error** -- Incorrect sequencing or branching
9. **Boundary error** -- Incorrect handling at limits/edges
10. **Computation error** -- Incorrect formula or algorithm
