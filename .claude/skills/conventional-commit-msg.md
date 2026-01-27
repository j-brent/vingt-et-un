# Conventional Commits

When creating git commits, follow the Conventional Commits 1.0.0 specification.

## Commit Message Format

```
<type>[optional scope][optional !]: <description>

[optional body]

[optional footer(s)]
```

## Commit Types

- `feat` - A new feature (correlates with MINOR in Semantic Versioning)
- `fix` - A bug fix (correlates with PATCH in Semantic Versioning)
- `build` - Changes that affect the build system or external dependencies
- `chore` - Other changes that don't modify src or test files
- `ci` - Changes to CI configuration files and scripts
- `docs` - Documentation only changes
- `style` - Changes that do not affect the meaning of the code (white-space, formatting, missing semi-colons, etc)
- `refactor` - A code change that neither fixes a bug nor adds a feature
- `perf` - A code change that improves performance
- `test` - Adding missing tests or correcting existing tests

## Rules

- The description MUST immediately follow the colon and space after the type/scope prefix
- The description is a short summary of the code changes
- A longer commit body MAY be provided after the description, providing additional contextual information
- The body MUST begin one blank line after the description
- Breaking changes MUST be indicated by either:
  - A `!` immediately before the `:` in the type/scope prefix, OR
  - A footer with the token `BREAKING CHANGE:` followed by a description
- A footer's token MUST use `-` in place of whitespace characters, e.g., `Acked-by` (exception: `BREAKING CHANGE`)

## Examples

**Feature with scope:**
```
feat(lang): add Polish language
```

**Bug fix:**
```
fix: prevent racing of requests

Introduce a request id and a reference to latest request. Dismiss
incoming responses other than from latest request.
```

**Breaking change with `!`:**
```
feat(api)!: send an email to the customer when a product is shipped
```

**Breaking change with footer:**
```
feat: allow provided config object to extend other configs

BREAKING CHANGE: `extends` key in config file is now used for extending other config files
```

**Docs:**
```
docs: correct spelling of CHANGELOG
```
