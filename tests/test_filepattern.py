# -*- coding: utf-8 -*-
import filepattern as fp
import os
import pprint
import test_generate_filepattern_data
import test_filepattern_data as fp_data


class TestFilePatternFunctions():

    def test_get_regex(self):
        # Test with a pattern that includes a single variable
        pattern = 'img_{row:c}{col:dd}f{f:dd}d{channel:d}.tif'
        regex_pattern = fp.get_regex(pattern)   
        assert regex_pattern == 'img_([a-zA-Z])([0-9][0-9])f([0-9][0-9])d([0-9]).tif'

    def test_get_variables(self):
        # Test with a pattern that includes a single variable
        pattern = 'img_r{r:ddd}_c{c:ddd}.tif'
        variables = fp.get_variables(pattern)
        assert (variables == ['r', 'c'] or variables == ['c', 'r'])


class TestArrayPattern():

    test_generate_filepattern_data.generate_text_data()
    root_directory = os.path.dirname(os.path.realpath(__file__))
    filepath = root_directory + '/test_data/data100.txt'
    old_pattern = 'img_r{rrr}_c{ccc}.tif'
    patterns = ['img_r{r:ddd}_c{c:ddd}.tif', 'img_r{r:d+}_c{c:d+}.tif',
                old_pattern]
    MAX_NUM = 9

    with open(filepath, 'r') as file:
        lines = file.readlines()  # Each line includes the newline character at the end

        # To remove newline characters, use list comprehension
        file_array = [line.strip() for line in lines]

    def test_array_pattern(self):

        for pattern in self.patterns:
            # old_files = filepattern.FilePattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.file_array, pattern)

            # old_result = []
            result = []

            # for file in old_files():
            #     old_result.append(file)
            for file in files():
                result.append(file)

            pprint.pprint(result)

            assert (len(fp_data.test_fp) == len(result))

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_file_pattern_pydantic(self):

        for pattern in self.patterns:

            files = fp.FilePattern(self.file_array, pattern)

            result = []

            for file in files(pydantic_output=True):
                result.append(file)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i].r
                assert fp_data.test_fp[i][0]["c"] == result[i].c
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i].path[0])

    def test_get_matching_pydantic(self):
        for pattern in self.patterns:
            nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

            files = fp.FilePattern(self.file_array, pattern)

            result = files.get_matching(r=[nums[0]], pydantic_output=True)

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i].r
                assert fp_data.test_fp[i][0]["c"] == result[i].c
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i].path[0])

    def test_get_matching(self):
        for pattern in self.patterns:
            nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

            files = fp.FilePattern(self.file_array, pattern)

            result = files.get_matching(r=[nums[0]])

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_get_matching_empty(self):

        pattern = 'wrong_pattern{r:d}.file'

        nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

        files = fp.FilePattern(self.file_array, pattern)

        result = files.get_matching(r=[nums[0]])

        assert result == []

    def test_get_matching_no_list(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.file_array, pattern)

            result = files.get_matching(r=0, c=0)

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_get_matching_single_value(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.file_array, pattern)

            result = files.get_matching(r=[0])

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_get_matching_single_value_no_list(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.file_array, pattern)

            result = files.get_matching(r=0)

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_group_by(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.file_array, pattern)

            result = []

            for file in files(group_by="r"):
                result.append(file)

            pprint.pprint(result)

            for i in range(len(result)):
                for j in range(len(result[i])):
                    assert fp_data.fp_groupby[i][1][j][0]["r"] == result[i][1][j][0]["r"]
                    assert fp_data.fp_groupby[i][1][j][0]["c"] == result[i][1][j][0]["c"]
                    assert os.path.basename(fp_data.fp_groupby[i][1][j][1][0]) == os.path.basename(result[i][1][j][1][0])

    def test_group_by_empty(self):
        pattern = 'wrong_pattern{r:d}.file'

        files = fp.FilePattern(self.file_array, pattern)

        result = []

        for file in files(group_by="r"):
            result.append(file)

        assert result == []

    def test_group_by_pydantic(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.file_array, pattern)

            result = []

            for file in files(group_by="r", pydantic_output=True):
                result.append(file)

            pprint.pprint(result)

            for i in range(len(result)):
                for j in range(len(result[i])):
                    assert fp_data.fp_groupby[i][1][j][0]["r"] == result[i][1][j].r
                    assert fp_data.fp_groupby[i][1][j][0]["c"] == result[i][1][j].c
                    assert os.path.basename(fp_data.fp_groupby[i][1][j][1][0]) == os.path.basename(result[i][1][j].path[0])

    def test_group_by_all(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.file_array, pattern)

            result = []

            for file in files(group_by=[]):
                result.append(file)

            result = result[0][1]

            print(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_group_by_all_pydantic(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.file_array, pattern)

            result = []

            for file in files(group_by=[], pydantic_output=True):
                result.append(file)

            result = result[0][1]

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i].r
                assert fp_data.test_fp[i][0]["c"] == result[i].c
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i].path[0])
    
    def test_no_sorting(self):

        pattern = 'img_r{r:ddd}_c{c:ddd}.tif'

        data = [
            'img_r002_c001.tif',
            'img_r002_c002.tif',
            'img_r001_c001.tif',
            'img_r001_c002.tif',
        ]

        files = fp.FilePattern(data, pattern, sorted=False)

        results = []
        for file in files():
            results.append(file)

        # ensure that files remain in the same order
        for i in range(len(data)):
            assert str(results[i][1][0]) == data[i]


class TestFilePattern():

    root_directory = os.path.dirname(os.path.realpath(__file__))
    path = root_directory + '/test_data/data100'
    sorted_path = root_directory + '/test_data/sorted_data'
    old_pattern = 'img_r{rrr}_c{ccc}.tif'
    patterns = ['img_r00{r:d}_c{c:ddd}.tif', 'img_r{r:d+}_c{c:d+}.tif', old_pattern]
    MAX_NUM = 9

    test_generate_filepattern_data.generate_data()
    test_generate_filepattern_data.generate_channel_data()
    test_generate_filepattern_data.generate_sorted_data()
    test_generate_filepattern_data.generate_bracket_data()
    test_generate_filepattern_data.generate_channel_data_sc()
    test_generate_filepattern_data.generate_channel_data_spaces()
    test_generate_filepattern_data.generate_recursive_no_capture_data() # Added new generator call

    def test_file_pattern(self):

        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern)

            result = []

            for file in files():
                result.append(file)

            pprint.pprint(result)

            assert (len(fp_data.test_fp) == len(result))

            assert (len(result) == len(files))  # test length operator

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_file_pattern_pydantic(self):

        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern)

            result = []

            for file in files(pydantic_output=True):
                result.append(file)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i].r
                assert fp_data.test_fp[i][0]["c"] == result[i].c
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i].path[0])

    def test_get_matching_pydantic(self):
        for pattern in self.patterns:
            nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

            files = fp.FilePattern(self.path, pattern)

            result = files.get_matching(r=[nums[0]], pydantic_output=True)

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i].r
                assert fp_data.test_fp[i][0]["c"] == result[i].c
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i].path[0])

    def test_get_matching(self):
        for pattern in self.patterns:
            nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

            files = fp.FilePattern(self.path, pattern)

            result = files.get_matching(r=[nums[0]])

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_get_matching_empty(self):

        pattern = 'wrong_pattern{r:d}.file'

        nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

        files = fp.FilePattern(self.path, pattern)

        result = files.get_matching(r=[nums[0]])

        assert result == []

    def test_get_matching_no_list(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern)

            result = files.get_matching(r=0, c=0)

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_get_matching_single_value(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern)

            result = files.get_matching(r=[0])

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_get_matching_single_value_no_list(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern)

            result = files.get_matching(r=0)

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_group_by(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern)

            result = []

            for file in files(group_by="r"):
                result.append(file)

            pprint.pprint(result)

            for i in range(len(result)):
                for j in range(len(result[i])):
                    assert fp_data.fp_groupby[i][1][j][0]["r"] == result[i][1][j][0]["r"]
                    assert fp_data.fp_groupby[i][1][j][0]["c"] == result[i][1][j][0]["c"]
                    assert os.path.basename(fp_data.fp_groupby[i][1][j][1][0]) == os.path.basename(result[i][1][j][1][0])

    def test_group_by_empty(self):
        pattern = 'wrong_pattern{r:d}.file'

        files = fp.FilePattern(self.path, pattern)

        result = []

        for file in files(group_by="r"):
            result.append(file)

        assert result == []

    def test_group_by_pydantic(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern)

            result = []

            for file in files(group_by="r", pydantic_output=True):
                result.append(file)

            pprint.pprint(result)

            for i in range(len(result)):
                for j in range(len(result[i])):
                    assert fp_data.fp_groupby[i][1][j][0]["r"] == result[i][1][j].r
                    assert fp_data.fp_groupby[i][1][j][0]["c"] == result[i][1][j].c
                    assert os.path.basename(fp_data.fp_groupby[i][1][j][1][0]) == os.path.basename(result[i][1][j].path[0])

    def test_group_by_all(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern)

            result = []

            for file in files(group_by=[]):
                result.append(file)

            result = result[0][1]

            print(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_group_by_all_pydantic(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern)

            result = []

            for file in files(group_by=[], pydantic_output=True):
                result.append(file)

            result = result[0][1]

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i].r
                assert fp_data.test_fp[i][0]["c"] == result[i].c
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i].path[0])

    def test_named_group_direcotry(self):

        path = self.root_directory + '/test_data/recursive_data'

        path += '/(?P<dir>[a-zA-Z]+)/img_r{r:ddd}_c{c:ddd}.tif'

        for pattern in self.patterns:
            files = fp.FilePattern(path)

            result = []

            for file in files():
                result.append(file)

            # test that same number of files are returned
            assert len(result) == len(fp_data.test_recursive_fp) 

            # test that each variable and path are equal for each file in list
            for i in range(len(fp_data.test_recursive_fp)): 
                assert fp_data.test_recursive_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_recursive_fp[i][0]["c"] == result[i][0]["c"]
                assert fp_data.test_recursive_fp[i][0]["dir"] == result[i][0]["dir"]
                assert str(os.path.basename(fp_data.test_recursive_fp[i][1][0])) == os.path.basename(result[i][1][0])

            basename = ''
            # test that all basenames in vector of paths are the same
            for mapping in files:
                basename = os.path.basename(mapping[1][0])
                for filepath in mapping[1]:
                    assert basename == os.path.basename(filepath)

    def test_recursive_directory_fp(self):

        path = self.root_directory + '/test_data/recursive_data'
        filepattern = '/{directory:c+}/img_r{r:ddd}_c{c:ddd}.tif'
        files = fp.FilePattern(path, filepattern, recursive=True)

        result = []

        for file in files():
            result.append(file)

        # test that same number of files are returned
        assert len(result) == len(fp_data.test_recursive_directory_fp) 

        # test that each variable and path are equal for each file in list
        for i in range(len(fp_data.test_recursive_directory_fp)): 
            print(result[i])
            assert fp_data.test_recursive_directory_fp[i][0]["r"] == result[i][0]["r"]
            assert fp_data.test_recursive_directory_fp[i][0]["c"] == result[i][0]["c"]
            assert fp_data.test_recursive_directory_fp[i][0]["directory"] == result[i][0]["directory"]
            assert str(os.path.basename(fp_data.test_recursive_directory_fp[i][1][0])) == os.path.basename(result[i][1][0])

    def test_recursive_directory_regex_fp(self):
        # Test that recursive matching with a regex pattern
        path = self.root_directory + '/test_data/recursive_data'
        filepattern = '/(?P<directory>[a-zA-Z]+)/img_r{r:ddd}_c{c:ddd}.tif'
        files = fp.FilePattern(path, filepattern, recursive=True)

        result = []

        for file in files():
            result.append(file)

        # test that same number of files are returned
        assert len(result) == len(fp_data.test_recursive_directory_fp) 

        # test that each variable and path are equal for each file in list
        for i in range(len(fp_data.test_recursive_directory_fp)): 
            print(result[i])
            assert fp_data.test_recursive_directory_fp[i][0]["r"] == result[i][0]["r"]
            assert fp_data.test_recursive_directory_fp[i][0]["c"] == result[i][0]["c"]
            assert fp_data.test_recursive_directory_fp[i][0]["directory"] == result[i][0]["directory"]
            assert str(os.path.basename(fp_data.test_recursive_directory_fp[i][1][0])) == os.path.basename(result[i][1][0])

    def test_recursive_directory_regex_special_character_fp(self):
        # Test that recursive matching with a regex pattern
        path = self.root_directory + '/test_data/recursive_data_sc'
        filepattern = '/(?P<directory>.*)/img_r{r:ddd}_c{c:ddd}.tif'
        files = fp.FilePattern(path, filepattern, recursive=True)

        result = []

        for file in files():
            result.append(file)

        # test that same number of files are returned
        assert len(result) == len(fp_data.test_recursive_directory_fp) 

        # test that each variable and path are equal for each file in list
        for i in range(len(fp_data.test_recursive_directory_fp)): 
            print(result[i])
            assert fp_data.test_recursive_directory_fp[i][0]["r"] == result[i][0]["r"]
            assert fp_data.test_recursive_directory_fp[i][0]["c"] == result[i][0]["c"]
            assert fp_data.test_recursive_directory_fp[i][0]["directory"] + '_TEST' == result[i][0]["directory"]
            assert str(os.path.basename(fp_data.test_recursive_directory_fp[i][1][0])) == os.path.basename(result[i][1][0])

    def test_recursive_directory_spaces_fp(self):

        path = self.root_directory + '/test_data/recursive_data_spaces/'
        filepattern = 'img_r{r:ddd}_c{c:ddd}.tif'
        files = fp.FilePattern(path, filepattern, recursive=True)

        result = []

        for file in files():
            result.append(file)

        print(result)

        # test that same number of files are returned
        assert len(result) == len(fp_data.test_recursive_space) 

        # test that each variable and path are equal for each file in list
        for i in range(len(fp_data.test_recursive_space)): 
            print(result[i])
            assert fp_data.test_recursive_space[i][0]["r"] == result[i][0]["r"]
            assert fp_data.test_recursive_space[i][0]["c"] == result[i][0]["c"]
            assert str(os.path.basename(fp_data.test_recursive_space[i][1][0])) == os.path.basename(result[i][1][0])

    def test_recursive_multi_directory_regex_fp(self):
        # Test that recursive matching with a regex pattern

        path = self.root_directory + '/test_data'
        filepattern = '/.*/{directory:c+}/img_r{r:ddd}_c{c:ddd}.tif'
        files = fp.FilePattern(path, filepattern, recursive=True)

        result = []

        for file in files():
            result.append(file)

        # test that same number of files are returned
        assert len(result) == len(fp_data.test_recursive_directory_fp) 

        # test that each variable and path are equal for each file in list
        for i in range(len(fp_data.test_recursive_directory_fp)): 
            print(result[i])
            assert fp_data.test_recursive_directory_fp[i][0]["r"] == result[i][0]["r"]
            assert fp_data.test_recursive_directory_fp[i][0]["c"] == result[i][0]["c"]
            assert fp_data.test_recursive_directory_fp[i][0]["directory"] == result[i][0]["directory"]
            assert str(os.path.basename(fp_data.test_recursive_directory_fp[i][1][0])) == os.path.basename(result[i][1][0])

    def test_recursive_no_capture_group_returns_all_files(self):
        # Test that recursive matching with a non-capturing pattern
        # returns all matching files across subdirectories.
        path_to_test_dir = os.path.join(self.root_directory, 'test_data', 'recursive_no_capture_data')
        pattern_no_capture = '.*.tmp'  # Match all .tmp files
        expected_file_count = 12  # 5 in subdir1 + 7 in subdir2

        files_fp = fp.FilePattern(path_to_test_dir, pattern_no_capture, recursive=True)

        # Using len() as it seems to be the standard way to get count in existing tests
        actual_file_count = len(files_fp)

        assert actual_file_count == expected_file_count, \
            f"Expected {expected_file_count} '.tmp' files, but found {actual_file_count} " \
            f"using recursive non-capturing pattern in {path_to_test_dir}."

    def test_file_pattern_iter(self):

        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern)

            result = []

            for file in files: # test iterator without call
                result.append(file)

            assert (len(fp_data.test_fp) == len(result))

            assert (len(result) == len(files))  # test length operator

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_file_pattern_sorting(self):
        # test that numeric only, double digit numbers are sorted properly   
        sorted_pattern = '{index:d+}.tif'
        files = fp.FilePattern(self.sorted_path, sorted_pattern)

        indices = []
        for index, file in files():
            indices.append(index['index'])

        assert sorted(indices) == indices

    def test_file_pattern_brackets(self):
        # test that numeric only, double digit numbers are sorted properly

        bracket_path = self.root_directory + '/test_data/bracket_data/'

        pattern = "x\\(0-31\\)_y\\(01-48\\)_c{c:d}.ome.tif"

        files = fp.FilePattern(bracket_path, pattern)

        result = []

        for file in files: # test iterator without call
            result.append(file)

        print(result)

        for i in range(len(result)):
            result[i][0]['c'] == i
            os.path.basename(
                result[i][1][0]) == f'x(0-31)_y(01-48)_c{i}.ome.tif'


# Todo: These tests need new data to be added after replacing the old version of filepattern.
"""
    def test_group_by_multi(self):

        for pattern in self.patterns:
            old_files = filepattern.FilePattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern)

            old_result = []
            result = []

            # group by "c" instead of "r" since we changed how group by works
            for file in old_files(group_by="rc"):
                old_result = file
            for file in files(group_by=["r", "c"]):
                result.append(file[1])

            pprint.pprint(old_result)
            print()
            pprint.pprint(result)

            assert len(old_result) == len(result)

            for i in range(len(old_result)):
                assert old_result[i]["r"] == result[i][0][0]["r"]
                assert old_result[i]["c"] == result[i][0][0]["c"]
                assert str(os.path.basename(old_result[i]['file'])) == os.path.basename(result[i][0][1][0])

    def test_recursive_filepattern(self):
        path = self.root_directory + '/test_data/recursive_data'
        old_path = self.root_directory + '/test_data/recursive_data/DAPI'

        for pattern in self.patterns:
            old_files = filepattern.FilePattern(old_path, self.old_pattern)
            files = fp.FilePattern(path, pattern, recursive=True)

            old_result = []
            result = []

            # group by "c" instead of "r" since we changed how group by works
            for file in old_files():
                old_result.append(file)
            for file in files():
                result.append(file)

            # test that same number of files are returned
            assert len(old_result) == len(result)

            # test that each variable and path are equal for each file in list
            for i in range(len(old_result)):
                assert old_result[i][0]["r"] == result[i][0]["r"]
                assert old_result[i][0]["c"] == result[i][0]["c"]
                assert str(os.path.basename(old_result[i][0]['file'])) == os.path.basename(result[i][1][0])

            basename = ''
            # test that all basenames in vector of paths are the same
            for mapping in files:
                basename = os.path.basename(mapping[1][0])
                for filepath in mapping[1]:
                    assert basename == os.path.basename(filepath)

    def test_get_item(self):

        for pattern in self.patterns:
            old_files = filepattern.FilePattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern)

            old_result = []
            result = []

            for file in old_files():
                old_result.append(file)

            result = files[0:-1]

            assert len(old_result) == len(result)

            for i in range(len(old_result)):
                assert old_result[i][0]["r"] == result[i][0]["r"]
                assert old_result[i][0]["c"] == result[i][0]["c"]
                assert str(os.path.basename(old_result[i][0]['file'])) == os.path.basename(result[i][1][0])

            assert old_result[0][0]["r"] == files[0][0]["r"]
            assert old_result[0][0]["c"] == files[0][0]["c"]
            assert str(os.path.basename(old_result[0][0]['file'])) == os.path.basename(files[0][1][0])

            indices = [1, 15, 25, 37]
            result_slice = files[indices]

            for i in range(len(result_slice)):
                assert old_result[indices[i]][0]["r"] == result_slice[i][0]["r"]
                assert old_result[indices[i]][0]["c"] == result_slice[i][0]["c"]
                assert str(os.path.basename(old_result[indices[i]][0]['file'])) == os.path.basename(result_slice[i][1][0])

            assert old_result[indices[-1]][0]["r"] == result_slice[-1][0]["r"]
            assert old_result[indices[-1]][0]["c"] == result_slice[-1][0]["c"]
            assert str(os.path.basename(old_result[indices[-1]][0]['file'])) == os.path.basename(result_slice[-1][1][0])

            result_slice = files[1::2]
            old_result_slice = old_result[1::2]
            for i in range(0, len(old_result_slice)):
                assert old_result_slice[i][0]["r"] == result_slice[i][0]["r"]
                assert old_result_slice[i][0]["c"] == result_slice[i][0]["c"]
                assert str(os.path.basename(old_result_slice[i][0]['file'])) == os.path.basename(result_slice[i][1][0])
"""


class TestStringPattern():

    test_generate_filepattern_data.generate_text_data()

    root_directory = os.path.dirname(os.path.realpath(__file__))

    path = root_directory + '/test_data/data100'

    filepath = root_directory + '/test_data/data100.txt'

    old_pattern = 'img_r{rrr}_c{ccc}.tif'

    patterns = ['img_r{r:ddd}_c{c:ddd}.tif', 'img_r{r:d+}_c{c:d+}.tif', old_pattern]

    MAX_NUM = 9

    def test_file_pattern(self):
        for pattern in self.patterns:
            # old_files = filepattern.FilePattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern)

            # old_result = []
            result = []

            # for file in old_files():
            #     old_result.append(file)
            for file in files():
                result.append(file)

            pprint.pprint(result)

            assert (len(fp_data.test_fp) == len(result))

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_get_matching(self):
        for pattern in self.patterns:
            nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

            files = fp.FilePattern(self.path, pattern)

            result = files.get_matching(r=[nums[0]])

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_group_by(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern)

            result = []

            for file in files(group_by="r"):
                result.append(file)

            pprint.pprint(result)

            for i in range(len(result)):
                for j in range(len(result[i])):
                    assert fp_data.fp_groupby[i][1][j][0]["r"] == result[i][1][j][0]["r"]
                    assert fp_data.fp_groupby[i][1][j][0]["c"] == result[i][1][j][0]["c"]
                    assert os.path.basename(fp_data.fp_groupby[i][1][j][1][0]) == os.path.basename(result[i][1][j][1][0])


""" Todo: update tests to use new test dataset
    def test_group_by_multi(self):

        for pattern in self.patterns:
            old_files = filepattern.FilePattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern)

            old_result = []
            result = []

            # group by "c" instead of "r" since we changed how group by works
            for file in old_files(group_by="rc"):
                old_result = file
            for file in files(group_by=["r", "c"]):
                result.append(file[1])

            pprint.pprint(old_result)
            print()
            pprint.pprint(result)

            assert len(old_result) == len(result)

            for i in range(len(old_result)):
                assert old_result[i]["r"] == result[i][0][0]["r"]
                assert old_result[i]["c"] == result[i][0][0]["c"]
                assert str(os.path.basename(old_result[i]['file'])) == os.path.basename(result[i][0][1][0])

    def test_get_item(self):

        for pattern in self.patterns:
            old_files = filepattern.FilePattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.filepath, pattern)

            old_result = []
            result = []

            for file in old_files():
                old_result.append(file)

            result = files[0:-1]

            assert len(old_result) == len(result)

            for i in range(len(old_result)):
                assert old_result[i][0]["r"] == result[i][0]["r"]
                assert old_result[i][0]["c"] == result[i][0]["c"]
                assert str(os.path.basename(old_result[i][0]['file'])) == result[i][1][0]

            assert old_result[0][0]["r"] == files[0][0]["r"]
            assert old_result[0][0]["c"] == files[0][0]["c"]
            assert str(os.path.basename(old_result[0][0]['file'])) == files[0][1][0]

            indices = [1, 15, 25, 37]
            result_slice = files[indices]

            for i in range(len(result_slice)):
                assert old_result[indices[i]][0]["r"] == result_slice[i][0]["r"]
                assert old_result[indices[i]][0]["c"] == result_slice[i][0]["c"]
                assert str(os.path.basename(old_result[indices[i]][0]['file'])) == result_slice[i][1][0]

            assert old_result[indices[-1]][0]["r"] == result_slice[-1][0]["r"]
            assert old_result[indices[-1]][0]["c"] == result_slice[-1][0]["c"]
            assert str(os.path.basename(old_result[indices[-1]][0]['file'])) == result_slice[-1][1][0]

            result_slice = files[1::2]
            old_result_slice = old_result[1::2]
            for i in range(0, len(old_result_slice)):
                assert old_result_slice[i][0]["r"] == result_slice[i][0]["r"]
                assert old_result_slice[i][0]["c"] == result_slice[i][0]["c"]
                assert str(os.path.basename(old_result_slice[i][0]['file'])) == result_slice[i][1][0]
"""


class TestVectorPattern():

    root_directory = os.path.dirname(os.path.realpath(__file__))

    path = root_directory + '/test_vectorpattern_data.txt'

    old_pattern = 'x{xx}_y{yy}_wx{z}_wy{r}_c{c}.ome.tif'

    patterns = ['x{x:dd}_y{y:dd}_wx{z:d}_wy{r:d}_c{c:d}.ome.tif', old_pattern]

    MAX_NUM = 9

    def test_file_pattern(self):

        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern)

            result = []

            for file in files():
                result.append(file)
            assert (len(fp_data.test_vp) == len(result))

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_vp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_vp[i][0]["c"] == result[i][0]["c"]
                assert fp_data.test_vp[i][0]['correlation'] == result[i][0]['correlation']
                assert fp_data.test_vp[i][0]['gridX'] == result[i][0]['gridX']
                assert fp_data.test_vp[i][0]['gridY'] == result[i][0]['gridY']
                assert fp_data.test_vp[i][0]['posX'] == result[i][0]['posX']
                assert fp_data.test_vp[i][0]['posY'] == result[i][0]['posY']
                assert fp_data.test_vp[i][0]['x'] == result[i][0]['x']
                assert fp_data.test_vp[i][0]['y'] == result[i][0]['y']
                assert fp_data.test_vp[i][0]['z'] == result[i][0]['z']
                assert os.path.basename(fp_data.test_vp[i][1][0]) == os.path.basename(result[i][1][0])

    """
    def test_get_matching(self):
         for pattern in self.patterns:
            nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

            files = fp.FilePattern(self.path, pattern)

            result = files.get_matching(r=[nums[0]])

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_group_by(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern)

            result = []

            for file in files(group_by="r"):
                result.append(file)

            pprint.pprint(result)

            for i in range(len(result)):
                for j in range(len(result[i])):
                    assert fp_data.fp_groupby[i][1][j][0]["r"] == result[i][1][j][0]["r"]
                    assert fp_data.fp_groupby[i][1][j][0]["c"] == result[i][1][j][0]["c"]
                    assert os.path.basename(fp_data.fp_groupby[i][1][j][1][0]) == os.path.basename(result[i][1][j][1][0])



    def test_group_by_multi(self):

        for pattern in self.patterns:
            old_files = filepattern.VectorPattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern)

            old_result = []
            result = []

            # group by "c" instead of "r" since we changed how group by works
            for file in old_files(group_by="xyzrc"):
                old_result = file
            for file in files(group_by=["x", "y", "z", "r", "c"]):
                result.append(file[1])

            assert len(old_result) == len(result)

            old_result = sorted(old_result, key=lambda k: k['file']) # Old version does not sort results

            for i in range(len(old_result)):
                for key in result[i][0][0]:
                    assert str(old_result[i][key]) == str(result[i][0][0][key])

                assert str(os.path.basename(old_result[i]['file'])) == os.path.basename(result[i][0][1][0])

    def test_get_item(self):
        for pattern in self.patterns:

            old_files = filepattern.VectorPattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern)

            old_result = []
            result = []

            for file in old_files():
                old_result.append(file)
            old_result = sorted(old_result, key=lambda k: k[0]['file'])

            result = files[0:-1]

            assert len(old_result) == len(result)


            for i in range(len(old_result)):
                for key in result[i][0]:
                    assert int(old_result[i][0][key]) == result[i][0][key]
                assert str(os.path.basename(old_result[i][0]['file'])) == os.path.basename(result[i][1][0])

            for key in files[-1][0]:
                assert str(old_result[-1][0][key]) == str(files[-1][0][key])
            assert str(os.path.basename(old_result[-1][0]['file'])) == files[-1][1][0]

            indices = [1, 15, 25, 37]
            result_slice = files[indices]

            for i in range(len(result_slice)):
                for key in result[i][0]:
                    assert str(old_result[i][0][key]) == str(result[i][0][key]) # Old version stores value as string
                assert os.path.basename(old_result[i][0]['file']) == os.path.basename(result[i][1][0])

            result_slice = files[1::2]
            old_result_slice = old_result[1::2]

            assert len(old_result_slice) == len(result_slice)

            for i in range(len(old_result)):
                for key in result[i][0]:
                    assert int(old_result[i][0][key]) == result[i][0][key]
                assert str(os.path.basename(old_result[i][0]['file'])) == os.path.basename(result[i][1][0])

    """


class TestExternalFilePattern():
    root_directory = os.path.dirname(os.path.realpath(__file__))

    path = root_directory + '/test_data/data100'

    old_pattern = 'img_r{rrr}_c{ccc}.tif'

    patterns = ['img_r{r:ddd}_c{c:ddd}.tif', 'img_r{r:d+}_c{c:d+}.tif']

    block_sizes = ['300 KB']

    MAX_NUM = 1

    def test_file_pattern(self):
        for pattern in self.patterns:
            # old_files = filepattern.FilePattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern, block_size=self.block_sizes[0])

            # old_result = []
            result = []

            # for file in old_files():
            #     old_result.append(file)
            for file in files():
                result.append(file)

            pprint.pprint(result)

            assert (len(fp_data.test_fp) == len(result))

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_file_pattern_pydantic(self):
        for pattern in self.patterns:
            # old_files = filepattern.FilePattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern, block_size=self.block_sizes[0])

            # old_result = []
            result = []

            # for file in old_files():
            #     old_result.append(file)
            for file in files(pydantic_output=True):
                result.append(file)

            pprint.pprint(result)

            assert (len(fp_data.test_fp) == len(result))

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i].r
                assert fp_data.test_fp[i][0]["c"] == result[i].c
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i].path[0])

    def test_get_matching(self):
        for pattern in self.patterns:
            nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

            files = fp.FilePattern(self.path, pattern, block_size=self.block_sizes[0])

            result = []
            for match in files.get_matching(r=[nums[0]]):
                result.append(match)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_get_matching_pydantic(self):
        for pattern in self.patterns:
            nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

            files = fp.FilePattern(self.path, pattern, block_size=self.block_sizes[0])

            result = []
            for match in files.get_matching(r=[nums[0]], pydantic_output=True):
                result.append(match)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i].r
                assert fp_data.test_fp[i][0]["c"] == result[i].c
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i].path[0])

    def test_group_by(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern, block_size=self.block_sizes[0])

            result = []

            for file in files(group_by="r"):
                result.append(file)

            pprint.pprint(result)

            for i in range(len(result)):
                for j in range(len(result[i])):
                    assert fp_data.fp_groupby[i][1][j][0]["r"] == result[i][1][j][0]["r"]
                    assert fp_data.fp_groupby[i][1][j][0]["c"] == result[i][1][j][0]["c"]

    def test_group_by_pydantic(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern, block_size=self.block_sizes[0])

            result = []

            for file in files(group_by="r", pydantic_output=True):
                result.append(file)

            pprint.pprint(result)

            for i in range(len(result)):
                for j in range(len(result[i])):
                    assert fp_data.fp_groupby[i][1][j][0]["r"] == result[i][1][j].r
                    assert fp_data.fp_groupby[i][1][j][0]["c"] == result[i][1][j].c


"""
    def test_group_by_multi(self):

        for pattern in self.patterns:
            old_files = filepattern.FilePattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern)

            old_result = []
            result = []

            # group by "c" instead of "r" since we changed how group by works
            for file in old_files(group_by="rc"):
                old_result = file
            for file in files(group_by=["r", "c"]):
                result.append(file[1])

            pprint.pprint(old_result)
            print()
            pprint.pprint(result)

            assert len(old_result) == len(result)

            for i in range(len(old_result)):
                assert old_result[i]["r"] == result[i][0][0]["r"]
                assert old_result[i]["c"] == result[i][0][0]["c"]
                assert str(os.path.basename(old_result[i]['file'])) == os.path.basename(result[i][0][1][0])

    def test_get_item(self):
        for pattern in self.patterns:
            for block_size in self.block_sizes:

                old_files = filepattern.FilePattern(self.path, self.old_pattern)
                files = fp.FilePattern(self.path, pattern, block_size=block_size)

                old_result = []
                result = []

                for file in old_files():
                    old_result.append(file)

                result = files[0:-1]

                assert len(old_result) == len(result)

                for i in range(len(old_result)):
                    assert old_result[i][0]["r"] == result[i][0]["r"]
                    assert old_result[i][0]["c"] == result[i][0]["c"]
                    assert str(os.path.basename(old_result[i][0]['file'])) == os.path.basename(result[i][1][0])

                assert old_result[0][0]["r"] == files[0][0]["r"]
                assert old_result[0][0]["c"] == files[0][0]["c"]
                assert str(os.path.basename(old_result[0][0]['file'])) == os.path.basename(files[0][1][0])

                indices = [1, 15, 25, 37]
                result_slice = files[indices]

                for i in range(len(result_slice)):
                    assert old_result[indices[i]][0]["r"] == result_slice[i][0]["r"]
                    assert old_result[indices[i]][0]["c"] == result_slice[i][0]["c"]
                    assert str(os.path.basename(old_result[indices[i]][0]['file'])) == os.path.basename(result_slice[i][1][0])

                assert old_result[indices[-1]][0]["r"] == result_slice[-1][0]["r"]
                assert old_result[indices[-1]][0]["c"] == result_slice[-1][0]["c"]
                assert str(os.path.basename(old_result[indices[-1]][0]['file'])) == os.path.basename(result_slice[-1][1][0])

                result_slice = files[1::2]
                old_result_slice = old_result[1::2]

                assert len(old_result_slice) == len(result_slice)

                for i in range(0, len(old_result_slice)):
                    assert old_result_slice[i][0]["r"] == result_slice[i][0]["r"]
                    assert old_result_slice[i][0]["c"] == result_slice[i][0]["c"]
                    assert str(os.path.basename(old_result_slice[i][0]['file'])) == os.path.basename(result_slice[i][1][0])
"""


class TestExternalStringPattern():
    root_directory = os.path.dirname(os.path.realpath(__file__))

    path = root_directory + '/test_data/data100'
    string_path = root_directory + '/test_data/data100.txt'

    old_pattern = 'img_r{rrr}_c{ccc}.tif'

    patterns = ['img_r{r:ddd}_c{c:ddd}.tif', 'img_r{r:d+}_c{c:d+}.tif']

    block_sizes = ['300 KB']

    MAX_NUM = 1

    def test_file_pattern(self):

        for pattern in self.patterns:
            # old_files = filepattern.FilePattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern, block_size=self.block_sizes[0])

            # old_result = []
            result = []

            # for file in old_files():
            #     old_result.append(file)
            for file in files():
                result.append(file)

            pprint.pprint(result)

            # assert (len(fp_data.test_fp) == len(result))

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_get_matching(self):
        for pattern in self.patterns:
            nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

            files = fp.FilePattern(self.path, pattern, block_size=self.block_sizes[0])

            result = []
            for match in files.get_matching(r=[nums[0]]):
                result.append(match)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_group_by(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern, block_size=self.block_sizes[0])

            result = []

            for file in files(group_by="r"):
                result.append(file)

            pprint.pprint(result)

            for i in range(len(result)):
                for j in range(len(result[i])):
                    assert fp_data.fp_groupby[i][1][j][0]["r"] == result[i][1][j][0]["r"]
                    assert fp_data.fp_groupby[i][1][j][0]["c"] == result[i][1][j][0]["c"]
                    assert os.path.basename(fp_data.fp_groupby[i][1][j][1][0]) == os.path.basename(result[i][1][j][1][0])


"""
    def test_group_by_multi(self):

        for pattern in self.patterns:
            old_files = filepattern.FilePattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern)

            old_result = []
            result = []

            # group by "c" instead of "r" since we changed how group by works
            for file in old_files(group_by="rc"):
                old_result = file
            for file in files(group_by=["r", "c"]):
                result.append(file[1])

            pprint.pprint(old_result)
            print()
            pprint.pprint(result)

            assert len(old_result) == len(result)

            for i in range(len(old_result)):
                assert old_result[i]["r"] == result[i][0][0]["r"]
                assert old_result[i]["c"] == result[i][0][0]["c"]
                assert str(os.path.basename(old_result[i]['file'])) == os.path.basename(result[i][0][1][0])

    def test_group_by(self):
        for pattern in self.patterns:
            for block_size in self.block_sizes:
                old_files = filepattern.FilePattern(self.path, self.old_pattern)
                files = fp.FilePattern(self.string_path, pattern, block_size=block_size)

                old_result = []
                result = []

                # group by "c" instead of "r" since we changed how group by works
                for file in old_files(group_by="c"):

                    old_result.append(file)

                for file in files(group_by="r"):
                    result.append(file)


                assert len(old_result) == len(result)
                for i in range(len(old_result)):
                   assert len(old_result[i]) == len(result[i][1])

                for i in range(len(old_result)):
                    for j in range(len(old_result[i])):
                        assert old_result[i][j]["r"] == result[i][1][j][0]["r"]
                        assert old_result[i][j]["c"] == result[i][1][j][0]["c"]
                        assert str(os.path.basename(old_result[i][j]['file'])) == result[i][1][j][1][0]

                result = []
                old_result = []

                for file in old_files(group_by="r"):
                    old_result.append(file)


                for file in files(group_by="c"):
                    result.append(file)

                assert len(old_result) == len(result)

                for i in range(len(old_result)):
                    for j in range(len(old_result[i])):
                        assert old_result[i][j]["r"] == result[i][1][j][0]["r"]
                        assert old_result[i][j]["c"] == result[i][1][j][0]["c"]
                        assert str(os.path.basename(old_result[i][j]['file'])) == result[i][1][j][1][0]

    def test_get_item(self):
        for pattern in self.patterns:
            for block_size in self.block_sizes:

                old_files = filepattern.FilePattern(self.path, self.old_pattern)
                files = fp.FilePattern(self.string_path, pattern, block_size=block_size)

                old_result = []
                result = []

                for file in old_files():
                    old_result.append(file)
                for file in files():
                    result.append(file)

                result = files[0:-1]

                assert len(old_result) == len(result)

                for i in range(len(old_result)):
                    assert old_result[i][0]["r"] == result[i][0]["r"]
                    assert old_result[i][0]["c"] == result[i][0]["c"]
                    assert str(os.path.basename(old_result[i][0]['file'])) == result[i][1][0]

                assert old_result[0][0]["r"] == files[0][0]["r"]
                assert old_result[0][0]["c"] == files[0][0]["c"]
                assert str(os.path.basename(old_result[0][0]['file'])) == files[0][1][0]

                indices = [1, 15, 25, 37]
                result_slice = files[indices]

                for i in range(len(result_slice)):
                    assert old_result[indices[i]][0]["r"] == result_slice[i][0]["r"]
                    assert old_result[indices[i]][0]["c"] == result_slice[i][0]["c"]
                    assert str(os.path.basename(old_result[indices[i]][0]['file'])) == result_slice[i][1][0]

                assert old_result[indices[-1]][0]["r"] == result_slice[-1][0]["r"]
                assert old_result[indices[-1]][0]["c"] == result_slice[-1][0]["c"]
                assert str(os.path.basename(old_result[indices[-1]][0]['file'])) == result_slice[-1][1][0]

                result_slice = files[1::2]
                old_result_slice = old_result[1::2]

                assert len(old_result_slice) == len(result_slice)

                for i in range(0, len(old_result_slice)):
                    assert old_result_slice[i][0]["r"] == result_slice[i][0]["r"]
                    assert old_result_slice[i][0]["c"] == result_slice[i][0]["c"]
                    assert str(os.path.basename(old_result_slice[i][0]['file'])) == result_slice[i][1][0]

"""

"""
class TestExternalVectorPattern():

    root_directory = os.path.dirname(os.path.realpath(__file__))

    path = root_directory + '/test_vectorpattern_data.txt'

    old_pattern = 'x{xx}_y{yy}_wx{z}_wy{r}_c{c}.ome.tif'

    patterns = ['x{x:dd}_y{y:dd}_wx{z:d}_wy{r:d}_c{c:d}.ome.tif', old_pattern]

    MAX_NUM = 9

    block_sizes = ['300 KB']

    def test_file_pattern(self):

        for pattern in self.patterns:
            #old_files = filepattern.FilePattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern, block_size=self.block_sizes[0])

            #old_result = []
            result = []

            #for file in old_files():
            #    old_result.append(file)
            for file in files():
                result.append(file)

            pprint.pprint(result)

            assert (len(fp_data.test_fp) == len(result))

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])


    def test_get_matching(self):
         for pattern in self.patterns:
            nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

            files = fp.FilePattern(self.path, pattern, block_size=self.block_sizes[0])

            result = files.get_matching(r=[nums[0]])

            pprint.pprint(result)

            for i in range(len(result)):
                assert fp_data.test_fp[i][0]["r"] == result[i][0]["r"]
                assert fp_data.test_fp[i][0]["c"] == result[i][0]["c"]
                assert os.path.basename(fp_data.test_fp[i][1][0]) == os.path.basename(result[i][1][0])

    def test_group_by(self):
        for pattern in self.patterns:

            files = fp.FilePattern(self.path, pattern, block_size=self.block_sizes[0])

            result = []

            for file in files(group_by="r"):
                result.append(file)

            pprint.pprint(result)

            for i in range(len(result)):
                for j in range(len(result[i])):
                    assert fp_data.fp_groupby[i][1][j][0]["r"] == result[i][1][j][0]["r"]
                    assert fp_data.fp_groupby[i][1][j][0]["c"] == result[i][1][j][0]["c"]
                    assert os.path.basename(fp_data.fp_groupby[i][1][j][1][0]) == os.path.basename(result[i][1][j][1][0])

    def test_group_by_multi(self):

        for pattern in self.patterns:
            old_files = filepattern.VectorPattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern, "50 MB")

            old_result = []
            result = []

            # group by "c" instead of "r" since we changed how group by works
            for file in old_files(group_by="xyzrc"):
                old_result = file
            for file in files(group_by=["x", "y", "z", "r", "c"]):
                result.append(file[1])

            assert len(old_result) == len(result)

            old_result = sorted(old_result, key=lambda k: k['file']) # Old version does not sort results

            for i in range(len(old_result)):
                for key in result[i][0][0]:
                    assert str(old_result[i][key]) == str(result[i][0][0][key])

                assert str(os.path.basename(old_result[i]['file'])) == os.path.basename(result[i][0][1][0])

    def test_get_item(self):
        for pattern in self.patterns:

            old_files = filepattern.VectorPattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern)

            old_result = []
            result = []

            for file in old_files():
                old_result.append(file)
            old_result = sorted(old_result, key=lambda k: k[0]['file'])

            result = files[0:-1]

            assert len(old_result) == len(result)


            for i in range(len(old_result)):
                for key in result[i][0]:
                    assert int(old_result[i][0][key]) == result[i][0][key]
                assert str(os.path.basename(old_result[i][0]['file'])) == os.path.basename(result[i][1][0])

            for key in files[-1][0]:
                assert str(old_result[-1][0][key]) == str(files[-1][0][key])
            assert str(os.path.basename(old_result[-1][0]['file'])) == files[-1][1][0]

            indices = [1, 15, 25, 37]
            result_slice = files[indices]

            for i in range(len(result_slice)):
                for key in result[i][0]:
                    assert str(old_result[i][0][key]) == str(result[i][0][key]) # Old version stores value as string
                assert os.path.basename(old_result[i][0]['file']) == os.path.basename(result[i][1][0])

            result_slice = files[1::2]
            old_result_slice = old_result[1::2]

            assert len(old_result_slice) == len(result_slice)

            for i in range(len(old_result)):
                for key in result[i][0]:
                    assert int(old_result[i][0][key]) == result[i][0][key]
                assert str(os.path.basename(old_result[i][0]['file'])) == os.path.basename(result[i][1][0])

"""
