import filepattern as fp
import pprint
inp_dir="/home/samee/Downloads/segout/segout/abdomen_dcm_FDG_TK_KM_20221208000000_0"
int_pattern = ".*.nii.gz"
files = fp.FilePattern(inp_dir, int_pattern, recursive=True)
for f in files():
    f_fpath = f[1][0]  # Extract the file path from the tuple
    pprint.pprint(f_fpath)