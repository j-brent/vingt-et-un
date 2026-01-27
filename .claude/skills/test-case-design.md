# Test-Case Design Techniques

Systematic methods for designing test cases that maximize error detection, derived from "The Art of Software Testing" (Myers, Chapter 4).

## Strategy Overview

No single technique is sufficient. Combine black-box and white-box methods:
1. Start with black-box techniques (equivalence partitioning, boundary analysis) to cover functional requirements
2. Supplement with white-box techniques (coverage analysis) to exercise internal logic
3. Apply error guessing based on experience and domain knowledge

---

## Black-Box (Functional) Techniques

### 1. Equivalence Partitioning

Divide input domain into classes (partitions) where the program is expected to behave identically for all members of a class. Test one representative from each class.

**Steps:**
1. Identify each input condition from the specification
2. For each condition, identify:
   - **Valid equivalence classes** (inputs the program should accept)
   - **Invalid equivalence classes** (inputs the program should reject)
3. Write test cases covering uncovered valid classes (combine multiple valid classes per test)
4. Write test cases covering uncovered invalid classes (one invalid class per test -- isolate the error)

**Rules for identifying classes:**
- If input is a **range** (e.g., 1-999): one valid class (1 <= x <= 999), two invalid classes (x < 1, x > 999)
- If input is a **specific value** (e.g., count must be 5): one valid class (x = 5), two invalid classes (x < 5, x > 5)
- If input is a **member of a set** (e.g., "A", "B", "C"): one valid class per member, one invalid class (not in set)
- If input is a **boolean**: one valid true class, one valid false class
- If input has a **must be** constraint (e.g., "first character must be a letter"): one valid class (is letter), one invalid class (not letter)
- If there is reason to believe elements in a class are **not handled identically**, split into smaller classes

### 2. Boundary Value Analysis

Errors tend to cluster at the edges of equivalence classes rather than at the center. Test at and around boundaries.

**Rules:**
- If input range is a..b, test: a, b, a-1, b+1
- If input specifies a count of values (1..n), test with 0, 1, n, n+1 values
- Apply rule 1 and 2 to **output** conditions as well (design inputs that produce boundary outputs)
- If input is an ordered set, test first, last, and adjacent elements
- If data structure has a size limit (e.g., array of max 100), test at 0, 1, 99, 100, 101 elements
- For numeric calculations, test values near zero, max int, min int, precision limits

### 3. Cause-Effect Graphing

For programs with complex combinations of input conditions, systematically derive test cases from logical relationships.

**Steps:**
1. Decompose the specification into causes (input conditions) and effects (outputs/actions)
2. Draw a cause-effect graph (boolean logic: AND, OR, NOT relationships between causes and effects)
3. Add constraints (e.g., at most one of causes A and B can be true)
4. Convert the graph to a decision table
5. Each column of the decision table becomes a test case

### 4. Error Guessing

Use intuition, experience, and common bug patterns to supplement systematic methods.

**Common error-prone areas:**
- The number zero (as input, output, count, or denominator)
- Empty/null inputs (empty string, null pointer, empty collection)
- Single-element collections
- Blank or whitespace-only strings
- Negative numbers where only positive expected
- Maximum and minimum values for the data type
- Duplicate values in lists expected to be unique
- Operations at exactly the boundary of resources (memory, disk, connections)
- Concurrent access / race conditions
- Unicode, special characters, very long strings
- Date/time boundaries (midnight, DST transitions, leap years, epoch)
- Division by zero, integer overflow/underflow
- Off-by-one errors (< vs <=, 0-indexed vs 1-indexed)
- State-dependent behavior (calling methods in wrong order)

---

## White-Box (Structural) Techniques

### Coverage Hierarchy (weakest to strongest)

1. **Statement coverage**: Every statement executed at least once. Weak -- misses many logic errors.
2. **Decision/branch coverage**: Every branch (true/false) of every decision taken at least once. Better but still misses compound condition errors.
3. **Condition coverage**: Every individual condition in a compound decision evaluates to both true and false. Does not guarantee decision coverage.
4. **Decision/condition coverage**: Satisfies both decision and condition coverage simultaneously.
5. **Multiple condition coverage** (strongest practical): All possible combinations of condition outcomes in each decision are tested. This is the recommended minimum for white-box testing.

### Applying Coverage

- Use a coverage tool to measure which level you achieve
- Start by writing tests from black-box techniques
- Analyze coverage gaps and add targeted white-box tests
- Focus on branches and conditions that involve error-handling, edge cases, and complex boolean logic
- **Cyclomatic complexity** (V(G) = edges - nodes + 2) gives the minimum number of independent paths to test

---

## Test Case Template

For each test case, specify:

| Field | Description |
|-------|-------------|
| **ID** | Unique identifier |
| **Description** | What is being tested and why |
| **Preconditions** | Required state before test execution |
| **Input** | Exact input values |
| **Expected Output** | Exact expected result (MANDATORY -- Principle #1) |
| **Technique** | Which design technique motivated this test case |
| **Priority** | High/Medium/Low based on error probability |

---

## Combining Techniques: Recommended Process

1. **Equivalence partition** all inputs and outputs
2. **Boundary value analyze** each partition boundary
3. **Cause-effect graph** complex input combinations
4. **Error guess** based on the domain and common bug patterns
5. **Measure coverage** and fill gaps with targeted white-box tests
6. **Review** the test suite against the spec to check for missing requirements coverage
