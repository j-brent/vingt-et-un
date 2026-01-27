# Code Inspection and Review Checklist

Systematic error-detection checklist for code inspections, derived from "The Art of Software Testing" (Myers, Chapter 3).

## Purpose

Code inspections find 30-70% of logic design and coding errors before execution. This checklist guides what to look for during reviews.

## Data Reference Errors

- [ ] Is a variable used before being initialized or assigned?
- [ ] Are array/container indexes within declared bounds?
- [ ] Are there off-by-one errors on indexes (0-based vs 1-based)?
- [ ] Is a dangling pointer or reference used after its target is freed/moved?
- [ ] Is a variable used inconsistently with its declared type?
- [ ] Are there aliasing issues (two references to the same data mutated unexpectedly)?
- [ ] Are string/buffer sizes adequate for the data they must hold (including null terminators)?
- [ ] Is a structure/record field accessed that does not belong to the current variant/union?
- [ ] Are bit-level data values computed/accessed correctly?

## Data Declaration Errors

- [ ] Are all variables explicitly declared (not relying on implicit rules)?
- [ ] Are default initializations understood and correct?
- [ ] Is each variable assigned the correct type and length/precision?
- [ ] Are variables initialized correctly where required?
- [ ] Are there variables with similar names that might be confused?
- [ ] Are all named constants defined only once and used consistently?

## Computation Errors

- [ ] Are there computations using variables of inconsistent types (e.g., int + float truncation)?
- [ ] Are there mixed-mode computations that cause implicit conversion errors?
- [ ] Are there computations with variables of different widths (e.g., 32-bit vs 64-bit)?
- [ ] Is the target variable of an assignment smaller than the right-hand side expression (overflow/truncation)?
- [ ] Is there a possibility of overflow or underflow during intermediate computation steps?
- [ ] Is there a possibility of divide-by-zero?
- [ ] Are there inaccuracies due to floating-point representation (comparing floats for equality)?
- [ ] Is operator precedence understood correctly in complex expressions?
- [ ] Is integer division used where floating-point is needed, or vice versa?

## Comparison / Logic Errors

- [ ] Are there comparisons between incompatible types?
- [ ] Are there mixed-type comparisons (e.g., comparing signed and unsigned)?
- [ ] Are comparison operators correct (< vs <=, > vs >=, == vs !=)?
- [ ] Are boolean expressions correct (AND vs OR, De Morgan's law)?
- [ ] Are comparison operands in the correct order (a < b, not b < a)?
- [ ] Are compound boolean expressions fully parenthesized (not relying on precedence)?
- [ ] Are there tests for floating-point equality (should use epsilon/tolerance)?
- [ ] Is the logic correct under all combinations of condition outcomes?
- [ ] Is short-circuit evaluation relied upon for side effects?

## Control Flow Errors

- [ ] Is there a path where a function can reach its end without returning a value?
- [ ] Are there unreachable code sections?
- [ ] Can a loop execute zero times, and is that handled?
- [ ] Can a loop fail to terminate (infinite loop)?
- [ ] Are loop iteration counts off-by-one?
- [ ] Are there fall-through cases in switch/case statements that should break?
- [ ] Are all switch/case statements handled, including a default case?
- [ ] Does each loop have a single, clearly defined exit condition?
- [ ] Are breaks/continues in loops correct and necessary?
- [ ] Does a try/catch handle the right set of exceptions?

## Interface Errors

- [ ] Does the number of arguments match the number of parameters?
- [ ] Do argument types and order match parameter definitions?
- [ ] Are units of measurement consistent across interfaces (e.g., meters vs feet, seconds vs milliseconds)?
- [ ] Are shared global variables or state used identically across all referencing modules?
- [ ] Are pass-by-value vs pass-by-reference semantics correct?
- [ ] Does the callee modify a parameter that the caller does not expect to change?
- [ ] Are constraints on argument values (preconditions) documented and validated?
- [ ] Are return values checked by the caller (not silently discarded)?
- [ ] Are file/stream/resource open/close operations balanced?
- [ ] Are error codes from called functions checked and handled?

## Input/Output Errors

- [ ] Are all file open/close operations correct and balanced?
- [ ] Are file format specifications adhered to?
- [ ] Are buffer sizes sufficient for the I/O operations?
- [ ] Are end-of-file conditions handled?
- [ ] Are I/O error conditions detected and handled?
- [ ] Are text formatting/parsing operations correct (locale, encoding)?

## Memory and Resource Management

- [ ] Are all dynamically allocated resources freed on every code path (including error paths)?
- [ ] Are resources freed exactly once (no double-free)?
- [ ] Are resources used after being freed?
- [ ] Is ownership/lifetime of shared resources clear and correct?
- [ ] Is there potential for memory leaks in error/exception paths?
- [ ] Are RAII or smart pointers used where appropriate (C++)?

## Concurrency Errors (if applicable)

- [ ] Are shared resources protected by appropriate synchronization?
- [ ] Are there potential deadlocks (lock ordering)?
- [ ] Are there potential race conditions?
- [ ] Are atomic operations used where needed?
- [ ] Are thread-local variables correctly used?

## Inspection Process

1. **Preparation**: Reviewers study the code individually (optimal: 150 lines/hour)
2. **Meeting**: Walk through logic line by line, applying this checklist
3. **Rework**: Developer fixes found issues
4. **Follow-up**: Verify fixes are correct and complete
5. **Record**: Track error counts by category to identify systemic issues
