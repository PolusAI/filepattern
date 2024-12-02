#include "../cpp/include/filepattern.h"
#include "../cpp/interface/filepattern_factory.h"

class FilePatternJava : public FilePattern {
public:
    FilePatternJava(const std::string& path, const std::string& filePattern, const std::string& block_size, bool recursive, bool suppressWarnings) :
    FilePattern(path, filePattern, block_size, recursive) {

        FilePatternFactory fpf = FilePatternFactory();

        std::vector<std::string> file_array {};
        this->fp_ = std::unique_ptr<PatternObject>(fpf.getObject(path, filePattern, block_size, recursive, suppressWarnings, file_array));

        if (block_size != "") {
            this->fp_->external = true;
        } else {
            this->fp_->external = false;
        }
    }

};