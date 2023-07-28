#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem> 
  namespace fs = std::experimental::filesystem;
#else
  error "Missing the <filesystem> header."
#endif


std::string generate_data() {
    const int MAX = 100;
    int length = 0;

    std::string directory = "test_data";
    std::string root_directory = fs::current_path().string();
    std::string path = (fs::path(root_directory) / directory).string();
    std::string data_path = path + "/data" + std::to_string(MAX);

    try {
        fs::create_directory(path);
        std::cout << "Data directory created at " << path << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Data directory already exists" << std::endl;
    }

    try {
        fs::create_directory(data_path);
        std::cout << "Data directory created at " << data_path << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Data directory already exists" << std::endl;
    }

    std::string name = "";
    int limit = static_cast<int>(std::ceil(std::sqrt(MAX)));

    std::ofstream infile(path + "/data" + std::to_string(MAX) + ".txt");

    for (int i = 0; i < limit; ++i) {
        std::string str_i = std::to_string(i);
        while (str_i.length() < 3)
            str_i = "0" + str_i;

        for (int j = 0; j < limit; ++j) {
            std::string str_j = std::to_string(j);
            while (str_j.length() < 3)
                str_j = "0" + str_j;

            length++;
            std::string data_name = "img_r" + str_i + "_c" + str_j + ".tif";
            infile << data_name << std::endl;
            std::ofstream f(data_path + '/' + data_name);
        }
    }
    infile.close();
    std::cout << length << " files generated." << std::endl;

    return path;
}

std::string generate_channel_data() {
    const int MAX = 3;

    std::string directory = "test_data";
    std::string root_directory = fs::current_path().string();
    std::string path = (fs::path(root_directory) / directory).string();
    std::string data_path = path + "/data";
    std::string recursive_path = path + "/recursive_data";

    try {
        fs::create_directory(path);
        fs::create_directory(data_path);
        fs::create_directory(recursive_path);
        std::cout << "Data directory created at " << path << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Data directory already exists" << std::endl;
    }

    std::string name = "";
    std::vector<std::string> channels = { "DAPI", "TXREAD", "GFP" };

    for (const std::string& channel : channels) {
        std::string current_path = recursive_path + "/" + channel;
        try {
            fs::create_directory(current_path);
        }
        catch (const std::exception& e) {
            std::cout << "Channel directory already exists" << std::endl;
        }

        for (int i = 0; i < MAX; ++i) {
            std::string str_i = std::to_string(i);
            while (str_i.length() < 3)
                str_i = "0" + str_i;

            for (int j = 0; j < MAX; ++j) {
                std::string str_j = std::to_string(j);
                while (str_j.length() < 3)
                    str_j = "0" + str_j;

                std::string data_name = "img_r" + str_i + "_c" + str_j + "_" + channel + ".tif";
                std::ofstream f(data_path + "/" + data_name);
                std::ofstream recursive_f(current_path + "/" + "img_r" + str_i + "_c" + str_j + ".tif");
            }
        }
    }
    std::cout << "Files generated." << std::endl;

    return path;
}