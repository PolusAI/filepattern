# Filepattern Utility

[![Documentation Status](https://readthedocs.org/projects/filepattern/badge/?version=latest)](https://filepattern.readthedocs.io/en/latest/?badge=latest)
[![PyPI](https://img.shields.io/pypi/v/filepattern)](https://pypi.org/project/filepattern/)
![PyPI - Downloads](https://img.shields.io/pypi/dm/filepattern)
![Bower](https://img.shields.io/bower/l/MI)

The `filepattern` Python utility is designed to information stored in file
names. A `filepattern` is essentially a simplified regular expression with named
groups, and regular expressions are valid `filepattern` expressions provided
they do not use groups.

The utility was born from the need to manipulate and organize image data from a
variety of microscopes, all of which have a systematic but different file naming
conventions. This made abstracting things like image stitching algorithms easier
to apply to files with disparate naming conventions by simply changing the
`filepattern` rather than generating new code to parse each new naming
convention. Although `filepattern` was born to wield against image data, it is
not limited to image data, and can handle filenames with any extension.

## Summary

  - [Read the docs!](https://filepattern.readthedocs.io/en/latest/)
  - [Install](#install)
  - [Versioning](#versioning)
  - [Authors](#authors)
  - [License](#license)
  - [Acknowledgments](#acknowledgments)

## Install

This utility is built in pure Python with no dependencies.

`pip install filepattern`

## Getting Started

What does a ``filepattern`` look like? It is probably easiest to show by
example. Say there is a folder with the following files:

```bash
my_data_folder/x000_y000_z001.tif
my_data_folder/x000_y000_z002.tif
my_data_folder/x000_y000_z003.tif
```

The `filepattern` for the above files would be `x000_y000_z00{z}.ome.tif`.
The curly brackets indicate a file name variable, and `{z}` indicates that the
number will be parsed and stored as a z value. If a similar regular expression
were to be written, then it would look like `x000_y000_z00([0-9]).ome.tif`,
which is not only longer but would require more code to parse the regular
expression.

To easily loop over the values, a `FilePattern` object can be created and used
to iterate over the files in order.

```python
import filepattern, pathlib

pattern = 'x000_y000_z00{z}.ome.tif'
path_to_files = pathlib.Path('/path/to/files')

fp = filepattern.FilePattern(path_to_files,pattern)

# Loop over all files that match the pattern
for files in fp():

    # Files contains a list of all files with identical z-value
    # In this case, there should only be one so select the first item
    file = files[0]

    # Each value in files is a dictionary containing the filename under the
    # "file" key, and the z-value extracted from the file name under the "z" key
    print(f"File {file['file']} has z-value {file['z']}")
```

The output should be as follows:

```bash
File my_data_folder/x000_y000_z001.tif has z-value 0
File my_data_folder/x000_y000_z002.tif has z-value 1
File my_data_folder/x000_y000_z003.tif has z-value 2
```

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available,
see the
[tags on this repository](https://github.com/PurpleBooth/a-good-readme-template/tags).

## Authors

Nick Schaub (nick.schaub@nih.gov, nick.schaub@labshare.org)

## License

This project is licensed under the [MIT License](LICENSE)
Creative Commons License - see the [LICENSE](LICENSE) file for
details

## Acknowledgments

  - This utility was inspired by the notation found in the 
  [MIST](https://github.com/usnistgov/MIST)
  algorithm developed at NIST.
