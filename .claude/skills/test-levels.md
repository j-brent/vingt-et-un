# Software Test Levels and Types

Testing levels and strategies from "The Art of Software Testing" (Myers, Chapters 5-7, 9-11).

## Test Level Hierarchy

```
Unit Testing          --> Individual functions/modules in isolation
  |
Integration Testing   --> Module interactions and interfaces
  |
System Testing        --> Complete system against requirements
  |
Acceptance Testing    --> User/customer validates fitness for purpose
  |
Installation Testing  --> Software works in target environment
```

---

## 1. Unit (Module) Testing

Test individual functions, methods, or classes in isolation.

### What to Test

- **Interface**: Parameters received correctly, return values correct, preconditions/postconditions honored
- **Local data structures**: Variables initialized, types correct, no overflow/underflow
- **Boundary conditions**: Inputs at limits of equivalence classes
- **Independent paths**: Every branch through the module executed at least once
- **Error handling paths**: Error conditions detected, correct action taken, error messages accurate

### Strategy

- **Incremental testing** (preferred over big-bang): Add and test one module at a time
- **Top-down integration**: Start from main/entry point, stub lower modules, replace stubs incrementally
- **Bottom-up integration**: Start from leaf modules, use test drivers, compose upward
- **Stubs**: Simplified replacements for called modules (return hardcoded or parameterized values)
- **Drivers**: Test harnesses that call the module under test with controlled inputs

### Unit Test Design Checklist

- [ ] Test each public function/method with valid inputs (happy path)
- [ ] Test each public function/method with invalid inputs (error path)
- [ ] Test boundary values for every parameter
- [ ] Test with null/empty/zero inputs where applicable
- [ ] Verify return values AND side effects (state changes)
- [ ] Verify error handling: correct exception type, error code, or return value
- [ ] Verify resource cleanup (no leaks on any path including error paths)
- [ ] Test interactions between methods that share state within the class
- [ ] Use equivalence partitioning to avoid redundant test cases

---

## 2. Integration Testing

Test interfaces and interactions between modules.

### What to Test

- Data passed between modules (type, order, count, units)
- Global data structures shared across modules
- Module call sequences and event ordering
- Error propagation across module boundaries
- Resource sharing and contention

### Common Interface Errors

- Mismatched parameter count or types
- Wrong order of arguments
- Different units of measurement across interface
- Callee modifies data caller expects unchanged
- Return value or error code ignored by caller

---

## 3. System Testing

Test the complete integrated system against its requirements specification. System testing is NOT a re-test of unit/integration functionality -- it tests the system as a whole from a user perspective.

### 15 System Test Categories

| Category | What to Test |
|----------|-------------|
| **Facility** | Every stated requirement/function exists and works |
| **Volume** | System handles large quantities of data |
| **Stress** | System handles peak load (max concurrent users, burst traffic) |
| **Usability** | Human factors: ease of use, error messages, documentation |
| **Security** | Unauthorized access prevented, data protected, audit trails |
| **Performance** | Response time, throughput, resource consumption under load |
| **Storage** | Memory/disk usage within limits, cleanup of temporary data |
| **Configuration** | Works on all supported hardware/software configurations |
| **Compatibility** | Coexists with other software, backward compatible |
| **Installability** | Installation/upgrade/uninstall procedures work correctly |
| **Reliability** | Mean time between failures, recovery from errors |
| **Recovery** | System recovers from crashes, data loss, hardware failure |
| **Serviceability** | Diagnostic tools, logs, monitoring work correctly |
| **Documentation** | User docs are accurate and complete |
| **Procedure** | Manual procedures (setup, backup, migration) are correct |

---

## 4. Acceptance Testing

- Conducted by or on behalf of the end user / customer
- Validates that the system meets **business needs** (not just technical spec)
- Uses real-world scenarios and data
- Criteria defined before testing begins (acceptance criteria)

---

## 5. Usability Testing (Chapter 7)

### Key Areas

- **Accessibility**: Works for users with disabilities (screen readers, keyboard navigation, color contrast)
- **Responsiveness**: UI reacts quickly, provides feedback for long operations
- **Efficiency**: Common tasks require minimal steps
- **Learnability**: New users can accomplish basic tasks without training
- **Error prevention**: UI design prevents errors before they happen
- **Error recovery**: Clear, actionable error messages; easy to undo/retry

### Usability Test Methods

- **Thinking aloud**: User narrates their thought process while using the software
- **Task-based testing**: Give users specific tasks and observe success/failure
- **Heuristic evaluation**: Experts evaluate against established usability heuristics

---

## 6. Agile / TDD Testing (Chapter 9)

### Test-Driven Development Cycle

1. **Write a failing test** for the next small piece of functionality
2. **Write minimal code** to make the test pass
3. **Refactor** the code while keeping tests green
4. Repeat

### Agile Testing Principles

- Tests are written before or alongside code, not after
- Automated test suite is the primary quality gate
- Tests run on every commit (continuous integration)
- Test suite must be fast (seconds to minutes, not hours)
- Acceptance tests are derived from user stories
- Pair programming serves as a form of continuous code review

---

## 7. Regression Testing

- **Purpose**: Ensure that changes (bug fixes, new features, refactoring) do not break existing functionality
- **Maintain the full test suite**: Never delete tests unless the tested functionality is removed
- **Automate**: Regression testing must be automated to be practical
- **Run frequently**: On every commit or at minimum daily
- **Prioritize**: When the full suite is too slow, run high-risk tests first; full suite nightly

---

## Debugging Principles (Chapter 8)

When a test finds a bug:

### Approaches

1. **Brute force**: Add logging/print statements, use debugger to step through. Least efficient but most common.
2. **Backtracking**: Start from the incorrect output and trace backward through logic to find the point of divergence.
3. **Cause elimination**: Form hypotheses about the cause, design experiments (test cases) to confirm or eliminate each hypothesis. Most efficient for complex bugs.

### Debugging Rules

- **Fix the error, not the symptom.** Understand the root cause before changing code.
- **The probability of the fix being correct is NOT 100%.** Always re-test after fixing.
- **The probability of the fix being correct decreases with program size.** Larger programs have more subtle interactions.
- **Beware of fixing one error and introducing another.** Regression test after every fix.
- **Fix the process, not just the code.** If the same category of error recurs, address the underlying development practice.
