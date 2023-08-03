#include "../../src/filepattern/cpp/include/filepattern.h"
#include "test_truth_data.h"
#include "test_generate_data.h"

static std::string path = "";

static std::string string_path = "";

const static std::string old_pattern = "img_r{rrr}_c{ccc}.tif";

const static std::vector<std::string> patterns = {"img_r{r:ddd}_c{c:ddd}.tif", "img_r{r:d+}_c{c:d+}.tif", old_pattern};

const static int MAX_NUM = 9;

typedef std::vector<std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>>> GroupedVector; 

void create_test_data() {
    auto data_path = generate_data();
    auto channel_path = generate_channel_data();

    path = data_path + "/data100";
    string_path = data_path + "/data100.txt";
}

void test_filepattern() {

    for (auto& pattern: patterns) {

        FilePattern files = FilePattern(path, pattern);

        std::vector<Tuple> old_result = truth[0];
        std::vector<Tuple> result;

        for (auto& file: files) {
            result.push_back(file);
        }

        ASSERT_TRUE(old_result.size() == result.size());
        
        for (int i = 0; i < old_result.size(); ++i) {
            ASSERT_TRUE(std::get<0>(old_result[i])["r"] == std::get<0>(result[i])["r"]);
            ASSERT_TRUE(std::get<0>(old_result[i])["c"] == std::get<0>(result[i])["c"]);
            ASSERT_TRUE(std::get<1>(old_result[i])[0] == s::getBaseName(std::get<1>(result[i])[0]));
        }
    }
}

void test_filepattern_get_matching() {

    for (auto& pattern: patterns) {

        FilePattern files = FilePattern(path, pattern);

        std::vector<Tuple> old_result = truth[1];
        std::vector<Tuple> result;

        std::vector<std::tuple<std::string, std::vector<Types>>> variables = {
            make_tuple("r", std::vector<Types>{0})
        };

        result = files.getMatching(variables);

        ASSERT_TRUE(old_result.size() == result.size());

        for (int i = 0; i < old_result.size(); ++i) {       
            ASSERT_TRUE(std::get<0>(old_result[i])["r"] == std::get<0>(result[i])["r"]);
            ASSERT_TRUE(std::get<0>(old_result[i])["c"] == std::get<0>(result[i])["c"]);
            ASSERT_TRUE(std::get<1>(old_result[i])[0] == s::getBaseName(std::get<1>(result[i])[0]));
        }

        old_result = truth[2];
        variables = {
            make_tuple("r", std::vector<Types>{0, 1})
        };

        result = files.getMatching(variables);

        ASSERT_TRUE(old_result.size() == result.size());

        for (int i = 0; i < old_result.size(); ++i) {       
            ASSERT_TRUE(std::get<0>(old_result[i])["r"] == std::get<0>(result[i])["r"]);
            ASSERT_TRUE(std::get<0>(old_result[i])["c"] == std::get<0>(result[i])["c"]);
            ASSERT_TRUE(std::get<1>(old_result[i])[0] == s::getBaseName(std::get<1>(result[i])[0]));
        }

    }
}

void test_filepattern_group_by() {

    for (auto& pattern: patterns) {

        FilePattern files = FilePattern(path, pattern);

        auto old_result = grouped_truth;
        GroupedVector result;
        std::vector<std::string> vec {"r"};
        for (const auto& file: files.groupBy(vec)) {
            result.push_back(file);
        }

        std::cout << old_result.size() << std::endl;
        std::cout << result.size() << std::endl;
        ASSERT_TRUE(old_result.size() == result.size());
        
        for (int i = 0; i < old_result.size(); ++i) {
            for(int j = 0; j < old_result[i].second.size(); ++j) {
                ASSERT_TRUE(std::get<0>(old_result[i].second[j])["r"] == std::get<0>(result[i].second[j])["r"]);
                ASSERT_TRUE(std::get<0>(old_result[i].second[j])["c"] == std::get<0>(result[i].second[j])["c"]);
                ASSERT_TRUE(std::get<1>(old_result[i].second[j])[0] == s::getBaseName(std::get<1>(result[i].second[j])[0]));
            }
        }
    }
}


void test_stringpattern() {

    for (auto& pattern: patterns) {

        FilePattern files = FilePattern(string_path, pattern);

        std::vector<Tuple> old_result = truth[0];
        std::vector<Tuple> result;

        for (auto& file: files) {
            result.push_back(file);
        }

        ASSERT_TRUE(old_result.size() == result.size());
        
        for (int i = 0; i < old_result.size(); ++i) {
            ASSERT_TRUE(std::get<0>(old_result[i])["r"] == std::get<0>(result[i])["r"]);
            ASSERT_TRUE(std::get<0>(old_result[i])["c"] == std::get<0>(result[i])["c"]);
            ASSERT_TRUE(std::get<1>(old_result[i])[0] == s::getBaseName(std::get<1>(result[i])[0]));
        }
    }
}

void test_stringpattern_get_matching() {

    for (auto& pattern: patterns) {

        FilePattern files = FilePattern(string_path, pattern);

        std::vector<Tuple> old_result = truth[1];
        std::vector<Tuple> result;

        std::vector<std::tuple<std::string, std::vector<Types>>> variables = {
            make_tuple("r", std::vector<Types>{0})
        };

        result = files.getMatching(variables);

        ASSERT_TRUE(old_result.size() == result.size());

        for (int i = 0; i < old_result.size(); ++i) {       
            ASSERT_TRUE(std::get<0>(old_result[i])["r"] == std::get<0>(result[i])["r"]);
            ASSERT_TRUE(std::get<0>(old_result[i])["c"] == std::get<0>(result[i])["c"]);
            ASSERT_TRUE(std::get<1>(old_result[i])[0] == s::getBaseName(std::get<1>(result[i])[0]));
        }

        old_result = truth[2];
        variables = {
            make_tuple("r", std::vector<Types>{0, 1})
        };

        result = files.getMatching(variables);

        ASSERT_TRUE(old_result.size() == result.size());

        for (int i = 0; i < old_result.size(); ++i) {       
            ASSERT_TRUE(std::get<0>(old_result[i])["r"] == std::get<0>(result[i])["r"]);
            ASSERT_TRUE(std::get<0>(old_result[i])["c"] == std::get<0>(result[i])["c"]);
            ASSERT_TRUE(std::get<1>(old_result[i])[0] == s::getBaseName(std::get<1>(result[i])[0]));
        }

    }
}

void test_stringpattern_group_by() {

    for (auto& pattern: patterns) {

        FilePattern files = FilePattern(string_path, pattern);

        auto old_result = grouped_truth;
        GroupedVector result;
        std::vector<std::string> vec {"r"};
        for (const auto& file: files.groupBy(vec)) {
            result.push_back(file);
        }

        std::cout << old_result.size() << std::endl;
        std::cout << result.size() << std::endl;
        ASSERT_TRUE(old_result.size() == result.size());
        
        for (int i = 0; i < old_result.size(); ++i) {
            for(int j = 0; j < old_result[i].second.size(); ++j) {
                ASSERT_TRUE(std::get<0>(old_result[i].second[j])["r"] == std::get<0>(result[i].second[j])["r"]);
                ASSERT_TRUE(std::get<0>(old_result[i].second[j])["c"] == std::get<0>(result[i].second[j])["c"]);
                ASSERT_TRUE(std::get<1>(old_result[i].second[j])[0] == s::getBaseName(std::get<1>(result[i].second[j])[0]));
            }
        }
    }
}

/*
void test_external_filepattern() {

    for (auto& pattern: patterns) {

        FilePattern files = FilePattern(path, pattern, "1 GB");

        std::vector<Tuple> old_result = truth[0];
        std::vector<Tuple> result;

        for (auto& file: files) {
            result.push_back(file);
        }

        ASSERT_TRUE(old_result.size() == result.size());
        
        for (int i = 0; i < old_result.size(); ++i) {
            ASSERT_TRUE(std::get<0>(old_result[i])["r"] == std::get<0>(result[i])["r"]);
            ASSERT_TRUE(std::get<0>(old_result[i])["c"] == std::get<0>(result[i])["c"]);
            ASSERT_TRUE(std::get<1>(old_result[i])[0] == s::getBaseName(std::get<1>(result[i])[0]));
        }
    }
}

void test_external_filepattern_get_matching() {

    for (auto& pattern: patterns) {

        FilePattern files = FilePattern(path, pattern, "1 GB");

        std::vector<Tuple> old_result = truth[1];
        std::vector<Tuple> result;

        std::vector<std::tuple<std::string, std::vector<Types>>> variables = {
            make_tuple("r", std::vector<Types>{0})
        };

        result = files.getMatching(variables);

        ASSERT_TRUE(old_result.size() == result.size());

        for (int i = 0; i < old_result.size(); ++i) {       
            ASSERT_TRUE(std::get<0>(old_result[i])["r"] == std::get<0>(result[i])["r"]);
            ASSERT_TRUE(std::get<0>(old_result[i])["c"] == std::get<0>(result[i])["c"]);
            ASSERT_TRUE(std::get<1>(old_result[i])[0] == s::getBaseName(std::get<1>(result[i])[0]));
        }

        old_result = truth[2];
        variables = {
            make_tuple("r", std::vector<Types>{0, 1})
        };

        result = files.getMatching(variables);

        ASSERT_TRUE(old_result.size() == result.size());

        for (int i = 0; i < old_result.size(); ++i) {       
            ASSERT_TRUE(std::get<0>(old_result[i])["r"] == std::get<0>(result[i])["r"]);
            ASSERT_TRUE(std::get<0>(old_result[i])["c"] == std::get<0>(result[i])["c"]);
            ASSERT_TRUE(std::get<1>(old_result[i])[0] == s::getBaseName(std::get<1>(result[i])[0]));
        }

    }
}

void test_external_filepattern_group_by() {

    for (auto& pattern: patterns) {

        FilePattern files = FilePattern(path, pattern, "1 GB");

        auto old_result = grouped_truth;
        GroupedVector result;
        std::vector<std::string> vec {"r"};
        for (const auto& file: files.groupBy(vec)) {
            result.push_back(file);
        }

        std::cout << old_result.size() << std::endl;
        std::cout << result.size() << std::endl;
        ASSERT_TRUE(old_result.size() == result.size());
        
        for (int i = 0; i < old_result.size(); ++i) {
            for(int j = 0; j < old_result[i].second.size(); ++j) {
                ASSERT_TRUE(std::get<0>(old_result[i].second[j])["r"] == std::get<0>(result[i].second[j])["r"]);
                ASSERT_TRUE(std::get<0>(old_result[i].second[j])["c"] == std::get<0>(result[i].second[j])["c"]);
                ASSERT_TRUE(std::get<1>(old_result[i].second[j])[0] == s::getBaseName(std::get<1>(result[i].second[j])[0]));
            }
        }
    }
}





void test_external_stringpattern() {

    for (auto& pattern: patterns) {

        FilePattern files = FilePattern(string_path, pattern, "1 GB");

        std::vector<Tuple> old_result = truth[0];
        std::vector<Tuple> result;

        for (auto& file: files) {
            result.push_back(file);
        }

        ASSERT_TRUE(old_result.size() == result.size());
        
        for (int i = 0; i < old_result.size(); ++i) {
            ASSERT_TRUE(std::get<0>(old_result[i])["r"] == std::get<0>(result[i])["r"]);
            ASSERT_TRUE(std::get<0>(old_result[i])["c"] == std::get<0>(result[i])["c"]);
            ASSERT_TRUE(std::get<1>(old_result[i])[0] == s::getBaseName(std::get<1>(result[i])[0]));
        }
    }
}

void test_external_stringpattern_get_matching() {

    for (auto& pattern: patterns) {

        FilePattern files = FilePattern(string_path, pattern, "1 GB");

        std::vector<Tuple> old_result = truth[1];
        std::vector<Tuple> result;

        std::vector<std::tuple<std::string, std::vector<Types>>> variables = {
            make_tuple("r", std::vector<Types>{0})
        };

        result = files.getMatching(variables);

        ASSERT_TRUE(old_result.size() == result.size());

        for (int i = 0; i < old_result.size(); ++i) {       
            ASSERT_TRUE(std::get<0>(old_result[i])["r"] == std::get<0>(result[i])["r"]);
            ASSERT_TRUE(std::get<0>(old_result[i])["c"] == std::get<0>(result[i])["c"]);
            ASSERT_TRUE(std::get<1>(old_result[i])[0] == s::getBaseName(std::get<1>(result[i])[0]));
        }

        old_result = truth[2];
        variables = {
            make_tuple("r", std::vector<Types>{0, 1})
        };

        result = files.getMatching(variables);

        ASSERT_TRUE(old_result.size() == result.size());

        for (int i = 0; i < old_result.size(); ++i) {       
            ASSERT_TRUE(std::get<0>(old_result[i])["r"] == std::get<0>(result[i])["r"]);
            ASSERT_TRUE(std::get<0>(old_result[i])["c"] == std::get<0>(result[i])["c"]);
            ASSERT_TRUE(std::get<1>(old_result[i])[0] == s::getBaseName(std::get<1>(result[i])[0]));
        }

    }
}

void test_external_stringpattern_group_by() {

    for (auto& pattern: patterns) {

        FilePattern files = FilePattern(string_path, pattern, "1 GB");

        auto old_result = grouped_truth;
        GroupedVector result;
        std::vector<std::string> vec {"r"};
        for (const auto& file: files.groupBy(vec)) {
            result.push_back(file);
        }

        std::cout << old_result.size() << std::endl;
        std::cout << result.size() << std::endl;
        ASSERT_TRUE(old_result.size() == result.size());
        
        for (int i = 0; i < old_result.size(); ++i) {
            for(int j = 0; j < old_result[i].second.size(); ++j) {
                ASSERT_TRUE(std::get<0>(old_result[i].second[j])["r"] == std::get<0>(result[i].second[j])["r"]);
                ASSERT_TRUE(std::get<0>(old_result[i].second[j])["c"] == std::get<0>(result[i].second[j])["c"]);
                ASSERT_TRUE(std::get<1>(old_result[i].second[j])[0] == s::getBaseName(std::get<1>(result[i].second[j])[0]));
            }
        }
    }
}
*/