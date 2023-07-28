#include <gtest/gtest.h>
#include "test_filepattern.h"

TEST(FILEPATTERN, TEST_FILEPATTERN) {
    test_filepattern();
}

TEST(FILEPATTERN, TEST_FILEPATTERN_GET_MATCHING) {
    test_filepattern_get_matching();
}

TEST(FILEPATTERN, TEST_FILEPATTERN_GROUP_BY) {
    test_filepattern_group_by();
}

TEST(FILEPATTERN, TEST_STRINGPATTERN) {
    test_stringpattern();
}

TEST(FILEPATTERN, TEST_STRINGPATTERN_GET_MATCHING) {
    test_stringpattern_get_matching();
}

TEST(FILEPATTERN, TEST_STRINGPATTERN_GROUP_BY) {
    test_stringpattern_group_by();
}
/*
TEST(FILEPATTERN, TEST_EXTERNAL_FILEPATTERN) {
    test_external_filepattern();
}

TEST(FILEPATTERN, TEST_EXTERNAL_FILEPATTERN_GET_MATCHING) {
    test_external_filepattern_get_matching();
}

TEST(FILEPATTERN, TEST_EXTERNAL_FILEPATTERN_GROUP_BY) {
    test_external_filepattern_group_by();
}

TEST(FILEPATTERN, TEST_EXTERNAL_STRINGPATTERN) {
    test_external_stringpattern();
}

TEST(FILEPATTERN, TEST_EXTERNAL_STRINGPATTERN_GET_MATCHING) {
    test_external_stringpattern_get_matching();
}

TEST(FILEPATTERN, TEST_EXTERNAL_STRINGPATTERN_GROUP_BY) {
    test_external_stringpattern_group_by();
}

*/
int main(int argc, char** argv) {
    create_test_data();
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}