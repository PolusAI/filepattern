# -*- coding: utf-8 -*-
from __future__ import absolute_import, unicode_literals

from .filepattern import FilePattern

from .functions import infer_pattern, get_regex, get_variables

__all__ = ["FilePattern", "infer_pattern", "get_regex"]

# Get version from setuptools-scm
try:
    from importlib.metadata import version, PackageNotFoundError
except ImportError:
    # Python < 3.8
    from importlib_metadata import version, PackageNotFoundError

try:
    __version__ = version("filepattern")
except PackageNotFoundError:
    # Package is not installed
    __version__ = "unknown"
