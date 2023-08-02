import pytest
import filepattern as fp
import os
from operator import itemgetter
import pprint
import test_generate_filepattern_data
import test_filepattern_data as fp_data

class TestFilePattern():

    test_generate_filepattern_data.generate_data()
    test_generate_filepattern_data.generate_channel_data()

    root_directory = os.path.dirname(os.path.realpath(__file__))

    path = root_directory + '/test_data/data100'

    old_pattern = 'img_r{rrr}_c{ccc}.tif'

    patterns = ['img_r00{r:d}_c{c:ddd}.tif', 'img_r{r:d+}_c{c:d+}.tif', old_pattern]

    MAX_NUM = 9

    def test_file_pattern(self):

        for pattern in self.patterns:
            #old_files = filepattern.FilePattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern)

            #old_result = []
            result = []

            #for file in old_files():
            #    old_result.append(file)
            for file in files():
                result.append(file)
                
            pprint.pprint(result)
            
            assert (len(fp_data.test_fp) == len(result))
            
            assert (len(result) == len(files)) # test length operator
            
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

    root_directory = os.path.dirname(os.path.realpath(__file__))

    path = root_directory + '/test_data/data100'

    filepath = root_directory + '/test_data/data100.txt'

    old_pattern = 'img_r{rrr}_c{ccc}.tif'

    patterns = ['img_r{r:ddd}_c{c:ddd}.tif', 'img_r{r:d+}_c{c:d+}.tif', old_pattern]

    MAX_NUM = 9

    def test_file_pattern(self):
    
        for pattern in self.patterns:
            #old_files = filepattern.FilePattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern)

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
            #old_files = filepattern.FilePattern(self.path, self.old_pattern)
            files = fp.FilePattern(self.path, pattern, block_size=self.block_sizes[0])

            #old_result = []
            result = []

            #for file in old_files():
            #    old_result.append(file)
            for file in files():
                result.append(file)
                
            pprint.pprint(result)
            
            
            #assert (len(fp_data.test_fp) == len(result))
            
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