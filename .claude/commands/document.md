# Document Command

Generate and maintain design documentation for this project.

## Instructions

1. **Check for existing design documents**
   - Look for any files in `./docs/design/` directory
   - If the directory doesn't exist, create it
   - Read all existing `.md` files in that directory to understand current documentation state

2. **Scan the project codebase**
   - Read all header files (`.h`) to understand class structures and public APIs
   - Read all implementation files (`.cpp`) to understand design decisions and algorithms
   - Read test files to understand expected behaviors
   - Read CMakeLists.txt files to understand build structure

3. **Generate/Update design documents**
   - Create or update `./docs/design/architecture.md` with:
     - High-level system overview
     - Component relationships
     - UML class diagrams using Mermaid syntax (```mermaid classDiagram)
     - Namespace organization
   - Create or update `./docs/design/game-logic.md` with:
     - Game state machine explanation
     - State transitions with UML state diagram
     - Scoring rules and algorithms
   - Create or update `./docs/design/decisions.md` with:
     - Design decisions found in the code (e.g., why regular types are enforced)
     - Trade-offs and rationale discovered from code patterns

4. **Verification step**
   - After writing/updating documents, read them back
   - Cross-reference with actual code to ensure accuracy
   - Fix any discrepancies found
   - Do NOT include information that cannot be verified in the source code

## Important Rules

- Only document what exists in the code - do not speculate or invent features
- Use Mermaid syntax for all UML diagrams
- Include file paths and line numbers when referencing specific implementations
- If a design decision is unclear from the code, note it as "Implementation detail - rationale not documented"
- Keep documents concise and focused on architecture, not implementation minutiae
