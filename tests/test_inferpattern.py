import json, pytest
from pathlib import Path
import filepattern as fp
import os

json_path = Path(__file__).parent.parent.joinpath("plugin.json")
with open(Path(__file__).with_name('test_infer_pattern_data.json'),'r') as fr:        
    data = json.load(fr)

class TestInference():
            
    def test_numeric_fixed_width(self):
        
        pattern = fp.infer_pattern(files=data['robot'])

        assert pattern =='00{r:d}0{t:dd}-{c:d}-00100100{z:d}.tif'

    def test_alphanumeric_fixed_width(self):
        
        pattern = fp.infer_pattern(files=data['brain'])

        assert pattern == 'S1_R{r:d}_C1-C11_A1_y0{t:dd}_x0{c:dd}_c0{z:dd}.ome.tif'
        
    def test_alphanumeric_variable_width(self):
        
        pattern = fp.infer_pattern(files=data['variable'])

        assert pattern == 'S1_R{r:d}_C1-C11_A1_y{t:d+}_x{c:d+}_c{z:d+}.ome.tif'

    def test_alphanumeric_channel_variable_width(self):
        
        pattern = fp.infer_pattern(files=data['channel'])

        assert pattern =='img_r00{r:d}_c00{t:d}_{c:c+}.tif'
    
    def test_variable_naming(self):
        pattern = fp.infer_pattern(files=data['channel'], variables='xyz')
        
        assert pattern == 'img_r00{x:d}_c00{y:d}_{z:c+}.tif'

    def test_alphanumeric_both_variable_width(self):

        pattern = fp.infer_pattern(files=data['both'])

        assert pattern == '{r:cccccc}_{t:d+}_{c:c+}.ome.tif'
    
    def test_invalid_input(self):
        with pytest.raises(RuntimeError):
            pattern = fp.infer_pattern(files=data['invalid'])
            
    def test_vp(self):
        root_directory = os.path.dirname(os.path.realpath(__file__))
        path = root_directory + '/test_vectorpattern_data.txt'
        pattern = fp.infer_pattern(path=path)
        
        assert pattern == 'x{r:dd}_y01_wx0_wy0_c1.ome.tif'

class TestExternalInference():
    
    root_directory = os.path.dirname(os.path.realpath(__file__))

    path = root_directory + '/test_data/data'
    
    def test_fp_single_block(self):
        pattern = fp.infer_pattern(path=self.path, block_size='1 GB')
        
        assert pattern == 'img_r00{r:d}_c00{t:d}_{c:c+}.tif'
        
    def test_fp_multiple_block(self):
        pattern = fp.infer_pattern(path=self.path, block_size='1 MB')
        
        assert pattern == 'img_r00{r:d}_c00{t:d}_{c:c+}.tif'

    def test_sp_single_block(self):
        path = self.root_directory + '/test_data/sp_data.txt'
        pattern = fp.infer_pattern(path=path, block_size='1 GB')
        
        assert pattern == 'img_r00{r:d}_c00{t:d}_{c:c+}.tif'

    def test_sp_multi_block(self):
        path = self.root_directory + '/test_data/sp_data.txt'
        pattern = fp.infer_pattern(path=path, block_size='900 B')
        
        assert pattern == 'img_r00{r:d}_c00{t:d}_{c:c+}.tif'
    
    def test_vp_single_block(self):
        path = self.root_directory + '/test_vectorpattern_data.txt'
        pattern = fp.infer_pattern(path=path, block_size='1 GB')
        
        assert pattern == 'x{r:dd}_y01_wx0_wy0_c1.ome.tif'
                
    def test_vp_multi_block(self):
        path = self.root_directory + '/test_vectorpattern_data.txt'
        pattern = fp.infer_pattern(path=path, block_size='1 KB')
        
        assert pattern == 'x{r:dd}_y01_wx0_wy0_c1.ome.tif'
