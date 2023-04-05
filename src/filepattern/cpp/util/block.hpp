/**
 * @file block.hpp
 * @author Jesse McKinzie (Jesse.McKinzie@axleinfo.com)
 * @brief Class to parse block size input
 * @version 0.1
 * @date 2021-12-21
 * 
 * Block is used to parse a memory size into bytes. A single static method is 
 * in the block class that takes a string as input in the form "<size> <unit>"
 * e.g. "50 MB". The method returns a long double that is the input in bytes.
 */

#pragma once

// logic shifts to convert to bytes
#define KB(x) ((size_t) (x) << 10);
#define MB(x) ((size_t) (x) << 20);
#define GB(x) ((size_t) (x) << 30);

class Block {
    public:
        /**
         * @brief Convert a block size to bytes.
         * 
         * Converts an input string from an number followed by a unit (B, KB, MB, or GB)
         * and returns the number converted to bytes. 
         *
         * @param block The memory size to be converted to bytes.
         * @return long double The memory size in bytes.
         */
        static long parseblockSize(const std::string& block){ 

            std::string num_string = "";
            std::string unit_string = "";
            
            for(const auto& c: block){
                if(isdigit(c) || c == '.') num_string.push_back(c);
                else if(c != ' ') unit_string.push_back(c);
            }

            if (num_string == "") throw std::invalid_argument("Block size must include a size.");
            std::string::size_type sz;
            long num = stoi (num_string, &sz);

            if (unit_string == "KB"){
                num = KB(num);
            } else if (unit_string == "MB") {
                num = MB(num);
            } else if (unit_string == "GB"){
                num = GB(num);
            } else if (unit_string != "B"){
                throw std::invalid_argument("Block size must include a valid unit (B, KB, MB, GB).");
            }

            return num;
        }
};