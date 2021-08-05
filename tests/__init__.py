from unittest import TestSuite
from .inference_test import InferenceTest

test_cases = (InferenceTest,
              )

def load_tests(loader, tests, pattern):
    suite = TestSuite()
    for test_class in test_cases:
        tests = loader.loadTestsFromTestCase(test_class)
        suite.addTests(tests)
    return suite