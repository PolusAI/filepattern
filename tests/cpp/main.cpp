#include <iostream>
#include "../../src/filepattern/cpp/interface/filepattern.h"
#include "test_truth_data.h"
#include <unordered_map>
#include <variant>



void test01()
{
    std::string path = "/mnt/hdd8/axle/data/intensity1";
    std::string pattern = "p03_x{x:dd}_y{y:dd}_wx0_wy0_c{c:d}.ome.tif";

    auto* files = new FilePattern(path, pattern);
    std::cout<<"pattern is " <<files->getPattern()<<std::endl;;

    auto vars = files->getUniqueValues(files->getVariables());

    for(auto& v:vars){
        
        std::cout << v.first <<std::endl;
        auto set_of_vals = v.second;
        for(auto& val: set_of_vals){
            std::cout<<std::get<int>(val)<<" ";
        }
    }
    auto var_list = files->getVariables();
    auto fv = files->getFiles();
    for(const auto& [mp, vl]: fv){
        for (const auto& [key, value] : mp){
            std::cout << key<< " "<< std::get<int>(value) <<std::endl;
        }
        
        for(auto var: var_list){

            std::cout << var << ":   ";
            auto it = mp.find(var);
            if(it != mp.end()){
                auto& val = std::get<int>(it->second);
            }
            
        }
        std::cout << std::endl;
        for(auto& f: vl){
            std::cout << f << " \n";
        }

    }
    delete files;
}

int main(){
    std::cout <<"hola " << std::endl;
    test01();
} 