# -*- coding: utf-8 -*-
from . import backend
from typing import Dict, List, Tuple, Union, Set, Any
import os
from .pydantic_filepattern import create_pydantic_fp, get_pydantic_fp


class PatternObject:

    def __init__(self, file_pattern, block_size):
        self._file_pattern = file_pattern
        self._block_size = block_size

    def get_matching(self, kwargs) -> List[Tuple[Dict[str, Union[int, float, str]], List[os.PathLike]]]:
        """Get all filenames matching specific values

        This method will return a list containing all files where the variable matches the supplied. For example,
        if the argument `x=1` is passed to get matching, all files where x is 1 will be returned. A list of values
        can also be passed, such as `x=[1,2,3]`. Furthermore, an arbitrary number of variables and values can be passed,
        such as `x=1, y=2, z=3` or `x=[1,2,3], y=['a', 'b', 'c'], z=[4, 5, 6]`.

        Args:
            kwargs: One or more keyword arguments where the key is a variable contained in the filepattern and
                    the value is a value for the variable

        Returns:
            List of matching files
        """

        vars = self.get_variables()

        mapping = []
        pydantic_output = False
        for key, value in kwargs.items():

            if (key == 'pydantic_output'):
                pydantic_output = value
                continue

            if (key not in vars and key != ""):
                raise ValueError("Variable \"" + key + "\" is not a valid variable. The variables are: " + str(vars) + ".")

            if (not isinstance(value, list)):
                value = [value]
            mapping.append((key, value))

        if self._block_size == "":

            files = self._file_pattern.getMatching(mapping)

            if (pydantic_output):

                if (len(files) > 0):

                    file = files[0]

                    self.FilepatternModel = get_pydantic_fp(file)

                    for i in range(len(files)):
                        temp = files[i]
                        file_dict = temp[0]
                        file_dict['path'] = temp[1]
                        files[i] = self.FilepatternModel(**file_dict)

            return files

        else:
            return self._get_matching_out_of_core(mapping, pydantic_output)

    def _get_matching_out_of_core(self, mapping, pydantic_output=False) -> List[Tuple[Dict[str, Union[int, float, str]], List[os.PathLike]]]:
        """get_matching functionality for out of core algorithms

        This method is called by get_mapping and should not be used directly.

        This function will yield blocks of the get_matching functionality for external memory filepattern objects.
        """

        self._file_pattern.getMatching(mapping)

        while True:
            matching = self._get_matching_block()
            if len(matching) == 0:
                break

            for match in matching:

                if (pydantic_output):

                    if (len(match) > 0):

                        self.FilepatternModel = get_pydantic_fp(match)

                        file_dict = match[0]
                        file_dict['path'] = match[1]
                        match = self.FilepatternModel(**file_dict)

                yield match

    def _get_matching_block(self) -> List[Tuple[Dict[str, Union[int, float, str]], List[os.PathLike]]]:
        """
        Returns block of matching files of size less than or equal to block_size.

        This method should not be called directly. Must be called after making a call to get_matching.

        Returns:
            List containing a block of matching files.
        """

        return self._file_pattern.getMatchingBlock()

    def get_occurrences(self, mapping: List[Tuple[str, List[Union[int, float, str]]]]) -> Dict[str, Dict[Union[int, float, str], int]]:
        """
        Returns the unique values for each variable along with the number of occurrences for each value.

        Args:
            **kwargs: Each keyword argument must be a variable. If no arguments are supplied, the occurrences
            for every variable will be returned.

        Returns:
            Dictionary of variables mapped to values where each value is mapped to the number of occurrences.
        """

        return self._file_pattern.getOccurrences(mapping)

    def get_unique_values(self, variables: List[str]) -> Dict[str, Set[Union[int, float, str]]]:
        """Returns the unique values for each variable.

        This method returns a dictionary of provided variables to a list of all unique occurrences. If no variables are provided,
        all variables will be returned.

        Args:
            **args: Variables to get the occurrences of. All variables will be returned if no arguments are provided.

        Returns:
            Dictionary of variables mapped to values.
        """

        vars = self.get_variables()

        for var in variables:
            if (var not in vars and var != ""):
                raise ValueError("Variable \"" + var + "\" is not a valid variable. The variables are: " + str(vars) + ".")

        return self._file_pattern.getUniqueValues(variables)

    def output_name(self, files: List[Tuple[Dict[str, Union[int, float, str]], List[os.PathLike]]] = []) -> str:
        """Returns a single filename that captures variables from a list of files.

        Given a list of files, this method will return a single filename that captures the variables from each
        file in the list. If a variable is constant through the list, the variable value will be in the returned
        name. If a variable is not constant, the minimum and maximum values will appear in the returned name in
        the form "(min-max)".

        Args:
            files: List of files to get a single filename of.

        Returns:
            A string that captures the variable values from each file in files.

        """

        return self._file_pattern.outputName(files)

    def __len__(self) -> int:

        return self._file_pattern.length()

    def get_variables(self) -> List[str]:

        return self._file_pattern.getVariables()

    def __call__(self,
                 group_by: Union[str, List[str]] = "",
                 pydantic_output: bool = False) -> Union[List[Tuple[List[Tuple[str, Union[str, int, float]]],
                                                                    List[Tuple[Dict[str, Union[int, float, str]], List[os.PathLike]]]]],
                                                         Tuple[Dict[str, Union[int, float, str]], List[os.PathLike]]]:
        """Iterate through files parsed using a filepattern

        This method returns an iterable of filenames matched to the filepattern.

        If a group_by variable is provided, lists of files where the variable is held constant are
        returned on each call.

        Note that the `group_by` argument works in the inverse of the previous version of `filepattern`.
        The variable passed to `group_by` will be held constant rather than the other variables remaining constant.

        Args:
            group_by: A string consisting of a single variable or a list of variables to group filenames by.
            pydantic_output: Get Pydantic models as the output
        """

        vars = self.get_variables()

        if (isinstance(group_by, str)):
            if (group_by not in vars and group_by != ""):
                raise ValueError("Variable \"" + group_by + "\" is not a valid variable. The variables are: " + str(vars) + ".")

        else:
            for var in group_by:
                if var not in vars:
                    raise ValueError("Variable \"" + var + "\" is not a valid variable. The variables are: " + str(vars) + ".")

        self.pydantic_iterator = pydantic_output

        if (self.pydantic_iterator):

            if (self.__len__() > 0):
                file = self.__getitem__(0)

                self.FilepatternModel = get_pydantic_fp(file)

            else:  # list of files is empty

                # get variables
                variables = self.get_variables()

                variable_map = {}

                # add paths to map
                variable_map["path"] = (list, ...)

                for variable in variables:
                    variable_map[variable] = (Any, ...)

                self.FilepatternModel = create_pydantic_fp(variable_map)

        if (group_by == []):
            group_by = ['*__all__*']

        if (isinstance(group_by, str)):
            group_by = [group_by]

        self._file_pattern.setGroup(group_by)

        if self._block_size == "":

            if len(group_by) == 0 or group_by[0] != "":
                self._file_pattern.groupBy(group_by)

            return self

        if len(group_by) == 0 or (group_by != [""] and len(group_by) != 1):
            self._file_pattern.setGroup(group_by)

        return self

    def _length(self) -> int:
        """Get length of current block for out of core algorithms
        """
        return self._file_pattern.currentBlockLength()

    def __iter__(self) -> Union[List[Tuple[List[Tuple[str, Union[str, int, float]]], List[Tuple[Dict[str, Union[int, float, str]], List[os.PathLike]]]]],
                                Tuple[Dict[str, Union[int, float, str]], List[os.PathLike]]]:
        """Yields files from files that match the filepattern

        Yields:
            Union[List[Tuple[List[Tuple[str, Union[str, int, float]]], List[Tuple[Dict[str, Union[int, float, str]], List[os.PathLike]]]]],
                  Tuple[Dict[str, Union[int, float, str]], List[os.PathLike]]] : Returns single file when group_by is not used and list of files otherwise
        """

        if self._block_size == "":
            for file in self._file_pattern.iterator():
                if (self.pydantic_iterator):

                    if (isinstance(file[0], dict)):
                        map_with_path = file[0]
                        map_with_path['path'] = file[1]

                        yield self.FilepatternModel(**map_with_path)

                    else:
                        for i in range(len(file[1])):

                            map_with_path = file[1][i][0]
                            map_with_path['path'] = file[1][i][1]

                            file[1][i] = self.FilepatternModel(**map_with_path)

                        yield file
                else:
                    yield file
        else:
            while True:
                for block in self._file_pattern.iteratorExternal():

                    if self._length() == 0:
                        break

                    if (self.pydantic_iterator):

                        if (isinstance(block[0], dict)):
                            map_with_path = block[0]
                            map_with_path['path'] = block[1]

                            yield self.FilepatternModel(**map_with_path)

                        else:
                            for i in range(len(block[1])):

                                map_with_path = block[1][i][0]
                                map_with_path['path'] = block[1][i][1]

                                block[1][i] = self.FilepatternModel(**map_with_path)

                            yield block
                    else:
                        yield block

                if self._length() == 0:
                    break

    def __getitem__(self, key) -> Union[List[Tuple[Dict[str, Union[int, float, str]], List[os.PathLike]]],
                                        Tuple[Dict[str, Union[int, float, str]], List[os.PathLike]]]:
        """Get slices of files that match the filepattern

        Slices of files can be retrieved using [] operator. Files can be accessed using a single index
        such as fp[1] or slices of files, such as fp[:10], f[1:10], or fp[1:2:10].

        Args:
            key (int): Index of file

        Returns:
            Union[List[Tuple[Dict[str, Union[int, float, str]], List[os.PathLike]]],
                Tuple[Dict[str, Union[int, float, str]], List[os.PathLike]]]: Returns single file for a single index or a List
                of files for a slice.
        """

        if (isinstance(key, int)):
            return self._file_pattern.getItem(key)
        if (isinstance(key, list)):
            return self._file_pattern.getItemList(key)

        slc = [key.start, key.stop, key.step]
        if (slc[0] is None):
            slc[0] = 'None'
        if (slc[1] is None):
            slc[1] = 'None'
        if (slc[2] is None):
            slc[2] = 'None'

        return self._file_pattern.getSlice(slc)


class FilePattern(PatternObject):
    """
    Class to create a FilePattern object.

    This class take in in 4 arguments: path, pattern, block_size, and recursive. For the path,
    either a path to a directory, text file, or stitching vector may be provided. ``filepattern`` will
    then match the filenames in the directory, or each line of the text file, to the provided ``pattern``.

    The ``block_size`` parameter allows for out of core processing, which consume ``block_size`` amount of memory at most.

    The ``recursive`` parameter enables recursive iteration of subdirectories when a directory is passed as ``path``. In
    this case ``filepattern`` will iterate over the subdirectories, storing filenames with the same basename in the same
    group.


    Args:
            path: Path to directory or text file
            pattern: Pattern to compare each filename to
            block_size: Maximum amount of RAM to consume at once. Defaults to "".
            recursive: Iterate over subdirectories. Defaults to False.
    """

    def __init__(
        self,
        path: str,
        pattern: str = "",
        block_size: str = "",
        recursive: bool = False,
        suppress_warnings=False
    ):
        """Constructor of the FilePattern class. The path argument can either be a directory, a text file,
        or a stitching vector. Passing in the optional argument `block_size` will
        create an ExternalFilePattern object, which will process the directory in blocks which consume less
        than or equal to `block_size` of memory.

        Just the path may be passed in the pattern is contained within the path. In this case,
        the names of the subdirectories are captured if they are named is the same manner as the pattern.
        For example, if just the path 'path/to/files/{channel: c+}/img_r{r:d+}_c{c:d+}.tif' is passed,
        the names of the channel subdirectories will be captured for each file.

        Args:
            path: Path to directory or text file
            pattern: Pattern to compare each filename to
            block_size: Maximum amount of RAM to consume at once. Defaults to "".
            recursive: Iterate over subdirectories. Defaults to False.
            suppress_warnings: True to suppress warning printed to console. Defaults to False.
        """

        path = str(path)  # change path type to string to support pathlib paths

        self._file_pattern = backend.FilePattern(path, pattern, block_size, recursive, suppress_warnings)

        super().__init__(self._file_pattern, block_size)

    def get_matching(self, **kwargs) -> List[Tuple[Dict[str, Union[int, float, str]], List[os.PathLike]]]:

        """Get all filenames matching specific values

        This method will return a list containing all files where the variable matches the supplied. For example,
        if the argument `x=1` is passed to get matching, all files where x is 1 will be returned. A list of values
        can also be passed, such as `x=[1,2,3]`. Furthermore, an arbitrary number of variables and values can be passed,
        such as `x=1, y=2, z=3` or `x=[1,2,3], y=['a', 'b', 'c'], z=[4, 5, 6]`.

        Example:

            For a directory containing the files
            ```
                img_r001_c001_DAPI.tif
                img_r002_c001_DAPI.tif
                img_r001_c001_TXREAD.tif
                img_r002_c001_TXREAD.tif
                img_r001_c001_GFP.tif
                img_r002_c001_GFP.tif
            ```

            The `get_matching` method can be used as:

            ```
                path = /path/to/directory

                pattern = 'img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif'

                files = fp.FilePattern(path, pattern)

                matching = files.get_matching(channel=['TXREAD'])
            ```

            the `matching` variable will be a list of matching files:

            ```
                [({'c': 1, 'channel': 'TXREAD', 'r': 1},
                ['/home/ec2-user/Dev/FilePattern/data/example/img_r001_c001_TXREAD.tif']),
                ({'c': 1, 'channel': 'TXREAD', 'r': 2},
                ['/home/ec2-user/Dev/FilePattern/data/example/img_r002_c001_TXREAD.tif'])]
            ```

        Args:
            **kwargs: One or more keyword arguments where the key is a variable contained in the filepattern and
                    the value is a value for the variable. Use pydantic_output=True to get Pydantic models as the output.

        Returns:
            List of matching files
        """

        return super().get_matching(kwargs)

    def get_occurrences(self, **kwargs) -> Dict[str, Dict[Union[int, float, str], int]]:
        """
        Takes in a variable as the key and a list of values as the value and returns the a dictionary
        mapping the variable to a dictionary of the values mapped to the number of occurrences of the variable
        value.

        For example, if the filepattern is `img_r{r:ddd}_c{r:ddd}.tif` and r=1 occurs 20 times in the path,
        then the passing `r=[1]` will return `{'r': {1: 20}}`.

        Args:
            **kwargs: Each keyword argument must be a variable. If no arguments are supplied, the occurrences
            for every variable will be returned.

        Returns:
            Dictionary of variables mapped to values where each value is mapped to the number of occurrences.
        """

        vars = self.get_variables()

        mapping = []
        for key, value in kwargs.items():

            if (key not in vars and key != ""):
                raise ValueError("Variable \"" + key + "\" is not a valid variable. The variables are: " + str(vars) + ".")

            mapping.append((key, value))

        return super(FilePattern, self).get_occurrences(mapping)

    def get_unique_values(self, *args) -> Dict[str, Set[Union[int, float, str]]]:
        """Given variable names from the filepattern as arguments, this method returns a dictionary
        of mapping the variable names to a set of the unique values for each variable. If no variables are
        provided, all variables will be returned.

        For example if the filepattern is `img_r{r:ddd}_c{r:ddd}.tif` and `r` ranges from 1 to 3 and c ranges from 1 to 2,
        then fp_object.get_unique_values('r', 'c') will return `{'r': {1,2,3}, 'c': {1,2}}`.

        Args:
            **args: Variables to get the occurrences of. All variables will be returned if no arguments are provided.

        Returns:
            Dictionary of variables mapped to values.
        """
        vec = []
        for str in args:
            vec.append(str)

        return super().get_unique_values(vec)

    def output_name(self, files: list = []) -> str:
        """Returns a single filename that captures variables from a list of files.

        Given a list of files, this method will return a single filename that captures the variables from each
        file in the list. If a variable is constant through the list, the variable value will be in the returned
        name. If a variable is not constant, the minimum and maximum values will appear in the returned name in
        the form "(min-max)".

        Args:
            files: List of files to get a single filename of.

        Returns:
            A string that captures the variable values from each file in files.

        """

        return super().output_name(files)

    def get_variables(self) -> List[str]:
        """ Returns a list of variables that are contained in the filepattern

        For example, if the filepattern is `img_x{x:d}_y{y:d}_c{c:c+}.tif`, get_variables will return
        the list `[x, y, c]`.

        Returns:
            List containing the variables in the filepattern

        """

        return super().get_variables()
