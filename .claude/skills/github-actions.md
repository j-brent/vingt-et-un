# Writing GitHub Actions Workflows

## Before writing any installation step, search for existing actions

Search GitHub Marketplace or web for `"GitHub Action install <tool>"` before writing manual installation code. Existing actions handle:
- Caching
- Cross-platform differences
- Environment variable setup
- Edge cases you haven't considered

**Examples of common actions:**
| Need | Action |
|------|--------|
| Qt | `jurplel/install-qt-action@v4` |
| Node.js | `actions/setup-node@v4` |
| Python | `actions/setup-python@v5` |
| .NET | `actions/setup-dotnet@v4` |
| Java | `actions/setup-java@v4` |
| Go | `actions/setup-go@v5` |
| Rust | `dtolnay/rust-toolchain@stable` |
| vcpkg | `lukka/run-vcpkg@v11` |

## Workflow structure

```yaml
name: Build

on:
  workflow_dispatch:          # Manual trigger
  push:
    paths: ['src/**', '*.cmake', '.github/workflows/build.yml']
  pull_request:
    branches: ["main"]
    paths: ['src/**', '*.cmake', '.github/workflows/build.yml']

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      fail-fast: false        # Don't cancel other jobs on failure
      matrix:
        include:
          - os: windows-latest
            # platform-specific variables
          - os: ubuntu-latest
            # platform-specific variables

    steps:
      - uses: actions/checkout@v4

      - name: Install dependencies
        uses: some/action@v1    # USE EXISTING ACTIONS
        with:
          cache: true           # Most actions support caching

      - name: Configure
        run: cmake -B build

      - name: Build
        run: cmake --build build
```

## Path filters

Only trigger on relevant file changes to save CI minutes:

```yaml
on:
  push:
    paths:
      - 'src/**'
      - 'CMakeLists.txt'
      - '.github/workflows/this-workflow.yml'
```

## Matrix builds

Use `include` for explicit configurations (clearer than exclude):

```yaml
matrix:
  include:
    - os: windows-latest
      compiler: cl
      preset: windows
    - os: ubuntu-latest
      compiler: gcc
      preset: linux
    - os: ubuntu-latest
      compiler: clang
      preset: linux-clang
```

## Caching

Most setup actions have built-in caching. Enable it:

```yaml
- uses: actions/setup-node@v4
  with:
    node-version: '20'
    cache: 'npm'              # Built-in cache support
```

For custom caching:

```yaml
- uses: actions/cache@v4
  with:
    path: ~/.cache/my-tool
    key: ${{ runner.os }}-my-tool-${{ hashFiles('**/lockfile') }}
```

## Validate YAML before pushing

```sh
python -c "import yaml; yaml.safe_load(open('.github/workflows/build.yml')); print('Valid')"
```

## Debugging failed workflows

1. Check the failing step's logs in GitHub Actions UI
2. Add debug output: `run: echo "VAR=${{ env.VAR }}"`
3. Use `workflow_dispatch` trigger to manually re-run with changes
4. For complex issues, use `tmate` action for SSH access:
   ```yaml
   - uses: mxschmitt/action-tmate@v3
     if: failure()
   ```

## Common mistakes

- **Manual installation when action exists** - Always search first
- **Missing path filters** - Wastes CI minutes on irrelevant changes
- **Hardcoded paths** - Use `${{ github.workspace }}` instead
- **No caching** - Dramatically slows down builds
- **fail-fast: true (default)** - Set to false to see all failures

## References

- Workflow syntax: https://docs.github.com/en/actions/reference/workflow-syntax-for-github-actions
- Marketplace: https://github.com/marketplace?type=actions
