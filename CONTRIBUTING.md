# Contributing to filepattern

Thank you for your interest in contributing to filepattern! This document provides guidelines for contributing to the project.

## Development Setup

1. Clone the repository:
   ```bash
   git clone https://github.com/PolusAI/filepattern.git
   cd filepattern
   ```

2. Install dependencies:
   ```bash
   pip install -e ".[dev]"
   ```

3. Run tests:
   ```bash
   pytest tests/
   ```

## Commit Message Convention

We use [Conventional Commits](https://www.conventionalcommits.org/) for commit messages. This allows us to automatically generate changelogs and determine version bumps.

### Format

```
<type>: <description>

[optional body]

[optional footer(s)]
```

### Types

- **feat**: A new feature (triggers minor version bump)
- **fix**: A bug fix (triggers patch version bump)
- **docs**: Documentation only changes
- **style**: Changes that don't affect code meaning (formatting, white-space, etc.)
- **refactor**: Code change that neither fixes a bug nor adds a feature
- **perf**: Performance improvements
- **test**: Adding or updating tests
- **build**: Changes to build system or dependencies
- **ci**: Changes to CI configuration files and scripts
- **chore**: Other changes that don't modify src or test files

### Breaking Changes

For breaking changes (triggers major version bump), add `!` after the type or include `BREAKING CHANGE:` in the footer:

```
feat!: redesign the API interface

BREAKING CHANGE: The FilePattern constructor now requires a pattern argument
```

### Examples

**Good commit messages:**

```
feat: add support for nested directory patterns

fix: handle non-alphanumeric characters in infer_pattern

docs: update installation instructions for Python 3.13

perf: optimize file traversal for large directories

refactor: simplify pattern matching logic

test: add test cases for edge cases in vector patterns

build: update pybind11 to version 2.12

ci: add Python 3.13 to test matrix
```

**Bad commit messages:**

```
Update code
Fix bug
WIP
changes
asdf
```

## Release Process

Releases are automated using [release-please](https://github.com/googleapis/release-please):

1. Make commits following the conventional commit format
2. When commits are pushed to `master`, release-please analyzes them
3. release-please creates/updates a "Release PR" with:
   - Updated CHANGELOG.md
   - Version bump (based on commit types)
   - Updated version files
4. When the Release PR is merged:
   - A GitHub Release is created automatically
   - Publishing workflows are triggered (PyPI, Maven Central)

### Version Bumping

- `feat:` commits → **minor** version bump (e.g., 2.1.4 → 2.2.0)
- `fix:` commits → **patch** version bump (e.g., 2.1.4 → 2.1.5)
- `feat!:` or `BREAKING CHANGE:` → **major** version bump (e.g., 2.1.4 → 3.0.0)

You don't need to manually update version numbers - the automation handles this!

## Pull Request Guidelines

1. **Branch naming**: Use descriptive names like `feat/add-recursive-search` or `fix/issue-123`
2. **Commit messages**: Follow the conventional commit format (see above)
3. **Tests**: Add tests for new features or bug fixes
4. **Documentation**: Update documentation for user-facing changes
5. **Code style**: Follow the existing code style and conventions

## Testing

Run the test suite before submitting a PR:

```bash
# Python tests
pytest tests/

# C++ tests (if modified)
cd src/filepattern/cpp
mkdir build && cd build
cmake ..
make test
```

## Building Locally

### Python Package

```bash
# Install build dependencies
pip install build setuptools-scm

# Build wheel
python -m build

# Install locally
pip install dist/*.whl
```

### C++ Library

```bash
cd src/filepattern/cpp
mkdir build && cd build
cmake ..
make
```

### Java Package

```bash
mvn clean package
```

## Version Management

The project uses **setuptools-scm** for Python versioning, which reads versions from git tags. Version files are automatically synchronized:

- `VERSION` - Used by CMake for C++ library versioning
- `pom.xml` - Java package version
- Python version - Derived from git tags via setuptools-scm

To manually update version files (normally not needed):

```bash
python scripts/update_versions.py
```

## Questions?

If you have questions or need help, please:
- Open an issue on GitHub
- Check the [documentation](https://filepattern.readthedocs.io/)
- Review existing issues and PRs

## Code of Conduct

Please be respectful and constructive in all interactions. We aim to maintain a welcoming and inclusive community.
