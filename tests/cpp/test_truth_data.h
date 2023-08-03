#include <string>
#include <vector>
#include <tuple>
#include <map>
#include "../../src/filepattern/cpp/util/util.hpp"

#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem> 
  namespace fs = std::experimental::filesystem;
#else
  error "Missing the <filesystem> header."
#endif

std::vector<std::vector<Tuple>> truth = {
 {{std::make_tuple(Map{{"c", 0}, {"r", 0}},
   std::vector<fs::path>{"img_r000_c000.tif"})},
 {std::make_tuple(Map{{"c", 1}, {"r", 0}},
  std::vector<fs::path>{"img_r000_c001.tif"})},
 {std::make_tuple(Map{{"c", 2}, {"r", 0}},
  std::vector<fs::path>{"img_r000_c002.tif"})},
 {std::make_tuple(Map{{"c", 3}, {"r", 0}},
  std::vector<fs::path>{"img_r000_c003.tif"})},
 {std::make_tuple(Map{{"c", 4}, {"r", 0}},
  std::vector<fs::path>{"img_r000_c004.tif"})},
 {std::make_tuple(Map{{"c", 5}, {"r", 0}},
  std::vector<fs::path>{"img_r000_c005.tif"})},
 {std::make_tuple(Map{{"c", 6}, {"r", 0}},
  std::vector<fs::path>{"img_r000_c006.tif"})},
 {std::make_tuple(Map{{"c", 7}, {"r", 0}},
  std::vector<fs::path>{"img_r000_c007.tif"})},
 {std::make_tuple(Map{{"c", 8}, {"r", 0}},
  std::vector<fs::path>{"img_r000_c008.tif"})},
 {std::make_tuple(Map{{"c", 9}, {"r", 0}},
  std::vector<fs::path>{"img_r000_c009.tif"})},
 {std::make_tuple(Map{{"c", 0}, {"r", 1}},
  std::vector<fs::path>{"img_r001_c000.tif"})},
 {std::make_tuple(Map{{"c", 1}, {"r", 1}},
  std::vector<fs::path>{"img_r001_c001.tif"})},
 {std::make_tuple(Map{{"c", 2}, {"r", 1}},
  std::vector<fs::path>{"img_r001_c002.tif"})},
 {std::make_tuple(Map{{"c", 3}, {"r", 1}},
  std::vector<fs::path>{"img_r001_c003.tif"})},
 {std::make_tuple(Map{{"c", 4}, {"r", 1}},
  std::vector<fs::path>{"img_r001_c004.tif"})},
 {std::make_tuple(Map{{"c", 5}, {"r", 1}},
  std::vector<fs::path>{"img_r001_c005.tif"})},
 {std::make_tuple(Map{{"c", 6}, {"r", 1}},
  std::vector<fs::path>{"img_r001_c006.tif"})},
 {std::make_tuple(Map{{"c", 7}, {"r", 1}},
  std::vector<fs::path>{"img_r001_c007.tif"})},
 {std::make_tuple(Map{{"c", 8}, {"r", 1}},
  std::vector<fs::path>{"img_r001_c008.tif"})},
 {std::make_tuple(Map{{"c", 9}, {"r", 1}},
  std::vector<fs::path>{"img_r001_c009.tif"})},
 {std::make_tuple(Map{{"c", 0}, {"r", 2}},
  std::vector<fs::path>{"img_r002_c000.tif"})},
 {std::make_tuple(Map{{"c", 1}, {"r", 2}},
  std::vector<fs::path>{"img_r002_c001.tif"})},
 {std::make_tuple(Map{{"c", 2}, {"r", 2}},
  std::vector<fs::path>{"img_r002_c002.tif"})},
 {std::make_tuple(Map{{"c", 3}, {"r", 2}},
  std::vector<fs::path>{"img_r002_c003.tif"})},
 {std::make_tuple(Map{{"c", 4}, {"r", 2}},
  std::vector<fs::path>{"img_r002_c004.tif"})},
 {std::make_tuple(Map{{"c", 5}, {"r", 2}},
  std::vector<fs::path>{"img_r002_c005.tif"})},
 {std::make_tuple(Map{{"c", 6}, {"r", 2}},
  std::vector<fs::path>{"img_r002_c006.tif"})},
 {std::make_tuple(Map{{"c", 7}, {"r", 2}},
  std::vector<fs::path>{"img_r002_c007.tif"})},
 {std::make_tuple(Map{{"c", 8}, {"r", 2}},
  std::vector<fs::path>{"img_r002_c008.tif"})},
 {std::make_tuple(Map{{"c", 9}, {"r", 2}},
  std::vector<fs::path>{"img_r002_c009.tif"})},
 {std::make_tuple(Map{{"c", 0}, {"r", 3}},
  std::vector<fs::path>{"img_r003_c000.tif"})},
 {std::make_tuple(Map{{"c", 1}, {"r", 3}},
  std::vector<fs::path>{"img_r003_c001.tif"})},
 {std::make_tuple(Map{{"c", 2}, {"r", 3}},
  std::vector<fs::path>{"img_r003_c002.tif"})},
 {std::make_tuple(Map{{"c", 3}, {"r", 3}},
  std::vector<fs::path>{"img_r003_c003.tif"})},
 {std::make_tuple(Map{{"c", 4}, {"r", 3}},
  std::vector<fs::path>{"img_r003_c004.tif"})},
 {std::make_tuple(Map{{"c", 5}, {"r", 3}},
  std::vector<fs::path>{"img_r003_c005.tif"})},
 {std::make_tuple(Map{{"c", 6}, {"r", 3}},
  std::vector<fs::path>{"img_r003_c006.tif"})},
 {std::make_tuple(Map{{"c", 7}, {"r", 3}},
  std::vector<fs::path>{"img_r003_c007.tif"})},
 {std::make_tuple(Map{{"c", 8}, {"r", 3}},
  std::vector<fs::path>{"img_r003_c008.tif"})},
 {std::make_tuple(Map{{"c", 9}, {"r", 3}},
  std::vector<fs::path>{"img_r003_c009.tif"})},
 {std::make_tuple(Map{{"c", 0}, {"r", 4}},
  std::vector<fs::path>{"img_r004_c000.tif"})},
 {std::make_tuple(Map{{"c", 1}, {"r", 4}},
  std::vector<fs::path>{"img_r004_c001.tif"})},
 {std::make_tuple(Map{{"c", 2}, {"r", 4}},
  std::vector<fs::path>{"img_r004_c002.tif"})},
 {std::make_tuple(Map{{"c", 3}, {"r", 4}},
  std::vector<fs::path>{"img_r004_c003.tif"})},
 {std::make_tuple(Map{{"c", 4}, {"r", 4}},
  std::vector<fs::path>{"img_r004_c004.tif"})},
 {std::make_tuple(Map{{"c", 5}, {"r", 4}},
  std::vector<fs::path>{"img_r004_c005.tif"})},
 {std::make_tuple(Map{{"c", 6}, {"r", 4}},
  std::vector<fs::path>{"img_r004_c006.tif"})},
 {std::make_tuple(Map{{"c", 7}, {"r", 4}},
  std::vector<fs::path>{"img_r004_c007.tif"})},
 {std::make_tuple(Map{{"c", 8}, {"r", 4}},
  std::vector<fs::path>{"img_r004_c008.tif"})},
 {std::make_tuple(Map{{"c", 9}, {"r", 4}},
  std::vector<fs::path>{"img_r004_c009.tif"})},
 {std::make_tuple(Map{{"c", 0}, {"r", 5}},
  std::vector<fs::path>{"img_r005_c000.tif"})},
 {std::make_tuple(Map{{"c", 1}, {"r", 5}},
  std::vector<fs::path>{"img_r005_c001.tif"})},
 {std::make_tuple(Map{{"c", 2}, {"r", 5}},
  std::vector<fs::path>{"img_r005_c002.tif"})},
 {std::make_tuple(Map{{"c", 3}, {"r", 5}},
  std::vector<fs::path>{"img_r005_c003.tif"})},
 {std::make_tuple(Map{{"c", 4}, {"r", 5}},
  std::vector<fs::path>{"img_r005_c004.tif"})},
 {std::make_tuple(Map{{"c", 5}, {"r", 5}},
  std::vector<fs::path>{"img_r005_c005.tif"})},
 {std::make_tuple(Map{{"c", 6}, {"r", 5}},
  std::vector<fs::path>{"img_r005_c006.tif"})},
 {std::make_tuple(Map{{"c", 7}, {"r", 5}},
  std::vector<fs::path>{"img_r005_c007.tif"})},
 {std::make_tuple(Map{{"c", 8}, {"r", 5}},
  std::vector<fs::path>{"img_r005_c008.tif"})},
 {std::make_tuple(Map{{"c", 9}, {"r", 5}},
  std::vector<fs::path>{"img_r005_c009.tif"})},
 {std::make_tuple(Map{{"c", 0}, {"r", 6}},
  std::vector<fs::path>{"img_r006_c000.tif"})},
 {std::make_tuple(Map{{"c", 1}, {"r", 6}},
  std::vector<fs::path>{"img_r006_c001.tif"})},
 {std::make_tuple(Map{{"c", 2}, {"r", 6}},
  std::vector<fs::path>{"img_r006_c002.tif"})},
 {std::make_tuple(Map{{"c", 3}, {"r", 6}},
  std::vector<fs::path>{"img_r006_c003.tif"})},
 {std::make_tuple(Map{{"c", 4}, {"r", 6}},
  std::vector<fs::path>{"img_r006_c004.tif"})},
 {std::make_tuple(Map{{"c", 5}, {"r", 6}},
  std::vector<fs::path>{"img_r006_c005.tif"})},
 {std::make_tuple(Map{{"c", 6}, {"r", 6}},
  std::vector<fs::path>{"img_r006_c006.tif"})},
 {std::make_tuple(Map{{"c", 7}, {"r", 6}},
  std::vector<fs::path>{"img_r006_c007.tif"})},
 {std::make_tuple(Map{{"c", 8}, {"r", 6}},
  std::vector<fs::path>{"img_r006_c008.tif"})},
 {std::make_tuple(Map{{"c", 9}, {"r", 6}},
  std::vector<fs::path>{"img_r006_c009.tif"})},
 {std::make_tuple(Map{{"c", 0}, {"r", 7}},
  std::vector<fs::path>{"img_r007_c000.tif"})},
 {std::make_tuple(Map{{"c", 1}, {"r", 7}},
  std::vector<fs::path>{"img_r007_c001.tif"})},
 {std::make_tuple(Map{{"c", 2}, {"r", 7}},
  std::vector<fs::path>{"img_r007_c002.tif"})},
 {std::make_tuple(Map{{"c", 3}, {"r", 7}},
  std::vector<fs::path>{"img_r007_c003.tif"})},
 {std::make_tuple(Map{{"c", 4}, {"r", 7}},
  std::vector<fs::path>{"img_r007_c004.tif"})},
 {std::make_tuple(Map{{"c", 5}, {"r", 7}},
  std::vector<fs::path>{"img_r007_c005.tif"})},
 {std::make_tuple(Map{{"c", 6}, {"r", 7}},
  std::vector<fs::path>{"img_r007_c006.tif"})},
 {std::make_tuple(Map{{"c", 7}, {"r", 7}},
  std::vector<fs::path>{"img_r007_c007.tif"})},
 {std::make_tuple(Map{{"c", 8}, {"r", 7}},
  std::vector<fs::path>{"img_r007_c008.tif"})},
 {std::make_tuple(Map{{"c", 9}, {"r", 7}},
  std::vector<fs::path>{"img_r007_c009.tif"})},
 {std::make_tuple(Map{{"c", 0}, {"r", 8}},
  std::vector<fs::path>{"img_r008_c000.tif"})},
 {std::make_tuple(Map{{"c", 1}, {"r", 8}},
  std::vector<fs::path>{"img_r008_c001.tif"})},
 {std::make_tuple(Map{{"c", 2}, {"r", 8}},
  std::vector<fs::path>{"img_r008_c002.tif"})},
 {std::make_tuple(Map{{"c", 3}, {"r", 8}},
  std::vector<fs::path>{"img_r008_c003.tif"})},
 {std::make_tuple(Map{{"c", 4}, {"r", 8}},
  std::vector<fs::path>{"img_r008_c004.tif"})},
 {std::make_tuple(Map{{"c", 5}, {"r", 8}},
  std::vector<fs::path>{"img_r008_c005.tif"})},
 {std::make_tuple(Map{{"c", 6}, {"r", 8}},
  std::vector<fs::path>{"img_r008_c006.tif"})},
 {std::make_tuple(Map{{"c", 7}, {"r", 8}},
  std::vector<fs::path>{"img_r008_c007.tif"})},
 {std::make_tuple(Map{{"c", 8}, {"r", 8}},
  std::vector<fs::path>{"img_r008_c008.tif"})},
 {std::make_tuple(Map{{"c", 9}, {"r", 8}},
  std::vector<fs::path>{"img_r008_c009.tif"})},
 {std::make_tuple(Map{{"c", 0}, {"r", 9}},
  std::vector<fs::path>{"img_r009_c000.tif"})},
 {std::make_tuple(Map{{"c", 1}, {"r", 9}},
  std::vector<fs::path>{"img_r009_c001.tif"})},
 {std::make_tuple(Map{{"c", 2}, {"r", 9}},
  std::vector<fs::path>{"img_r009_c002.tif"})},
 {std::make_tuple(Map{{"c", 3}, {"r", 9}},
  std::vector<fs::path>{"img_r009_c003.tif"})},
 {std::make_tuple(Map{{"c", 4}, {"r", 9}},
  std::vector<fs::path>{"img_r009_c004.tif"})},
 {std::make_tuple(Map {{"c", 5}, {"r", 9}},
  std::vector<fs::path>{"img_r009_c005.tif"})},
 {std::make_tuple(Map {{"c", 6}, {"r", 9}},
  std::vector<fs::path>{"img_r009_c006.tif"})},
 {std::make_tuple(Map{{"c", 7}, {"r", 9}},
  std::vector<fs::path>{"img_r009_c007.tif"})},
 {std::make_tuple(Map{{"c", 8}, {"r", 9}},
  std::vector<fs::path>{"img_r009_c008.tif"})},
 {std::make_tuple(Map{{"c", 9}, {"r", 9}},
  std::vector<fs::path>{"img_r009_c009.tif"})}},


  {
    {std::make_tuple(Map{{"c", 0}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c000.tif"})},
    {std::make_tuple(Map{{"c", 1}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c001.tif"})},
    {std::make_tuple(Map{{"c", 2}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c002.tif"})},
    {std::make_tuple(Map{{"c", 3}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c003.tif"})},
    {std::make_tuple(Map{{"c", 4}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c004.tif"})},
    {std::make_tuple(Map{{"c", 5}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c005.tif"})},
    {std::make_tuple(Map{{"c", 6}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c006.tif"})},
    {std::make_tuple(Map{{"c", 7}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c007.tif"})},
    {std::make_tuple(Map{{"c", 8}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c008.tif"})},
    {std::make_tuple(Map{{"c", 9}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c009.tif"})}
  },

  {
    {std::make_tuple(Map{{"c", 0}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c000.tif"})},
    {std::make_tuple(Map{{"c", 1}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c001.tif"})},
    {std::make_tuple(Map{{"c", 2}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c002.tif"})},
    {std::make_tuple(Map{{"c", 3}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c003.tif"})},
    {std::make_tuple(Map{{"c", 4}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c004.tif"})},
    {std::make_tuple(Map{{"c", 5}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c005.tif"})},
    {std::make_tuple(Map{{"c", 6}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c006.tif"})},
    {std::make_tuple(Map{{"c", 7}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c007.tif"})},
    {std::make_tuple(Map{{"c", 8}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c008.tif"})},
    {std::make_tuple(Map{{"c", 9}, {"r", 0}},
    std::vector<fs::path>{"img_r000_c009.tif"})},
    {std::make_tuple(Map{{"c", 0}, {"r", 1}},
    std::vector<fs::path>{"img_r001_c000.tif"})},
    {std::make_tuple(Map{{"c", 1}, {"r", 1}},
    std::vector<fs::path>{"img_r001_c001.tif"})},
    {std::make_tuple(Map{{"c", 2}, {"r", 1}},
    std::vector<fs::path>{"img_r001_c002.tif"})},
    {std::make_tuple(Map{{"c", 3}, {"r", 1}},
    std::vector<fs::path>{"img_r001_c003.tif"})},
    {std::make_tuple(Map{{"c", 4}, {"r", 1}},
    std::vector<fs::path>{"img_r001_c004.tif"})},
    {std::make_tuple(Map{{"c", 5}, {"r", 1}},
    std::vector<fs::path>{"img_r001_c005.tif"})},
    {std::make_tuple(Map{{"c", 6}, {"r", 1}},
    std::vector<fs::path>{"img_r001_c006.tif"})},
    {std::make_tuple(Map{{"c", 7}, {"r", 1}},
    std::vector<fs::path>{"img_r001_c007.tif"})},
    {std::make_tuple(Map{{"c", 8}, {"r", 1}},
    std::vector<fs::path>{"img_r001_c008.tif"})},
    {std::make_tuple(Map{{"c", 9}, {"r", 1}},
    std::vector<fs::path>{"img_r001_c009.tif"})}
  }
};

std::vector<std::pair<std::vector<std::pair<fs::path, Types>> , std::vector<Tuple>>> grouped_truth {

   {
   std::make_pair(std::vector<std::pair<fs::path, Types>>{std::make_pair("r", 0)},
      std::vector<Tuple>{{std::make_tuple(Map{{"c", 0}, {"r", 0}},
      std::vector<fs::path>{"img_r000_c000.tif"})},
      {std::make_tuple(Map{{"c", 1}, {"r", 0}},
      std::vector<fs::path>{"img_r000_c001.tif"})},
      {std::make_tuple(Map{{"c", 2}, {"r", 0}},
      std::vector<fs::path>{"img_r000_c002.tif"})},
      {std::make_tuple(Map{{"c", 3}, {"r", 0}},
      std::vector<fs::path>{"img_r000_c003.tif"})},
      {std::make_tuple(Map{{"c", 4}, {"r", 0}},
      std::vector<fs::path>{"img_r000_c004.tif"})},
      {std::make_tuple(Map{{"c", 5}, {"r", 0}},
      std::vector<fs::path>{"img_r000_c005.tif"})},
      {std::make_tuple(Map{{"c", 6}, {"r", 0}},
      std::vector<fs::path>{"img_r000_c006.tif"})},
      {std::make_tuple(Map{{"c", 7}, {"r", 0}},
      std::vector<fs::path>{"img_r000_c007.tif"})},
      {std::make_tuple(Map{{"c", 8}, {"r", 0}},
      std::vector<fs::path>{"img_r000_c008.tif"})},
      {std::make_tuple(Map{{"c", 9}, {"r", 0}},
      std::vector<fs::path>{"img_r000_c009.tif"})}})
   },

   {
   std::make_pair(std::vector<std::pair<fs::path, Types>>{std::make_pair("r", 1)},
      std::vector<Tuple>{{std::make_tuple(Map{{"c", 0}, {"r", 1}},
      std::vector<fs::path>{"img_r001_c000.tif"})},
      {std::make_tuple(Map{{"c", 1}, {"r", 1}},
      std::vector<fs::path>{"img_r001_c001.tif"})},
      {std::make_tuple(Map{{"c", 2}, {"r", 1}},
      std::vector<fs::path>{"img_r001_c002.tif"})},
      {std::make_tuple(Map{{"c", 3}, {"r", 1}},
      std::vector<fs::path>{"img_r001_c003.tif"})},
      {std::make_tuple(Map{{"c", 4}, {"r", 1}},
      std::vector<fs::path>{"img_r001_c004.tif"})},
      {std::make_tuple(Map{{"c", 5}, {"r", 1}},
      std::vector<fs::path>{"img_r001_c005.tif"})},
      {std::make_tuple(Map{{"c", 6}, {"r", 1}},
      std::vector<fs::path>{"img_r001_c006.tif"})},
      {std::make_tuple(Map{{"c", 7}, {"r", 1}},
      std::vector<fs::path>{"img_r001_c007.tif"})},
      {std::make_tuple(Map{{"c", 8}, {"r", 1}},
      std::vector<fs::path>{"img_r001_c008.tif"})},
      {std::make_tuple(Map{{"c", 9}, {"r", 1}},
      std::vector<fs::path>{"img_r001_c009.tif"})}})
   },

   {
   std::make_pair(std::vector<std::pair<fs::path, Types>>{std::make_pair("r", 2)},
      std::vector<Tuple>{{std::make_tuple(Map{{"c", 0}, {"r", 2}},
      std::vector<fs::path>{"img_r002_c000.tif"})},
      {std::make_tuple(Map{{"c", 1}, {"r", 2}},
      std::vector<fs::path>{"img_r002_c001.tif"})},
      {std::make_tuple(Map{{"c", 2}, {"r", 2}},
      std::vector<fs::path>{"img_r002_c002.tif"})},
      {std::make_tuple(Map{{"c", 3}, {"r", 2}},
      std::vector<fs::path>{"img_r002_c003.tif"})},
      {std::make_tuple(Map{{"c", 4}, {"r", 2}},
      std::vector<fs::path>{"img_r002_c004.tif"})},
      {std::make_tuple(Map{{"c", 5}, {"r", 2}},
      std::vector<fs::path>{"img_r002_c005.tif"})},
      {std::make_tuple(Map{{"c", 6}, {"r", 2}},
      std::vector<fs::path>{"img_r002_c006.tif"})},
      {std::make_tuple(Map{{"c", 7}, {"r", 2}},
      std::vector<fs::path>{"img_r002_c007.tif"})},
      {std::make_tuple(Map{{"c", 8}, {"r", 2}},
      std::vector<fs::path>{"img_r002_c008.tif"})},
      {std::make_tuple(Map{{"c", 9}, {"r", 2}},
      std::vector<fs::path>{"img_r002_c009.tif"})}})
   },

   {
   std::make_pair(std::vector<std::pair<fs::path, Types>>{std::make_pair("r", 3)},
      std::vector<Tuple>{{std::make_tuple(Map{{"c", 0}, {"r", 3}},
      std::vector<fs::path>{"img_r003_c000.tif"})},
      {std::make_tuple(Map{{"c", 1}, {"r", 3}},
      std::vector<fs::path>{"img_r003_c001.tif"})},
      {std::make_tuple(Map{{"c", 2}, {"r", 3}},
      std::vector<fs::path>{"img_r003_c002.tif"})},
      {std::make_tuple(Map{{"c", 3}, {"r", 3}},
      std::vector<fs::path>{"img_r003_c003.tif"})},
      {std::make_tuple(Map{{"c", 4}, {"r", 3}},
      std::vector<fs::path>{"img_r003_c004.tif"})},
      {std::make_tuple(Map{{"c", 5}, {"r", 3}},
      std::vector<fs::path>{"img_r003_c005.tif"})},
      {std::make_tuple(Map{{"c", 6}, {"r", 3}},
      std::vector<fs::path>{"img_r003_c006.tif"})},
      {std::make_tuple(Map{{"c", 7}, {"r", 3}},
      std::vector<fs::path>{"img_r003_c007.tif"})},
      {std::make_tuple(Map{{"c", 8}, {"r", 3}},
      std::vector<fs::path>{"img_r003_c008.tif"})},
      {std::make_tuple(Map{{"c", 9}, {"r", 3}},
      std::vector<fs::path>{"img_r003_c009.tif"})}})
   },

   {
   std::make_pair(std::vector<std::pair<fs::path, Types>>{std::make_pair("r", 4)},
      std::vector<Tuple>{{std::make_tuple(Map{{"c", 0}, {"r", 4}},
      std::vector<fs::path>{"img_r004_c000.tif"})},
      {std::make_tuple(Map{{"c", 1}, {"r", 4}},
      std::vector<fs::path>{"img_r004_c001.tif"})},
      {std::make_tuple(Map{{"c", 2}, {"r", 4}},
      std::vector<fs::path>{"img_r004_c002.tif"})},
      {std::make_tuple(Map{{"c", 3}, {"r", 4}},
      std::vector<fs::path>{"img_r004_c003.tif"})},
      {std::make_tuple(Map{{"c", 4}, {"r", 4}},
      std::vector<fs::path>{"img_r004_c004.tif"})},
      {std::make_tuple(Map{{"c", 5}, {"r", 4}},
      std::vector<fs::path>{"img_r004_c005.tif"})},
      {std::make_tuple(Map{{"c", 6}, {"r", 4}},
      std::vector<fs::path>{"img_r004_c006.tif"})},
      {std::make_tuple(Map{{"c", 7}, {"r", 4}},
      std::vector<fs::path>{"img_r004_c007.tif"})},
      {std::make_tuple(Map{{"c", 8}, {"r", 4}},
      std::vector<fs::path>{"img_r004_c008.tif"})},
      {std::make_tuple(Map{{"c", 9}, {"r", 4}},
      std::vector<fs::path>{"img_r004_c009.tif"})}})
   },

   {
   std::make_pair(std::vector<std::pair<fs::path, Types>>{std::make_pair("r", 5)},
      std::vector<Tuple>{{std::make_tuple(Map{{"c", 0}, {"r", 5}},
      std::vector<fs::path>{"img_r005_c000.tif"})},
      {std::make_tuple(Map{{"c", 1}, {"r", 5}},
      std::vector<fs::path>{"img_r005_c001.tif"})},
      {std::make_tuple(Map{{"c", 2}, {"r", 5}},
      std::vector<fs::path>{"img_r005_c002.tif"})},
      {std::make_tuple(Map{{"c", 3}, {"r", 5}},
      std::vector<fs::path>{"img_r005_c003.tif"})},
      {std::make_tuple(Map{{"c", 4}, {"r", 5}},
      std::vector<fs::path>{"img_r005_c004.tif"})},
      {std::make_tuple(Map{{"c", 5}, {"r", 5}},
      std::vector<fs::path>{"img_r005_c005.tif"})},
      {std::make_tuple(Map{{"c", 6}, {"r", 5}},
      std::vector<fs::path>{"img_r005_c006.tif"})},
      {std::make_tuple(Map{{"c", 7}, {"r", 5}},
      std::vector<fs::path>{"img_r005_c007.tif"})},
      {std::make_tuple(Map{{"c", 8}, {"r", 5}},
      std::vector<fs::path>{"img_r005_c008.tif"})},
      {std::make_tuple(Map{{"c", 9}, {"r", 5}},
      std::vector<fs::path>{"img_r005_c009.tif"})}})
   },

   {
   std::make_pair(std::vector<std::pair<fs::path, Types>>{std::make_pair("r", 6)},
      std::vector<Tuple>{{std::make_tuple(Map{{"c", 0}, {"r", 6}},
      std::vector<fs::path>{"img_r006_c000.tif"})},
      {std::make_tuple(Map{{"c", 1}, {"r", 6}},
      std::vector<fs::path>{"img_r006_c001.tif"})},
      {std::make_tuple(Map{{"c", 2}, {"r", 6}},
      std::vector<fs::path>{"img_r006_c002.tif"})},
      {std::make_tuple(Map{{"c", 3}, {"r", 6}},
      std::vector<fs::path>{"img_r006_c003.tif"})},
      {std::make_tuple(Map{{"c", 4}, {"r", 6}},
      std::vector<fs::path>{"img_r006_c004.tif"})},
      {std::make_tuple(Map{{"c", 5}, {"r", 6}},
      std::vector<fs::path>{"img_r006_c005.tif"})},
      {std::make_tuple(Map{{"c", 6}, {"r", 6}},
      std::vector<fs::path>{"img_r006_c006.tif"})},
      {std::make_tuple(Map{{"c", 7}, {"r", 6}},
      std::vector<fs::path>{"img_r006_c007.tif"})},
      {std::make_tuple(Map{{"c", 8}, {"r", 6}},
      std::vector<fs::path>{"img_r006_c008.tif"})},
      {std::make_tuple(Map{{"c", 9}, {"r", 6}},
      std::vector<fs::path>{"img_r006_c009.tif"})}})
   },

   {
   std::make_pair(std::vector<std::pair<fs::path, Types>>{std::make_pair("r", 7)},
      std::vector<Tuple>{{std::make_tuple(Map{{"c", 0}, {"r", 7}},
      std::vector<fs::path>{"img_r007_c000.tif"})},
      {std::make_tuple(Map{{"c", 1}, {"r", 7}},
      std::vector<fs::path>{"img_r007_c001.tif"})},
      {std::make_tuple(Map{{"c", 2}, {"r", 7}},
      std::vector<fs::path>{"img_r007_c002.tif"})},
      {std::make_tuple(Map{{"c", 3}, {"r", 7}},
      std::vector<fs::path>{"img_r007_c003.tif"})},
      {std::make_tuple(Map{{"c", 4}, {"r", 7}},
      std::vector<fs::path>{"img_r007_c004.tif"})},
      {std::make_tuple(Map{{"c", 5}, {"r", 7}},
      std::vector<fs::path>{"img_r007_c005.tif"})},
      {std::make_tuple(Map{{"c", 6}, {"r", 7}},
      std::vector<fs::path>{"img_r007_c006.tif"})},
      {std::make_tuple(Map{{"c", 7}, {"r", 7}},
      std::vector<fs::path>{"img_r007_c007.tif"})},
      {std::make_tuple(Map{{"c", 8}, {"r", 7}},
      std::vector<fs::path>{"img_r007_c008.tif"})},
      {std::make_tuple(Map{{"c", 9}, {"r", 7}},
      std::vector<fs::path>{"img_r007_c009.tif"})}})
   },

   {
   std::make_pair(std::vector<std::pair<fs::path, Types>>{std::make_pair("r", 8)},
      std::vector<Tuple>{{std::make_tuple(Map{{"c", 0}, {"r", 8}},
      std::vector<fs::path>{"img_r008_c000.tif"})},
      {std::make_tuple(Map{{"c", 1}, {"r", 8}},
      std::vector<fs::path>{"img_r008_c001.tif"})},
      {std::make_tuple(Map{{"c", 2}, {"r", 8}},
      std::vector<fs::path>{"img_r008_c002.tif"})},
      {std::make_tuple(Map{{"c", 3}, {"r", 8}},
      std::vector<fs::path>{"img_r008_c003.tif"})},
      {std::make_tuple(Map{{"c", 4}, {"r", 8}},
      std::vector<fs::path>{"img_r008_c004.tif"})},
      {std::make_tuple(Map{{"c", 5}, {"r", 8}},
      std::vector<fs::path>{"img_r008_c005.tif"})},
      {std::make_tuple(Map{{"c", 6}, {"r", 8}},
      std::vector<fs::path>{"img_r008_c006.tif"})},
      {std::make_tuple(Map{{"c", 7}, {"r", 8}},
      std::vector<fs::path>{"img_r008_c007.tif"})},
      {std::make_tuple(Map{{"c", 8}, {"r", 8}},
      std::vector<fs::path>{"img_r008_c008.tif"})},
      {std::make_tuple(Map{{"c", 9}, {"r", 8}},
      std::vector<fs::path>{"img_r008_c009.tif"})}})
   },

   {
   std::make_pair(std::vector<std::pair<fs::path, Types>>{std::make_pair("r", 9)},
      std::vector<Tuple>{{std::make_tuple(Map{{"c", 0}, {"r", 9}},
      std::vector<fs::path>{"img_r009_c000.tif"})},
      {std::make_tuple(Map{{"c", 1}, {"r", 9}},
      std::vector<fs::path>{"img_r009_c001.tif"})},
      {std::make_tuple(Map{{"c", 2}, {"r", 9}},
      std::vector<fs::path>{"img_r009_c002.tif"})},
      {std::make_tuple(Map{{"c", 3}, {"r", 9}},
      std::vector<fs::path>{"img_r009_c003.tif"})},
      {std::make_tuple(Map{{"c", 4}, {"r", 9}},
      std::vector<fs::path>{"img_r009_c004.tif"})},
      {std::make_tuple(Map{{"c", 5}, {"r", 9}},
      std::vector<fs::path>{"img_r009_c005.tif"})},
      {std::make_tuple(Map{{"c", 6}, {"r", 9}},
      std::vector<fs::path>{"img_r009_c006.tif"})},
      {std::make_tuple(Map{{"c", 7}, {"r", 9}},
      std::vector<fs::path>{"img_r009_c007.tif"})},
      {std::make_tuple(Map{{"c", 8}, {"r", 9}},
      std::vector<fs::path>{"img_r009_c008.tif"})},
      {std::make_tuple(Map{{"c", 9}, {"r", 9}},
      std::vector<fs::path>{"img_r009_c009.tif"})}})
   }

};
