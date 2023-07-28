#!/usr/bin/env bash
set -e

#----------------------------------------------------------------------
#
# Usage: $bash build_conda.sh $FILEPATTERN_SRC_ROOT
#
if [ -z "$1" ]
then
      echo "No path to the Filepattern source location provided"
      echo "Usage: \$bash build_conda.sh \$FILEPATTERN_SRC_ROOT"
      echo "       where \$FILEPATTERN_SRC_ROOT points to location of Filepattern source"
      exit
fi

MINICONDA=$PWD/miniconda-for-filepattern # Modify this to your preferred location for persistance
CPP_BUILD_DIR=$PWD
SRC_ROOT=$1 #source dir location
FILEPATTERN_ROOT=$SRC_ROOT
PYTHON=3.7

git config --global --add safe.directory $FILEPATTERN_ROOT

#----------------------------------------------------------------------
# Run these only once

function setup_miniconda() {
  MINICONDA_URL="https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh"
  curl -L $MINICONDA_URL -o miniconda.sh
  echo "installing miniconda"
  bash miniconda.sh -b -p $MINICONDA
  rm -f miniconda.sh
  LOCAL_PATH=$PATH
  export PATH="$MINICONDA/bin:$PATH"

  echo "updating miniconda"
  conda update -y -q conda
  conda config --set auto_update_conda false
  conda info -a

  conda config --set show_channel_urls True
  conda config --add channels https://repo.continuum.io/pkgs/free
  conda config --add channels conda-forge

  echo "install dependencies"
  conda create -y -n filepattern-$PYTHON -c conda-forge  \
        python=$PYTHON \
        --file ${SRC_ROOT}/ci-utils/envs/conda_cpp.txt \
        --file ${SRC_ROOT}/ci-utils/envs/conda_py.txt \

  export PATH=$LOCAL_PATH
}

setup_miniconda

#----------------------------------------------------------------------
# Activate conda in bash and activate conda environment

. $MINICONDA/etc/profile.d/conda.sh
conda activate filepattern-$PYTHON
export FILEPATTERN_HOME=$CONDA_PREFIX
#Build CLI
export CMAKE_ARGS="-DCMAKE_PREFIX_PATH=$CONDA_PREFIX -DCMAKE_INSTALL_PREFIX=$CONDA_PREFIX -DBUILD_CLI=ON -DUSEGPU=ON"
echo $CMAKE_ARGS
mkdir -p $CPP_BUILD_DIR
pushd $CPP_BUILD_DIR
cmake -DCMAKE_PREFIX_PATH=$CONDA_PREFIX \
      -DCMAKE_INSTALL_PREFIX=$CONDA_PREFIX \
      -DBUILD_CLI=ON \
      -DUSEGPU=ON \
      $FILEPATTERN_ROOT

cmake --build . --parallel 4
