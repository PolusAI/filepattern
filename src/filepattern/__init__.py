from __future__ import absolute_import, unicode_literals

from .filepattern import FilePattern

from .functions import infer_pattern, get_regex

__all__ = ["FilePattern"]
from . import _version
__version__ = _version.get_versions()['version']
