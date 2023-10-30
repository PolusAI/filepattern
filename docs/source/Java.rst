===========================
Java API
===========================

Filepattern also supplies support in Java through bindings from the same C++ backend used by the Python API.
To install the Java package, a Maven build is supplied along with a jar on the Github.

To build the filepattern package from source, clone the Github repo for filepattern and then run `mvn clean install` 
from the root directory of filepattern to build the package. Alternatively, the jar can be downloaded from the Github repo
and used directly.


~~~~~~~~~~~~~~~~~~~~~~
Using the Java package
~~~~~~~~~~~~~~~~~~~~~~

To use the Java api after installing filepattern, import the `filepattern.java` library. Next, a FilePattern object can be constructed
using a Builder, as shown below.

.. code-block:: java

    FilePattern fp = null;
    try {
        fp = new FilePattern.FilePatternBuilder("test_fp_data")
            .recursive(false)
            .filePattern("img_r00{r:d}_c00{c:d}_{channel:c+}.tif")
            .suppressWarnings(false)
            .blockSize("").build();
    } catch (Exception e) {
        System.out.println("Error creating FilePattern object.");
    }

The arguments in the Builder will control the following:

* recursive: Recursively iterate through subdirectories of the given path 
* filePattern: The filepattern to match the filenames in the directory to 
* suppressWarnings: Suppress warnings given by filepattern
* blockSize: The size of blocks used in out-of-core processing

Note that just the path can be supplied when the filepattern is provided within the path.
This allows for directory names to be captured.

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


the images can be returned in groups where ``r`` is held constant by passing the parameter `"r"`` to the iterator.

.. code-block:: java

    FilePattern files = null;
    try {
        files = new FilePattern.FilePatternBuilder("path/to/directory")
            .filePattern("img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif").build();
    } catch (Exception e) {
        System.out.println("Error creating FilePattern object.");
    }

    ArrayList<Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>>> result = new ArrayList<>();

    fp.setGroup("r");

    for (Iterator<?> i = fp.iterator(); i.hasNext(); ) {
        System.out.println((Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>>) i.next());
    }

    

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
For example, if only files from the TXREAD channel are needed, then this channel can be specified in the iterator function as follows.

.. code-block:: java

    FilePattern fp = null;

    FilePattern files = null;
    try {
        files = new FilePattern.FilePatternBuilder("path/to/directory")
            .filePattern("img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif").build();
    } catch (Exception e) {
        System.out.println("Error creating FilePattern object.");
    }

    ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> result = new ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>();

    HashMap<String, Object> matching = new HashMap<>();
    matching.put("channel", "TXREAD");

    for (Iterator<?> i = fp.iterator(matching); i.hasNext(); ) {
        result.add((Pair<HashMap<String, Object>, ArrayList<Path>>) i.next());
    }

    System.out.println(result)

The output is:

.. code-block:: bash

    [({'c': 1, 'channel': 'TXREAD', 'r': 1},
    ['/path/to/directory/img_r001_c001_TXREAD.tif']),
    ({'c': 1, 'channel': 'TXREAD', 'r': 2},
    ['/path/to/directory/img_r002_c001_TXREAD.tif'])]:

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

.. code-block:: java

    name = fp.outputName(files)

    print(name)

The output is:

.. code-block:: bash

    img_r001_c(001-003).tif

Note that this function currently only works on numeric differences in the files. If there are string differences, such as channel names,
``outputName`` will not work on the files.

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

.. code-block:: java

    values = fp.getUniqueValues("r", "c")

    System.out.println(values)

The output is:

.. code-block:: bash

    {'c': {1, 2, 3}, 'r': {1}}

As mentioned earlier, it is also possible to pass a subset of the available variables:

.. code-block:: java

    values = fp.getUniqueValues("c")

    print(values)

The output in this case is:

.. code-block:: bash

    {'c': {1, 2, 3}}

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Get variables from a filepattern
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To get the variables that are contained in a ``filepattern``, the ``get_variables`` method is used.
This method takes in no arguments and returns a list of strings containing the variable names from
the ``filepattern``. For example,

.. code-block:: java

    FilePattern fp = null;

    FilePattern files = null;
    try {
        files = new FilePattern.FilePatternBuilder("path/to/directory")
            .filePattern("img_r{r:ddd}_c{c:ddd}.tif").build();
    } catch (Exception e) {
        System.out.println("Error creating FilePattern object.");
    }

    variables = fp.getVariables()

    System.out.println(variables)

the output will be

.. code-block:: bash

    ["r", "c"]

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Other input formats
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Java API is also able to process stitching vectors and text files in the same manner as the Python API. For more information 
on these file formats, see the Python documentation. The Java API will handle these formats in the same method
as processing a directory. To use these formats, replace the directory path in the constructor with 
a path to the text file or stitching vector. All of the same methods as the directory parsing can then be used with these formats.