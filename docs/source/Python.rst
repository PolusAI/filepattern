========
Python API
========

FilePattern iterates over a directory, text file, or stitching vector, matching filenames to a supplied ``filepattern``.

When only a path to a directory and a pattern are supplied to the constructor of ``filepattern``, ``filepattern``
will iterate over the directory, matching the filenames in the directory to the ``filepattern``. The  ``filepattern``
can either be supplied by  the user or can be found using the ``infer_pattern`` method of ``filepattern``.
For example, consider a directory containing the following files,


.. code-block:: bash

    img_r001_c001_DAPI.tif
    img_r001_c001_TXREAD.tif
    img_r001_c001_GFP.tif


n each of these filenames, there are three descriptors of the image: the row, the column, and the channel. To match
these files, the pattern ``img_r{r:ddd}_c{c:ddd}_{channel:c+}`` can be used. In this pattern, the named groups are
contained within the curly brackets, where the variable name is before the colon and the value is after the colon.
For the value, the descriptors ``d`` and ``c`` are used, which represent a digit and a character, respectively.
In the example pattern, three ``d``'s are used to capture three digits. The ``+`` after ``c`` denotes that one or
more characters will be captured, which is equivalent to ``[a-zA-z]+`` in a regular expression. The ``+`` symbol
may be used after either ``d`` or ``c``.

Note that special regex characters in the filepattern that are intended to be treated as literal characters must be escaped. For example,
the filepattern

To have ``filepattern`` guess what the pattern is for a directory, the static method ``infer_pattern`` can be used:

.. code-block:: python

    import filepattern.functions as fp

    path = 'path/to/directory'

    pattern = fp.infer_pattern(filepath)

    print(pattern)


The result is:

.. code-block:: bash

    img_r00{r:d}_c00{c:d}_{t:c+}.tif


Note that the ``infer_pattern`` can also guess the patterns from stitching vectors and text files when a path to a text
file is passed, rather than a path to a directory.

To retrieve files from a directory that match the ``filepattern``, an iterator is called on the ``FilePattern`` object,
as shown below. A user specified custom pattern, such as the one below, or the guessed pattern can be used as input in the constructor.

.. code-block:: python

    import filepattern as fp
    import pprint

    filepath = "path/to/directory"

    pattern = "img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif"

    files = fp.FilePattern(filepath, pattern)

    for file in files():
        pprint.pprint(file)

The output is:

.. code-block:: bash

    ({'c': 1, 'channel': 'DAPI', 'r': 1},
    ['path/to/directory/img_r001_c001_DAPI.tif'])
    ({'c': 1, 'channel': 'TXREAD', 'r': 1},
    ['path/to/directory/img_r001_c001_TXREAD.tif'])
    ({'c': 1, 'channel': 'GFP', 'r': 1},
    ['path/to/directory/img_r001_c001_GFP.tif'])


As shown in this example, the output is a tuple where the first member is a map between the group name supplied in the
pattern and the value of the group for each file name. The second member of the tuple is a vector containing the path to
the matched file. The second member is stored in a vector for the case where a directory is supplied with multiple
subdirectories. In this case, a third optional parameter can be passed to the constructor. If the parameter ``recursive``
is set to ``True``, a recursive directory iterator will be used, which iterates over all subdirectories. If the basename of
two files from two different subdirectories match, ``filepattern`` will add the path of the file to the vector in the
existing tuple rather than creating a new tuple.

 For example, consider the directory with the structure

.. code-block:: bash

    /root_directory
        /DAPI
            img_r001_c001.tif
        /GFP
            img_r001_c001.tif
        /TXREAD
            img_r001_c001.tif


In this case, the subdirectories are split by the channel. Recursive matching can be used as shown below.

.. code-block:: python

    import filepattern as fp
    import pprint

    filepath = "path/to/root/directory"

    pattern = "img_r{r:ddd}_c{c:ddd}.tif"

    files = fp.FilePattern(filepath, pattern, recursive=True)

    for file in files():
        pprint.pprint(file)


The output of this case is:

.. code-block:: bash

    ({'c': 1, 'r': 1},
    ['path/to/root/directory/DAPI/img_r001_c001.tif',
    'path/to/root/directory/GFP/img_r001_c001.tif',
    'path/to/root/directory/TXREAD/img_r001_c001.tif'])

~~~~~~~~
Group By
~~~~~~~~

If images need to be processed in a specific order, for example by the row
number, the ``group_by`` function is used. With the directory

.. code-block:: bash

    img_r001_c001_DAPI.tif
    img_r002_c001_DAPI.tif
    img_r001_c001_TXREAD.tif
    img_r002_c001_TXREAD.tif
    img_r001_c001_GFP.tif
    img_r002_c001_GFP.tif


the images can be returned in groups where ``r`` is held constant by passing the parameter ``group_by='r'`` to the object iterator.

.. code-block:: python

    import filepattern as fp
    import pprint

    filepath = "path/to/directory"

    pattern = "img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif"

    files = fp.FilePattern(filepath, pattern)

    for file in files(group_by='r'):
        pprint.pprint(file)


The output is:

.. code-block:: bash

   ('r': 1, [({'c': 1, 'channel': 'DAPI', 'file': 0, 'r': 1},
    ['/path/to/directory/img_r001_c001_DAPI.tif']),
    ({'c': 1, 'channel': 'TXREAD', 'file': 0, 'r': 1},
    ['/path/to/directory/img_r001_c001_TXREAD.tif']),
    ({'c': 1, 'channel': 'GFP', 'file': 0, 'r': 1},
    ['/path/to/directory/img_r001_c001_GFP.tif'])])
   ('r': 2, [({'c': 1, 'channel': 'DAPI', 'file': 0, 'r': 2},
    ['/path/to/directory/img_r002_c001_DAPI.tif']),
    ({'c': 1, 'channel': 'GFP', 'file': 0, 'r': 2},
    ['/path/to/directory/img_r002_c001_GFP.tif']),
    ({'c': 1, 'channel': 'TXREAD', 'file': 0, 'r': 2},
    ['/path/to/directory/img_r002_c001_TXREAD.tif'])])

~~~~~~~~~~~~
Get Matching
~~~~~~~~~~~~

To get files where the variable matches a value, the ``get_matching`` method is used.
For example, if only files from the TXREAD channel are needed, ``get_matching(channel=['TXREAD']`` is called.

.. code-block:: python

    filepath = "/path/to/directory"

    pattern = "img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif"

    files = fp.FilePattern(filepath, pattern)

    matching = files.get_matching(channel=['TXREAD'])

    pprint.pprint(matching)


The output is:


.. code-block:: bash

    [({'c': 1, 'channel': 'TXREAD', 'r': 1},
    ['/path/to/directory/img_r001_c001_TXREAD.tif']),
    ({'c': 1, 'channel': 'TXREAD', 'r': 2},
    ['/path/to/directory/img_r002_c001_TXREAD.tif'])]

~~~~~~~~~~~~~~
Output name
~~~~~~~~~~~~~~
The ``output_name`` method takes in a list of filenames, for example the output of the filepattern iterator, and returns a single filename that captures
all variables from the list of files. If a variable is constant through the list, the variable value will be in the returned
name. If a variable is not constant, the minimum and maximum values will appear in the returned name in
the form "(min-max)".

For example, if the directory of files contains

.. code-block:: bash

    img_r001_c001.tif
    img_r001_c002.tif
    img_r001_c003.tif

The result of creating a ``filepattern`` object with the pattern ``img_r{r:ddd}_c{c:ddd}.tif`` and iterating over the files and appending them to a list will be

.. code-block:: bash

    [({'c': 1, 'r': 1},
    [PosixPath('/path/to/directory/img_r001_c001.tif')]),
    ({'c': 2, 'r': 1},
    [PosixPath('/path/to/directory/img_r001_c002.tif')]),
    ({'c': 3, 'r': 1},
    [PosixPath('/path/to/directory/img_r001_c003.tif')])]

If this list is named ``files``, then ``output_name`` can be used as:

.. code-block:: python

    name = fp_object.output_name(files)

    print(name)

The output is:

.. code-block:: bash

    img_r001_c(001-003).tif

Note that this function currently only works on numeric differences in the files. If there are string differences, such as channel names,
``output_name`` will not work on the files.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Get unique values of variables
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To get the values that occur for each of the variables in a file, the ``get_unique_values``
function is used. This function takes in variable names as string as the arguments and returns
a dictionary mapping the variable name to a set containing the values of the variable.

Consider the directory containing the files

.. code-block:: bash

    img_r001_c001.tif
    img_r001_c002.tif
    img_r001_c003.tif

with the filepattern ``img_r{r:ddd}_c{c:ddd}.tif``. This filepattern contains two variables,
``r`` and ``c``. Therefore, the ``get_unique_values`` function can take in ``'r'``, ``'c'``, or ``'r', 'c'``
as the argument(s). If no arguments are passed, this will have an equivalent return values as
if all variables were passed to the function.

.. code-block:: python

    values = fp_object.get_unique_values('r', 'c')

    print(values)

The output is:

.. code-block:: bash

    {'c': {1, 2, 3}, 'r': {1}}

As mentioned earlier, it is also possible to pass a subset of the available variables:

.. code-block:: python

    values = fp_object.get_unique_values('c')

    print(values)

The output in this case is:

.. code-block:: bash

    {'c': {1, 2, 3}}

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Get unique values of each variable with the number of occurrences
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This function takes in keyword arguments, where the key is a variable name and the value is a list of
values that the variable could have. The ``get_occurrences`` method will return a dictionary
mapping the variable name to another dictionary where this dictionary contains the value of the variable
mapped to the number of times the value occurs.

For example, if there is a directory containing the files

.. code-block:: bash
    
    img_r001_c001_z001.tif
    img_r001_c002_z001.tif
    img_r001_c003_z002.tif

Then ``get_occurrences`` can be used as:

.. code-block:: python

    path = '/path/to/directory'
    pattern = 'img_r{r:ddd}_c{c:ddd}_z{z:ddd}.tif'

    fp_object = fp.FilePattern(path, pattern)

    occurrences = fp_object.get_occurrences(z=[1,2])

    print(occurrences)

The result will be

.. code-block:: bash

    {'z': {1: 2, 2: 1}}

Note that if no arguments are passed to this function, then all variables mapped
to all values will be returned.

If a variable or value is passed that is not matched, then the value will be zero.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Get variables from a filepattern
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To get the variables that are contained in a ``filepattern``, the ``get_variables`` method is used.
This method takes in no arguments and returns a list of strings containing the variable names from
the ``filepattern``. For example,

.. code-block:: python

    path = '/path/to/directory'
    pattern = 'img_r{r:ddd}_c{c:ddd}.tif'

    fp_object = fp.FilePattern(path, pattern)

    variables = fp_object.get_variables()

    print(variables)

the output will be

.. code-block:: bash

    ['r', 'c']

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Capturing directory names
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Directory names can be also be captured when the the `recursive` option is set to `True`. In this case, 
the directory name can be treated the same as a variable in the `filepattern`. For example, if a directory has the
structure:

.. code-block:: bash

    data
        DAPI
            img_r001_c001.tif 
        TXREAD
            img_r001_c001.tif
        GFP
            img_r001_c001.tif

Then the following `filepattern` will capture the directory names.

.. code-block:: python 

    path = '/path/to/data'

    filepattern = '/{directory:c+}/img_r{r:ddd}_c{c:ddd}.tif'
        
    files = fp.FilePattern(path, filepattern, recursive=True)

    for file in files():
        print(file)

The output will be:

.. code-block:: bash

    ({'c': 1, 'directory': 'DAPI', 'r': 1}, ['path/to/data/img_r001_c001.tif'])
    ({'c': 1, 'directory': 'GFP', 'r': 1}, ['path/to/data/img_r001_c001.tif'])
    ({'c': 1, 'directory': 'TXREAD', 'r': 1}, ['path/to/data/img_r001_c001.tif'])


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Pydantic models as return values
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The methods in ``filepattern`` that return nested data types containing the files matching the filepattern
have the option to return the nested structure as Pydantic models by using the flag ``pydantic_output=True``. This applies
to the call method of the filepattern object (with or without ``group_by``) and the ``get_matching`` method. The Pydantic models
are dynamically created at runtime, allowing the fields of the model to be the variables names from the filepattern. For example,

.. code-block:: python

    import filepattern as fp
    import pprint

    filepath = "path/to/directory"

    pattern = "img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif"

    files = fp.FilePattern(filepath, pattern, recursive=True)

    for file in files(pydantic_output=True):
        print(file.r)

will output the ``r`` value for each file that matched the pattern. The path of the file is stored in the ``path`` field. For more information
on Pydantic models, see ``Pydantic Models <https://docs.pydantic.dev/latest/usage/models/>``_.


~~~~~~~~~~
Text files
~~~~~~~~~~

``filepattern`` can also take in a text file as an input rather than a directory.
To use this functionality, a path to a text file is supplied to the ``path`` variable rather than a directory.
When a text file is passed as input, each line of the text file will be matched to the pattern. For example, a
text file containing containing the strings

.. code-block:: bash

    img_r001_c001_DAPI.tif
    img_r001_c001_TXREAD.tif
    img_r001_c001_GFP.tif


can be matched to the pattern ``img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif`` with:

.. code-block:: python

    import filepattern as fp
    import pprint

    filepath = "path/to/file.txt"

    pattern = "img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif"

    files = fp.FilePattern(filepath, pattern)

    for file in files():
        pprint.pprint(file)



The output is:

.. code-block:: bash

    ({'c': 1, 'channel': 'DAPI', 'r': 1},
    ['img_r001_c001_DAPI.tif'])
    ({'c': 1, 'channel': 'TXREAD', 'r': 1},
    ['img_r001_c001_TXREAD.tif'])
    ({'c': 1, 'channel': 'GFP', 'r': 1},
    ['img_r001_c001_GFP.tif']`)


After calling ``filepattern`` on a text file, also contains the [group_by](#group-by) and [get_matching](#get-matching) functionality as outlined in the [FilePattern](#filepattern-section) section.

~~~~~~~~~~~~~~~~~
Stitching Vectors
~~~~~~~~~~~~~~~~~

``filepattern`` can also take in stitching vectors as input. In this case, a path to a text file
containing a stitching vector is passed to the ``path`` variable. A stitching vector has the following form,

.. code-block:: bash

    file: x01_y01_wx0_wy0_c1.ome.tif; corr: 0; position: (0, 0); grid: (0, 0);
    file: x02_y01_wx0_wy0_c1.ome.tif; corr: 0; position: (3496, 0); grid: (3, 0);
    file: x03_y01_wx0_wy0_c1.ome.tif; corr: 0; position: (6992, 0); grid: (6, 0);
    file: x04_y01_wx0_wy0_c1.ome.tif; corr: 0; position: (10488, 0); grid: (9, 0);


This stitching vector can be processed using

.. code-block:: python

    import filepattern as fp
    import pprint

    filepath = 'path/to/stitching/vector.txt'

    pattern = 'x0{x:d}_y01_wx0_wy0_c1.ome.tif'

    files = fp.FilePattern(filepath, pattern)

    for file in files():
        pprint.pprint(files)


The output is:

.. code-block:: bash

    ({'correlation': 0, 'gridX': 0, 'gridY': 0, 'posX': 0, 'posY': 0, 'x': 1},
    ['x01_y01_wx0_wy0_c1.ome.tif'])
    ({'correlation': 0, 'gridX': 3, 'gridY': 0, 'posX': 3496, 'posY': 0, 'x': 2},
    ['x02_y01_wx0_wy0_c1.ome.tif'])
    ({'correlation': 0, 'gridX': 6, 'gridY': 0, 'posX': 6992, 'posY': 0, 'x': 3},
    ['x03_y01_wx0_wy0_c1.ome.tif'])
    ({'correlation': 0, 'gridX': 9, 'gridY': 0, 'posX': 10488, 'posY': 0, 'x': 4},
    ['x04_y01_wx0_wy0_c1.ome.tif'])

As shown in the output, ``filepattern`` not only captures the specified variables from the pattern, but also
captures the variables supplied in the stitching vector.

~~~~~~~~~~~
Out of Core
~~~~~~~~~~~

``filepattern`` has the ability to use external memory when the dataset is too large to fit in main memory,
i.e. it utilizes disk memory along with RAM. It has the same functionality as ``filepattern``, however it takes in an
addition parameter called ``block_size``, which limits the amount of main memory used by ``filepattern``. Consider a
directory containing the files:

.. code-block:: bash

    img_r001_c001_DAPI.tif
    img_r001_c001_TXREAD.tif
    img_r001_c001_GFP.tif


This directory can be processed with only one file in memory as:

.. code-block:: python

    import filepattern as fp
    import pprint

    filepath = "path/to/directory"

    pattern = "img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif"

    files = fp.FilePattern(filepath, pattern, block_size="125 B")


    for file in files():
        pprint.pprint(file)


The output from this example is:

.. code-block:: bash

    ({'c': 1, 'channel': 'DAPI', 'r': 1},
    ['/path/to/directory/img_r001_c001_DAPI.tif'])
    ({'c': 1, 'channel': 'TXREAD', 'r': 1},
    ['/path/to/directory/img_r001_c001_TXREAD.tif'])
    ({'c': 1, 'channel': 'GFP', 'r': 1},
    ['/path/to/directory/img_r001_c001_GFP.tif'])

Note that the ``block_size`` argument is provided in bytes (B) in this example, but also has the options
for kilobytes (KB), megabytes (MB), and gigabytes (GB). The ``block_size`` must be under 1000 GB.


The out of core version of ``filepattern`` contains the same functionalities as the in memory version. ``group_by`` is
called the same way, i.e.,

.. code-block:: python

    for file in files(group_by="r"):
        pprint.pprint(file)


The output remains identical to the in memory version.

The ``get_matching`` functionality remains the same, however the API is slightly different. In this case,
``get_matching`` is called as

.. code-block:: python

    files.get_matching(channel=['TXREAD'])

    for matching in files.get_matching_block()
        pprint.pprint(matching)

where the output is returned in blocks of ``block_size``. The output is:

.. code-block:: bash

    ({'c': 1, 'channel': 'TXREAD', 'r': 1},
    ['/path/to/directory/img_r001_c001_TXREAD.tif'])


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Out of Core: text files and stitching vectors
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Out of core processing can also be used for stitching vectors and text files. To utilize this functionality,
call ``filepattern`` the same way as described previously,
but add in the ``block_size`` parameter, as described in the (Out of Core)[#out-of-core] section.
