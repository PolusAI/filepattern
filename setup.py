import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()
    
with open("VERSION",'r') as fh:
    version = fh.read()

setuptools.setup(
    name="filepattern",
    version=version,
    author="Jesse McKinzie, Nick Schaub",
    author_email="Jesse.McKinzie@axleinfo.com, nick.schaub@nih.gov",
    description="Utilities for parsing files in a directory based on a file name pattern.",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/LabShare/filepattern",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    project_urls={
        'Documentation': 'https://filepattern.readthedocs.io/en/latest/',
        'Source': 'https://github.com/Labshare/filepattern'
    },
    python_requires='>=3.6',
)