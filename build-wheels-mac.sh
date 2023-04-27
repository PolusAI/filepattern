#!/bin/bash

export MACOSX_DEPLOYMENT_TARGET=11.0

PYTHON_VERSIONS=("p3.6" "p3.7" "p3.8" "p3.9")

for PYTHON_VERSION in ${PYTHON_VERSIONS[@]}; do
    source /opt/anaconda3/bin/activate ${PYTHON_VERSION}
    env CMAKE_ARGS="-DPython_ROOT_DIR=/opt/anaconda3/bin/envs/${PYTHON_VERSION}/" python setup.py bdist_wheel -d dist
done