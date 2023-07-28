/**
 * @file util.hpp
 * @author Jesse McKinzie (Jesse.McKinzie@axleinfo.com)
 * @brief Utility functions to support filepattern.
 * @version 0.1
 * @date 2022-01-26
 *
 * @copyright Copyright (c) 2022
 *
 * Utility functions to operate on std::string, std::map, and Types. Types are defined to be
 * either a int or string and are used to allow for type preservation when extracting
 * variables from filenames. This file also contains type definitions for nested data structures
 * that are used throughout `filepattern`.
 *
 *
 */

#pragma once
#include <regex>
#include <tuple>
#include <variant>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <chrono>
#include <sstream>
#include <utility>
#include <cstring>
#include <iostream>
#include <regex>

#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem> 
  namespace fs = std::experimental::filesystem;
#else
  error "Missing the <filesystem> header."
#endif

using Types = std::variant<int, std::string, double>;
using Map = std::map<std::string, Types>;
#ifdef JAVA_BINDING
using Tuple = std::tuple<Map, std::vector<std::string>>;
#else 
using Tuple = std::tuple<Map, std::vector<fs::path>>;
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
static const std::string SLASH = "\\";
#else
static const std::string SLASH = "/";
#endif


/**
 * @brief Functions for std::strings and Types.
 *
 */
namespace s {

    /**
     * @brief Get the value of a Type as a string or int based on the type.
     *
     */
    struct AnyGet {

        /**
         * @brief Helper method of to_string. Returns an integer as a string
         *
         * @param value Integer to be converted to a string
         * @return std::string String version of the integer
         */
        std::string operator()(int value) { return std::to_string(value); }

        /**
         * @brief Helper method of to_string. Returns string if input is string
         *
         * @param value Value to be converted
         * @return std::string String of the value
         */
        std::string operator()(const std::string& value) { return value; }
    };

    /**
     * @brief Convert a Types to a string.
     *
     * @param input A Type to be converted to a string
     * @return std::string The type value as a string
     */
    inline std::string to_string(const Types& input) {
        return std::visit(AnyGet{}, input);
    }

    /**
     * @brief Returns the amount of memory taken by a Types
     *
     * @param input Types to find the memory footprint of
     * @return int How much memory the Types is using
     */
    inline int size(const Types& input) {
        return input.index() == 0 ? sizeof(int) : sizeof(std::string) + s::to_string(input).length();
    }

    /**
     * @brief Check if a string is a digit.
     *
     * @param s String to be checked
     * @return true String is a numbers
     * @return false String is not a number
     */
    inline bool is_number(const std::string& s) {
        return std::regex_match(s, std::regex("^(-?)(0|([0-9][0-9]*))(\\.[0-9]+)?$"));
    }

    inline bool is_integer(const std::string& s) {
        return std::regex_match(s, std::regex("^(-?)(^0$|^[0-9][0-9]*$)?$"));
    }

    inline bool is_double(const std::string& s) {
        return std::regex_match(s, std::regex("^(-?)(0|([0-9][0-9]*))(\\.[0-9]+)?$"));
    }

    /**
    * @brief Get the basename of a filepath.
    *
    * @param file_path Filepath to find the basename of.
    * @return string The basename of the filepath.
    */
    inline std::string getBaseName(const std::string& file_path) {
        if (file_path.find('/') == std::string::npos && file_path.find('\\') == std::string::npos) return file_path;

        int i = file_path.size() - 1;
        std::string file;
        while (file_path[i] != '/' && file_path[i] != '\\') {
            file.insert(0, 1, file_path[i]);
            --i;
        }
        return file;
    }

    /**
    * @brief Get the basename of a filepath.
    *
    * @param file_path Filepath to find the basename of.
    * @return string The basename of the filepath.
    */
    inline std::string getBaseName(const fs::path& filesystem_path) {
        std::string file_path = filesystem_path.u8string();
        
        if (file_path.find('/') == std::string::npos && file_path.find('\\') == std::string::npos) return file_path;

        int i = file_path.size() - 1;
        std::string file;
        while (file_path[i] != '/' && file_path[i] != '\\') {
            file.insert(0, 1, file_path[i]);
            --i;
        }
        return file;
    }

    /**
     * @brief Split a string on a character
     *
     * Splits a string into a vector of strings along a character.
     * From https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
     *
     * @param s String to be split
     * @param delimiter Character to split string on
     * @return std::vector<std::string> Vector of split strings
     */
    inline std::vector<std::string> split(std::string& s, const std::string& delimiter) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> res;

        while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
            token = s.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }

        res.push_back(s.substr(pos_start));
        return res;
    }

    /**
     * @brief Replace a part of a string with another string
     *
     * Replace a part of a string with another string in place. For example,
     * if s::replace(str="here is a string", "string", "sentence") is called,
     * str will become "here is a sentence".
     *
     * @param str String to replace part of
     * @param from Section of string to replace
     * @param to What to replace the section of the string with
     * @return true Replace successful
     * @return false Replace unsuccessful
     */
    inline bool replace(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = str.find(from);
        if (start_pos == std::string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return true;
    }

    /**
     * @brief Checks if a string ends in another string.
     *
     * From: https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
     *
     * @param value String to check
     * @param ending Ending to check for
     * @return true value ends with ending
     * @return false value does not end with ending
     */
    inline bool endsWith(std::string const& value, std::string const& ending) {

        if (ending.size() > value.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());

    }

    /**
     * @brief Remove spaces from string inplace
     *
     * @param str String to remove spaces from
     */
    inline void removeSpaces(std::string& str) {
        str.erase(remove(str.begin(), str.end(), ' '), str.end());
    }

    /**
     * @brief Pad the front of a string with zeros
     *
     * From https://stackoverflow.com/questions/6143824/add-leading-zeros-to-string-without-sprintf
     *
     * @param s String to pad with zeros
     * @param desired_length Length of string after padding is added
     */
    inline void padWithZeros(std::string& s, unsigned int desired_length) {
        unsigned int number_of_zeros = desired_length - s.length(); // add 2 zeros

        s.insert(0, number_of_zeros, '0');
    }

    /**
     * @brief Get the number of times a character appears in a string
     *
     * @param str String to check
     * @param c Character to check for
     * @return int Number of occurrences of c in str
     */
    inline int getCount(const std::string& str, const char c) {
        int count = 0;
        for (const auto& s : str) {
            if (s == c) ++count;
        }
        return count;
    }

    inline bool contains(const std::string& str, const char c) {
        for (const auto& s : str) {
            if (s == c) return true;
        }
        return false;
    }

    /**
     * @brief Removed substring from string
     *
     * From: https://thispointer.com/how-to-remove-substrings-from-a-string-in-c/
     *
     * @param str String to erase from
     * @param to_erase Substring to erase from mainStr
     */
    inline std::string eraseSubStr(std::string& str, const std::string& to_erase) {
        std::string mainStr = str;
        // Search for the substring in string
        size_t pos = mainStr.find(to_erase);
        if (pos != std::string::npos)
        {
            // If found then erase it from string
            mainStr.erase(pos, to_erase.length());
        }
        return mainStr;
    }

    inline std::string getTimeString() {
        return std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
            ).count());
    }
    /** https://stackoverflow.com/questions/15840718/convert-file-path-from-windows-to-linux-and-back-again-using-boostfilesystem
    */
    inline std::string windows_to_unix_file_path(std::string file_path, bool is_wsl) {
        static const char* const WSL_FILE_PATH_HEADER = "/mnt";
        static const char WINDOWS_FILE_PATH_SEPARATOR = '\\';
        static const char UNIX_FILE_PATH_SEPARATOR = '/';

        // Replace the slashes
        std::replace(file_path.begin(), file_path.end(), WINDOWS_FILE_PATH_SEPARATOR, UNIX_FILE_PATH_SEPARATOR);

        // Convert the drive letter to lowercase
        std::transform(file_path.begin(), file_path.begin() + 1, file_path.begin(),
            [](unsigned char character) {
                return std::tolower(character);
            });

        // Remove the colon
        const auto drive_letter = file_path.substr(0, 1);
        const auto remaining_path = file_path.substr(2, file_path.size() - 2);
        file_path = drive_letter + remaining_path;

        std::stringstream stringstream;

        if (is_wsl) {
            stringstream << WSL_FILE_PATH_HEADER;
        }

        stringstream << "/";
        stringstream << file_path;

        return stringstream.str();
    }
};

namespace m {

    /**
     * @brief Get a map from a .txt file
     *
     * Gets a map from a txt file that contains variables mapped to values.
     *
     * @param infile Input stream
     * @param mapping Map to be modified
     * @return true The end of the file has not been reached and the map is modified
     * @return false The end of the file has been reached and the mao is not modified
     */
    inline bool getMap(std::ifstream& infile, Tuple& member, int map_size) {

        std::string str;
        Map map;

        std::string key, value;
        int valueLength;
        size_t pos;

        std::get<1>(member).clear();

        while (std::getline(infile, str)) {
            
            // if map is correct size, return 

            if (map.size() == (map_size)) {
                std::get<0>(member) = map;
                str.pop_back();
                std::get<1>(member).push_back(str);

                return true;
            }

            // map variable to value 
            pos = str.find(":");
            key = str.substr(0, pos);
            valueLength = str.length() - pos;
            value = str.substr(pos + 1, valueLength);

            map.insert(std::make_pair(key, value));
        }

        return false;
    }

    /**
     * @brief Returns the amount of memory that a Tuple uses
     *
     * @param mapping Tuple to calculate the memory footprint of
     * @return long Amount of memory the Tuple uses
     */
    inline long getSize(Tuple& mapping) {
        long size = 0;
        for (const auto& mem : std::get<0>(mapping)) {
            size += sizeof(mem.first);
            if (s::is_number(s::to_string(mem.second))) size += sizeof(int);
            else size += sizeof(std::string) + s::to_string(mem.second).length();
        }
        size += sizeof(std::vector<std::string>);
        for (const auto& str : std::get<1>(mapping)) {
            #ifdef JAVA_BINDING
            size += str.length();
            #else
            size += str.u8string().length();
            #endif
        }
        return size;
    }

    /**
     * @brief Write map to output file.
     *
     * @param file Output file
     * @param mapping Map to write to output file
     */
    inline void writeMap(std::ofstream& file, Tuple& mapping) {

        for (const auto& element : std::get<0>(mapping)) {
            file << element.first << ":" << s::to_string(element.second) << '\n';
        }

        for (const auto& element : std::get<1>(mapping)) {
            #ifdef JAVA_BINDING
            file << element << "," << '\n';
            #else
            file << element.string() << "," << '\n';
            #endif
        }

    }

    /**
     * @brief Converts the value in a mapping of string to Types to the correct type after reading
     * from a text file.
     *
     * @param mapping The Tuple to convert the Types to the correct type
     */
    inline void preserveType(Tuple& mapping) {
        Types value;
        for (auto& p : std::get<0>(mapping)) {
            value = p.second;
            if (s::is_number(s::to_string(p.second))) {
                if (s::is_integer(s::to_string(p.second))) {
                    value = std::stoi(s::to_string(p.second));
                } else {
                    value = std::stod(s::to_string(p.second));
                }
            }
            p.second = value;
        }
    }

    /**
     * @brief Get the index of the minimum value of a vector of Tuples at a specific variable
     *
     * @param vec Vector to find the minimum value of
     * @param variable Which variable to use to find the minumum value
     * @return int Index of minumum value
     */
    inline int getMinIdx(std::vector<Tuple>& vec, std::string& variable) {
        Types min = std::get<0>(vec[0])[variable];

        int idx = 0;
        int minIdx = 0;
        for (auto& v : vec) {
            if (std::get<0>(v)[variable] < min) {
                min = std::get<0>(v)[variable];
                minIdx = idx;
            }
            ++idx;
        }
        return minIdx;
    }

    /**
     * @brief Get the index of the maximum value of a vector of Tuples at a specific variable
     *
     * @param vec Vector to find the max value of
     * @param variable Which variable to use to find the max value
     * @return int Index of max value
     */
    inline int getMaxIdx(std::vector<Tuple>& vec, std::string& variable) {
        Types max = std::get<0>(vec[0])[variable];

        int idx = 0;
        int maxIdx = 0;
        for (auto& v : vec) {
            if (std::get<0>(v)[variable] > max) {
                max = std::get<0>(v)[variable];
                maxIdx = idx;
            }
            ++idx;
        }
        return maxIdx;
    }
}

namespace d {

    inline void remove_dir(std::string& path_to_dir) {
        if (s::endsWith(path_to_dir, ".txt")) path_to_dir = path_to_dir.substr(0, path_to_dir.find_last_of('/'));
        fs::path path = path_to_dir;
        try {
            uintmax_t n = fs::remove_all(path);
        }
        catch (fs::filesystem_error& e) {}
    }

}

namespace v {

    inline std::vector<Tuple> sliceVector(std::vector<Tuple>& vec, int start, int stop, int step){

        std::vector<Tuple> slice;
        for(int i = start; i < stop; i += step){
            slice.push_back(vec[i]);
        }
        return slice;
    }

    inline bool areEqual(const std::vector<std::string>& vec1, const std::vector<std::string>& vec2){
        if (vec1.size() != vec2.size()) return false;

        auto sorted1 = vec1;
        auto sorted2 = vec2;

        std::sort(sorted1.begin(), sorted1.end());
        std::sort(sorted2.begin(), sorted2.end());

        return std::equal(sorted1.begin(), sorted1.end(), sorted2.begin());
    }
}

namespace f {
    /**
     * https://stackoverflow.com/questions/5207550/in-c-is-there-a-way-to-go-to-a-specific-line-in-a-text-file
     */ 
    inline std::ifstream& goToLine(std::ifstream& file, unsigned int num){

        file.seekg(std::ios::beg);

        for(int i=0; i < num - 1; ++i){
            file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
        
        return file;
    }
}