# Filepattern

[![Documentation Status](https://readthedocs.org/projects/filepattern/badge/?version=latest)](https://filepattern.readthedocs.io/en/latest/?badge=latest)
[![PyPI](https://img.shields.io/pypi/v/filepattern)](https://pypi.org/project/filepattern/)
![PyPI - Downloads](https://img.shields.io/pypi/dm/filepattern)
![Bower](https://img.shields.io/bower/l/MI)

The ``filepattern`` utility is used to store files that follow a pattern, where the pattern is analogous to a simplified regular expression. The need for 
``filepattern`` arises in situations where large amounts of data with a systematic naming convention needs to be filtered by patterns in the naming. For example, one may have
a directory containing segmented images where the name contains information such as the channel, the column value, and the row value. ``filepattern`` provides the ability to 
extract all images containing such a naming pattern, filter by the row or column value, or group files by one or more of the aforementioned variables. 

## Summary
- [Read the Docs](https://filepattern2.readthedocs.io/en/latest/Home.html)
- [Install](#install)
- [Authors](#authors)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## Install

`filepattern` is pip installable from https://pypi.org/project/filepattern/.

or by running

pip install -i https://pypi.org/simple/ filepattern


## Build and Install
Alternatively, ``filepattern`` can either be build inside a `conda` environment or independently outside of it directly from the source. 

### __Inside Conda__
``filepattern`` uses a CMake build system. 
Below is an example of how to build ``filepattern`` Python package inside a `conda` environment on Linux.

```bash
git clone https://github.com/PolusAI/filepattern.git
cd filepattern
conda install -y -c conda-forge compilers --file ci-utils/envs/conda_cpp.txt --file ci-utils/envs/conda_py.txt
CMAKE_ARGS="-DCMAKE_PREFIX_PATH=$CONDA_PREFIX -DCMAKE_INSTALL_PREFIX=$CONDA_PREFIX " python -m pip install . -vv
```

### __Without Using Conda__
To build ``filepattern`` outside of a `conda` environment, use the following example.
```bash
git clone https://github.com/PolusAI/filepattern.git
cd filepattern
mkdir build_dep
cd build_dep
bash ../ci-utils/install_prereq_linux.sh
cd ..
export FILEPATTERN_DEP_DIR=./build_dep
python -m pip install . -vv
```

### __C++ Library__
``filepattern`` also comes with a C++ API. To install ``filepattern`` as a C++ library, use the following command in conjuction with installing necessary dependency using the methods mentioned above.
```bash
git clone https://github.com/PolusAI/filepattern.git
cd filepattern
mkdir build_dep
cd build
cmake -Dfilepattern_SHARED_LIB=ON ..
make -j4
make install
```
To link ``filepattern`` with the client code, use the following CMake statements.
```
find_package(filepattern REQUIRED)
target_link_libraries(client_executable PRIVATE filepattern::filepattern)
```

<h2 id="filepattern-section"> Filepattern </h2> 

When only a path to a directory and a pattern are supplied to the constructor of ``filepattern``, ``filepattern`` will iterate over the directory, matching the filenames in the directory to the ``filepattern``. The  ``filepattern`` can either be supplied by  the user or can be found using the ``infer_pattern`` method of ``filepattern``. For example, consider a directory containing the following files, 

```
img_r001_c001_DAPI.tif
img_r001_c001_TXREAD.tif
img_r001_c001_GFP.tif
```

In each of these filenames, there are three descriptors of the image: the row, the column, and the channel. To match these files, the pattern ``img_r{r:ddd}_c{c:ddd}_{channel:c+}`` can be used. In this pattern, the named groups are contained within the curly brackets, where the variable name is before the colon and the value is after the colon. For the value, the descriptors ``d`` and ``c`` are used, which represent a digit and a character, respectively. In the example pattern, three `d`'s are used to capture three digits. The ``+`` after ``c`` denotes that one or more characters will be captured, which is equivalent to ``[a-zA-z]+`` in a regular expression. The ``+`` symbol may be used after either ``d`` or ``c``. 

To have ``filepattern`` guess what the pattern is for a directory, the static method ``infer_pattern`` can be used:

```python
import filepattern as fp 

path = 'path/to/directory'

pattern = fp.infer_pattern(path)

print(pattern)

```
The result is:

```
img_r001_c001_{r:c+}.tif
``` 

Note that the ``infer_pattern`` can also guess the patterns from stitching vectors and text files when a path to a text file is passed, rather than a path to a directory. 

To retrieve files from a directory that match the ``filepattern``, an iterator is called on the `FilePattern` object, as shown below. A user specified custom pattern, such as the one below, or the guessed pattern can be passed to the constructor.

```python
import filepattern as fp
import pprint

filepath = "path/to/directory"

pattern = "img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif"

files = fp.FilePattern(filepath, pattern)

for file in files(): 
    pprint.pprint(file)
```
The output is:
```
({'c': 1, 'channel': 'DAPI', 'r': 1},
 ['path/to/directory/img_r001_c001_DAPI.tif'])
({'c': 1, 'channel': 'TXREAD', 'r': 1},
 ['path/to/directory/img_r001_c001_TXREAD.tif'])
({'c': 1, 'channel': 'GFP', 'r': 1},
 ['path/to/directory/img_r001_c001_GFP.tif'])
```

As shown in this example, the output is a tuple where the first member is a map between the group name supplied in the pattern and the value of the group for each file name. The second member of the tuple is a vector containing the path to the matched file. The second member is stored in a vector for the case where a directory is supplied with multiple subdirectories. In this case, a third optional parameter can be passed to the constructor. If the parameter ``recursive`` is set to `True`, a recursive directory iterator will be used, which iterates over all subdirectories. If the basename of two files from two different subdirectories match, ``filepattern`` will add the path of the file to the vector in the existing tuple rather than creating a new tuple.

 For example, consider the directory with the structure 

```
/root_directory
    /DAPI
        img_r001_c001.tif
    /GFP
        img_r001_c001.tif
    /TXREAD
        img_r001_c001.tif
```

In this case, the subdirectories are split by the channel. Recursive matching can be used as shown below.

```python
import filepattern as fp
import pprint

filepath = "path/to/root/directory"

pattern = "img_r{r:ddd}_c{c:ddd}.tif"

files = fp.FilePattern(filepath, pattern, recursive=True)

for file in files(): 
    pprint.pprint(file)
```

The output of this case is:
```
({'c': 1, 'r': 1},
 ['path/to/root/directory/DAPI/img_r001_c001.tif',
  'path/to/root/directory/GFP/img_r001_c001.tif',
  'path/to/root/directory/TXREAD/img_r001_c001.tif'])
```

<h3 id="floating-point"> Floating Point Support </h3>
`filepattern` has the ability to capture floating point values in file patterns. For example, if we have a set of files

```
img_r0.05_c1.15.tif
img_r1.05_c2.25.tif
img_r2.05_c3.35.tif
```

We can capture the values in a couple of different ways. Similar to capturing digits, the character `f` can be used to capture an element of a floating point number.
Note that with this method, the decimal point in the number must be captured by an `f`. For example, in the file `img_r0.05_c1.15.tif`, the floating point numbers would be capture with `ffff`.
The code to utilize this method is

```python
filepath = "path/to/directory"

pattern = "img_r{r:ffff}_c{c:ffff}.tif"

files = fp.FilePattern(filepath, pattern)

for file in files(): 
    pprint.pprint(file)
```

The result is:
```
({'c': 1.15, 'r': 0.05},
 ['path/to/directory/img_r0.05_c1.15.tif'])
({'c': 2.25, 'r': 1.05},
 ['path/to/directory/img_r1.05_c2.25.tif'])
({'c': 3.35, 'r': 2.05},
 ['path/to/directory/img_r2.05_c3.35.tif'])
```

To capture floating point numbers with an arbitrary number of digits, we can use `f+`. This method operates in the same way as using `d+` or `c+`, where all digits (and the decimal point) will be
captured for a floating point of any length. The code for this method is

```python
filepath = "path/to/directory"

pattern = "img_r{r:f+}_c{c:f+}.tif"

files = fp.FilePattern(filepath, pattern)

for file in files(): 
    pprint.pprint(file)
```

The result of this code is the same as the previous example.

The final method for capturing floating points is to use `d` to capture the digits and to add the decimal point where needed. For example, in the file `img_r0.05_c1.15.tif`, the floating point numbers could be captured using `d.dd`. The code for this method is:

```python
filepath = "path/to/directory"

pattern = "img_r{r:d.dd}_c{c:d.dd}.tif"

files = fp.FilePattern(filepath, pattern)

for file in files(): 
    pprint.pprint(file)
```

Once again, the results are the same as the first example.

Note that `d` can be used to specify even more specific floating points. For example, if we want to capturing all floating points with one digit in the whole part and an arbitrary number of digits in the decimal, we can add `d.d+` for the pattern. Similarly, this could be used in a reverse manner to capture an arbitrary number of digits in the whole part using `d+.ddd`.

<h3 id="group-by"> Group By </h3>

If images need to be processed in a specific order, for example by the row number, the ``group_by`` function is used. With the directory 

```
img_r001_c001_DAPI.tif
img_r002_c001_DAPI.tif
img_r001_c001_TXREAD.tif
img_r002_c001_TXREAD.tif
img_r001_c001_GFP.tif
img_r002_c001_GFP.tif
```

the images can be returned in groups where ``r`` is held constant by passing the parameter ``group_by='r'`` to the object iterator.

```python
import filepattern as fp
import pprint

filepath = "path/to/directory"

pattern = "img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif"

files = fp.FilePattern(filepath, pattern)

for file in files(group_by='r'): 
    pprint.pprint(file)
```

The output is:
```
('r': 1, [({'c': 1, 'channel': 'DAPI', 'file': 0, 'r': 1},
  ['/home/ec2-user/Dev/FilePattern/data/example/img_r001_c001_DAPI.tif']),
 ({'c': 1, 'channel': 'TXREAD', 'file': 0, 'r': 1},
  ['/home/ec2-user/Dev/FilePattern/data/example/img_r001_c001_TXREAD.tif']),
 ({'c': 1, 'channel': 'GFP', 'file': 0, 'r': 1},
  ['/home/ec2-user/Dev/FilePattern/data/example/img_r001_c001_GFP.tif'])])
('r': 2, [({'c': 1, 'channel': 'DAPI', 'file': 0, 'r': 2},
  ['/home/ec2-user/Dev/FilePattern/data/example/img_r002_c001_DAPI.tif']),
 ({'c': 1, 'channel': 'GFP', 'file': 0, 'r': 2},
  ['/home/ec2-user/Dev/FilePattern/data/example/img_r002_c001_GFP.tif']),
 ({'c': 1, 'channel': 'TXREAD', 'file': 0, 'r': 2},
  ['/home/ec2-user/Dev/FilePattern/data/example/img_r002_c001_TXREAD.tif'])])
```
Note that the return of each call is a tuple where the first member is the ``group_by`` variable mapped to the current value and the second member is a list of files where the ``group_by`` variable matches the current value.

<h3 id="get-matching"> Get Matching </h3>

To get files where the variable matches a value, the ``get_matching`` method is used. For example, if only files from the TXREAD channel are needed, ``get_matching(channel=['TXREAD']`` is called. 

```python
filepath = "/home/ec2-user/Dev/FilePattern/data/example"

pattern = "img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif"

files = fp.FilePattern(filepath, pattern)

matching = files.get_matching(channel=['TXREAD'])

pprint.pprint(matching)
```

The output is:
```
[({'c': 1, 'channel': 'TXREAD', 'r': 1},
  ['/home/ec2-user/Dev/FilePattern/data/example/img_r001_c001_TXREAD.tif']),
 ({'c': 1, 'channel': 'TXREAD', 'r': 2},
  ['/home/ec2-user/Dev/FilePattern/data/example/img_r002_c001_TXREAD.tif'])]
```

## Text files
``filepattern`` can also take in a text file as an input rather than a directory. To use this functionality, a path to a text file is supplied to the ``path`` variable rather than a directory. When a text file is passed as input, each line of the text file will be matched to the pattern. For example, a text file containing containing the strings
```
img_r001_c001_DAPI.tif
img_r001_c001_TXREAD.tif
img_r001_c001_GFP.tif
```

can be matched to the pattern ```img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif``` with:

```python
import filepattern as fp
import pprint

filepath = "path/to/file.txt"

pattern = "img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif"

files = fp.FilePattern(filepath, pattern)

for file in files(): 
    pprint.pprint(file)

```

The output is:

```
({'c': 1, 'channel': 'DAPI', 'r': 1}, 
 ['img_r001_c001_DAPI.tif'])
({'c': 1, 'channel': 'TXREAD', 'r': 1}, 
 ['img_r001_c001_TXREAD.tif'])
({'c': 1, 'channel': 'GFP', 'r': 1}, 
 ['img_r001_c001_GFP.tif'])
```

 After calling ``filepattern`` on a text file,  the [group_by](#group-by) and [get_matching](#get-matching) functionality can be used the same as outlined in the [FilePattern](#filepattern-section) section. 

## Stitching vectors

``filepattern`` can also take in stitching vectors as input. In this case, a path to a text file containing a stitching vector is passed to the ``path`` variable. A stitching vector has the following form,

```
file: x01_y01_wx0_wy0_c1.ome.tif; corr: 0; position: (0, 0); grid: (0, 0);
file: x02_y01_wx0_wy0_c1.ome.tif; corr: 0; position: (3496, 0); grid: (3, 0);
file: x03_y01_wx0_wy0_c1.ome.tif; corr: 0; position: (6992, 0); grid: (6, 0);
file: x04_y01_wx0_wy0_c1.ome.tif; corr: 0; position: (10488, 0); grid: (9, 0);
```

This stitching vector can be processed using 

```python
import filepattern as fp

filepath = 'path/to/stitching/vector.txt'

pattern = 'x0{x:d}_y01_wx0_wy0_c1.ome.tif'

files = fp.FilePattern(filepath, pattern)

for file in files():
    pprint.pprint(files)
```

The output is:
```
({'correlation': 0, 'gridX': 0, 'gridY': 0, 'posX': 0, 'posY': 0, 'x': 1},
 ['x01_y01_wx0_wy0_c1.ome.tif'])
({'correlation': 0, 'gridX': 3, 'gridY': 0, 'posX': 3496, 'posY': 0, 'x': 2},
 ['x02_y01_wx0_wy0_c1.ome.tif'])
({'correlation': 0, 'gridX': 6, 'gridY': 0, 'posX': 6992, 'posY': 0, 'x': 3},
 ['x03_y01_wx0_wy0_c1.ome.tif'])
({'correlation': 0, 'gridX': 9, 'gridY': 0, 'posX': 10488, 'posY': 0, 'x': 4},
 ['x04_y01_wx0_wy0_c1.ome.tif'])
```
As shown in the output, ``filepattern`` not only captures the specified variables from the pattern, but also captures the variables supplied in the stitching vector. 

## Out of core 

``filepattern`` has the ability to use external memory when the dataset is too large to fit in main memory, i.e. it utilizes disk memory along with RAM. It has the same functionality as ``filepattern``, however it takes in an addition parameter called `block_size`, which limits the amount of main memory used by ``filepattern``. Consider a directory containing the files:

```
img_r001_c001_DAPI.tif
img_r001_c001_TXREAD.tif
img_r001_c001_GFP.tif
```

This directory can be processed with only one file in memory as:

```python
import filepattern as fp
import pprint

filepath = "path/to/directory"

pattern = "img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif"

files = fp.FilePattern(filepath, pattern, block_size="125 B")


for file in files():
    pprint.pprint(file)
    

```
The output from this example is:

```
({'c': 1, 'channel': 'DAPI', 'r': 1},
 ['/home/ec2-user/Dev/FilePattern/data/example/img_r001_c001_DAPI.tif'])
({'c': 1, 'channel': 'TXREAD', 'r': 1},
 ['/home/ec2-user/Dev/FilePattern/data/example/img_r001_c001_TXREAD.tif'])
({'c': 1, 'channel': 'GFP', 'r': 1},
 ['/home/ec2-user/Dev/FilePattern/data/example/img_r001_c001_GFP.tif'])
```
Note that the ``block_size`` argument is provided in bytes (B) in this example, but also has the options for kilobytes (KB), megabytes (MB), and gigabytes (GB). The ``block_size`` must be under 1000 GB.

<h3 id="group-by-external"> Group by and get matching</h3>

The out of core version of ``filepattern`` contains the same functionalities as the in memory version. ``group_by`` is called the same way, i.e.,

```python
for file in files(group_by="r"):
    pprint.pprint(file)
```

The output remains identical to the in memory version.

The ``get_matching`` functionality remains the same, however the API is slightly different. In this case, ``get_matching`` is called as

```python

for matching in files.get_matching(channel=['TXREAD'])
    pprint.pprint(matching)
```
where the output is returned in blocks of `block_size`. The output is:

```
({'c': 1, 'channel': 'TXREAD', 'r': 1},
 ['/home/ec2-user/Dev/FilePattern/data/example/img_r001_c001_TXREAD.tif'])
```

## Out of Core: text files and stitching vectors

Out of core processing can also be used for stitching vectors and text files. To utilize this functionality, call ``filepattern`` the same way as described previously, but add in the ``block_size`` parameter, as described in the (Out of Core)[#out-of-core] section.

## Authors

Jesse McKinzie(Jesse.McKinzie@axleinfo.com, jesse.mckinzie@nih.gov)
Nick Schaub (nick.schaub@nih.gov, nick.schaub@labshare.org)

## License

This project is licensed under the [MIT License](LICENSE)
Creative Commons License - see the [LICENSE](LICENSE) file for
details

## Acknowledgments

- This utility was inspired by the notation found in the 
[MIST](https://github.com/usnistgov/MIST)
algorithm developed at NIST.
