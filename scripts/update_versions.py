#!/usr/bin/env python3
"""
Update version files for filepattern project.

This script:
1. Reads the version from setuptools-scm (git tags)
2. Writes it to the VERSION file (for CMake)
3. Updates the version in pom.xml (for Maven)

Usage:
    python scripts/update_versions.py [--version VERSION]

If --version is not provided, it will be auto-detected from git.
"""
import argparse
import re
import sys
from pathlib import Path


def get_version_from_scm():
    """Get version from setuptools-scm."""
    try:
        from setuptools_scm import get_version
        version = get_version(root='..', fallback_version='0.0.0')
        # Extract base version (remove dev/local version identifiers)
        base_version = version.split('+')[0].split('.dev')[0]
        # Don't return if it's the fallback version
        if base_version == '0.0.0':
            print("Warning: setuptools-scm returned fallback version, trying git directly", file=sys.stderr)
            return None
        return base_version
    except ImportError:
        print("Warning: setuptools-scm not installed, trying git directly", file=sys.stderr)
        return None
    except Exception as e:
        print(f"Warning: Could not get version from setuptools-scm: {e}", file=sys.stderr)
        return None


def get_version_from_git():
    """Get version from git tags as fallback."""
    import subprocess

    # Try to get the latest tag
    try:
        result = subprocess.run(
            ['git', 'describe', '--tags', '--abbrev=0'],
            capture_output=True,
            text=True,
            check=True,
            cwd=Path(__file__).parent.parent
        )
        version = result.stdout.strip()
        # Remove 'v' prefix if present
        if version.startswith('v'):
            version = version[1:]
        return version
    except subprocess.CalledProcessError:
        # If no tags, try to get from git tag list
        try:
            result = subprocess.run(
                ['git', 'tag', '--sort=-version:refname'],
                capture_output=True,
                text=True,
                check=True,
                cwd=Path(__file__).parent.parent
            )
            tags = result.stdout.strip().split('\n')
            if tags and tags[0]:
                version = tags[0]
                if version.startswith('v'):
                    version = version[1:]
                return version
        except Exception:
            pass
    except Exception as e:
        print(f"Warning: Could not get version from git: {e}", file=sys.stderr)

    return None


def write_version_file(version, root_dir):
    """Write .version file for CMake."""
    version_file = root_dir / '.version'
    version_file.write_text(version + '\n')
    print(f"[OK] Updated {version_file} to {version}")


def update_pom_xml(version, root_dir):
    """Update version in pom.xml."""
    pom_file = root_dir / 'pom.xml'
    if not pom_file.exists():
        print(f"Warning: {pom_file} not found, skipping", file=sys.stderr)
        return

    content = pom_file.read_text()

    # Update the version tag (first occurrence after <artifactId>filepattern</artifactId>)
    pattern = r'(<artifactId>filepattern</artifactId>\s*<version>)[^<]+(</version>)'
    replacement = rf'\g<1>{version}\g<2>'

    new_content, count = re.subn(pattern, replacement, content, count=1)

    if count == 0:
        print(f"Warning: Could not find version tag in {pom_file}", file=sys.stderr)
        return

    pom_file.write_text(new_content)
    print(f"[OK] Updated {pom_file} to {version}")


def main():
    parser = argparse.ArgumentParser(description='Update version files for filepattern')
    parser.add_argument('--version', help='Version to set (auto-detected from git if not provided)')
    parser.add_argument('--root', default='.', help='Root directory of the project')
    args = parser.parse_args()

    root_dir = Path(args.root).resolve()

    # Get version
    if args.version:
        version = args.version
        print(f"Using provided version: {version}")
    else:
        # Try setuptools-scm first, then fall back to git
        version = get_version_from_scm()
        if not version:
            version = get_version_from_git()

        if not version:
            print("Error: Could not determine version. Please provide it with --version", file=sys.stderr)
            sys.exit(1)

        print(f"Auto-detected version: {version}")

    # Validate version format
    if not re.match(r'^\d+\.\d+\.\d+$', version):
        print(f"Warning: Version '{version}' does not match expected format (x.y.z)", file=sys.stderr)

    # Update files
    write_version_file(version, root_dir)
    update_pom_xml(version, root_dir)

    print(f"\n[OK] All version files updated to {version}")


if __name__ == '__main__':
    main()
