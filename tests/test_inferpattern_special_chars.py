# -*- coding: utf-8 -*-
"""
Tests for pattern inference with special (non-alphanumeric) characters in filenames.

Regression tests for GitHub issue #102, which caused RuntimeError when files
contained special characters like periods, hyphens, or underscores in their names
(e.g., .ome.tif extension). The error message was "Non-numeric, non-alphabetic
characters found that do not match" during pattern inference.
"""

import pytest
import filepattern as fp


class TestInferPatternSpecialChars():
    """Test cases for pattern inference with special characters in filenames."""

    def test_ome_tif_extension(self):
        """Test inference with .ome.tif double extension."""
        files = [
            "image_001.ome.tif",
            "image_002.ome.tif",
            "image_003.ome.tif"
        ]
        pattern = fp.infer_pattern(files=files)
        # Should successfully infer pattern without throwing RuntimeError
        assert pattern == "image_00{r:d}.ome.tif"

    def test_multiple_periods(self):
        """Test inference with multiple periods in filename."""
        files = [
            "exp.data.v1.001.tif",
            "exp.data.v1.002.tif",
            "exp.data.v1.003.tif"
        ]
        pattern = fp.infer_pattern(files=files)
        assert pattern == "exp.data.v1.00{r:d}.tif"

    def test_hyphens_and_underscores(self):
        """Test inference with hyphens and underscores."""
        files = [
            "sample-001_exp_01.tif",
            "sample-002_exp_02.tif",
            "sample-003_exp_03.tif"
        ]
        pattern = fp.infer_pattern(files=files)
        assert pattern == "sample-00{r:d}_exp_0{t:d}.tif"

    def test_complex_special_chars(self):
        """Test inference with multiple types of special characters."""
        files = [
            "Week1_22141.ome.001.tif",
            "Week1_22141.ome.002.tif",
            "Week1_22141.ome.003.tif"
        ]
        pattern = fp.infer_pattern(files=files)
        assert pattern == "Week1_22141.ome.00{r:d}.tif"

    def test_ome_tif_with_variable_naming(self):
        """Test .ome.tif files with custom variable names."""
        files = [
            "cell_x01_y02_z03.ome.tif",
            "cell_x04_y05_z06.ome.tif",
            "cell_x07_y08_z09.ome.tif"
        ]
        pattern = fp.infer_pattern(files=files, variables="xyz")
        assert pattern == "cell_x0{x:d}_y0{y:d}_z0{z:d}.ome.tif"
