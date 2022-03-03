#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include "graph.hpp"


int main() {
  //std::cout << "Current path is " << std::filesystem::current_path() << '\n'; // (1)
  std::vector<std::filesystem::path> directories;

  // get sub-directories of non-combination circuits in current directory
  for (const auto& entry : std::filesystem::directory_iterator("../benchmark")) {
    if (std::filesystem::is_directory(entry.path()) && 
        static_cast<std::string>(entry.path())[2] != 'c') {
      directories.push_back(entry.path());
      //directories.push_back(static_cast<std::string>(entry.path()));
      //std::cout << entry.path() << '\n';
    }
  }

  //for (const auto& p : directories) {
  //  std::cout << "processing directory = " << p << '\n';
  //  
  //  Graph g{p};
  //  std::cout << "  |V| = " << g.get_number_vertices() << '\n';
  //  std::cout << "  |E| = " << g.get_number_edges() << '\n';
  //  std::cout << "  avg_length = " 
  //            << g.get_average_linear_chain_length() << '\n';
  //  std::cout << "  max_length = " 
  //            << g.get_max_linear_chain_length() << '\n';
  //  std::cout << "  min_length = " 
  //            << g.get_min_linear_chain_length() << '\n';
  //  std::cout << "  variance_length = " 
  //            << g.get_variance_linear_chain_length() << '\n';
  //  std::cout << "  % = " 
  //            << (100.0*g.get_number_linear_chain()/g.get_number_vertices()) 
  //            << '\n';
  //}
 
  
  std::string p = "../benchmark/s526_design10";
  std::cout << "processing directory = " << p << '\n';
  Graph g{p};
  std::cout << "  |V| = " << g.get_number_vertices() << '\n';
  std::cout << "  |E| = " << g.get_number_edges() << '\n';
  std::cout << "  avg_length = " 
            << g.get_average_linear_chain_length() << '\n';
  std::cout << "  max_length = " 
            << g.get_max_linear_chain_length() << '\n';
  std::cout << "  min_length = " 
            << g.get_min_linear_chain_length() << '\n';
  std::cout << "  variance_length = " 
            << g.get_variance_linear_chain_length() << '\n';
  std::cout << "  % = " 
            << (100.0*g.get_number_linear_chain()/g.get_number_vertices()) 
            << '\n';
  


  return 0;
}


