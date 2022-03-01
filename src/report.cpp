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

  
  for (const auto& p : directories) {
    std::cout << "processing directory = " << p << '\n';
    
    Graph g{p};
    std::cout << "  |V| = " << g.get_number_vertices() << '\n';
    std::cout << "  |E| = " << g.get_number_edges() << '\n';
    std::cout << "  avg_length = " 
              << g.get_average_linear_chain_length() << '\n';
    std::cout << "  % = " 
              << (100.0*g.get_number_linear_chain()/g.get_number_vertices()) 
              << '\n';
  }
  
  //Graph g{"../benchmark/simple_6"};
  //std::cout << "directory = ../benchmark/simple_6" << '\n';
  //std::cout << g.get_number_edges() << '\n';
  //std::cout << g.get_number_vertices() << '\n';
  //std::cout << g.get_average_linear_chain_length() << '\n';


  return 0;
}


