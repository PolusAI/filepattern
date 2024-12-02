# -*- coding: utf-8 -*-
from . import backend
from typing import List
# Reference/Functions

def get_variables(filepattern: str, suppress_warnings=False) -> List[str]:
    """Returns a list containing the variables in a filepattern.

    Args:
        filepattern: A filepattern to get the regex equivalent of.
        suppress_warnings: True to suppress warnings (Defaults False)

    Returns:
        A list of strings containing the variables in the filepattern
    """

    return backend.FilePattern.getVariablesFromPattern(filepattern, suppress_warnings)

def get_regex(filepattern: str, suppress_warnings=False) -> str:
    """Returns the regex equivalent of the filepattern.

    Args:
        filepattern: A filepattern to get the regex equivalent of.
        suppress_warnings: True to suppress warnings (Defaults False)

    Returns:
        String containing the regex equivalent of the filepattern

    """
    return backend.FilePattern.getRegex(filepattern, suppress_warnings)
    


def infer_pattern(
    path: str = "", files: list = [], variables: str = "", block_size: str = ""
):
    """Returns a guess of a pattern given path to a directory of files or a list of files.

    This function takes in either a path to a directory or a list of filenames  to provide a guess
    of the filepattern. The optional argument `variables` will provide names for the variables.
    If variable names are not given, default variable names will be used. If a `block_size` is
    specified, this method will only used the specified amount of RAM.

    Args:
        path: The path to a directory of files. Defaults to "".
        files: A list of files. Defaults to [].
        variables: A string of variables. If an empty string, variable names will be provided. Defaults to "".
        block_size: An string that specifies a maximum amount of RAM to consume. If "", no limit will be imposed. Defaults to "".

    Returns:
        A string that is a guess of the pattern for the supplied filenames.
    """
    if path == "" and files == []:
        raise ValueError("A path or list of files must be provided")
    elif path != "" and files != []:
        raise ValueError("Pass in only a path or list of files, not both.")

    if files == []:
        return backend.FilePattern.inferPattern(str(path), str(variables), str(block_size))
    else:
        return backend.FilePattern.inferPattern(files, variables)
