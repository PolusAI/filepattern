import unittest, json
from pathlib import Path
from filepattern import infer_pattern

class InferenceTest(unittest.TestCase):
    """ Verify VERSION is correct """

    json_path = Path(__file__).parent.parent.joinpath("plugin.json")
    
    def setUp(self):
        
        with open(Path(__file__).with_name('inference_test.json'),'r') as fr:
            
            self.data = json.load(fr)
            
    def test_numeric_fixed_width(self):
        
        pattern = self.data['robot']
        
        self.assertEqual(pattern,'00{r}0{tt}-{c}-00100100{z}.tif')

    def test_alphanumeric_fixed_width(self):
        
        pattern = self.data['brain']
        
        self.assertEqual(pattern,'S1_R{r}_C1-C11_A1_y0{tt}_x0{cc}_c0{zz}.ome.tif')
        
    def test_alphanumeric_variable_width(self):
        
        pattern = self.data['variable']
        
        self.assertEqual(pattern,'S1_R{r}_C1-C11_A1_y{t+}_x{c+}_c{z+}.ome.tif')

if __name__=="__main__":

    unittest.main()