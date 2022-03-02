#include <iostream>
#include "graph.hpp"
#include <cstdlib>






int main(int argc, char* argv[]) {

  //std::cout << "generating benchmark in directory = ../benchmark/s526_design1" << '\n';
  //std::string p = "../benchmark/s526_design1";
  //Graph g{p};
  //std::cout << "Orignal graph : " << '\n';
  //std::cout << "  |V| = " << g.get_number_vertices() << '\n';
  //std::cout << "  |E| = " << g.get_number_edges() << '\n';
  //std::cout << "  avg_length = " 
  //          << g.get_average_linear_chain_length() << '\n';
  //std::cout << "  max_length = " 
  //          << g.get_max_linear_chain_length() << '\n';
  //std::cout << "  min_length = " 
  //          << g.get_min_linear_chain_length() << '\n';
  //std::cout << "  variance_length = " 
  //          << g.get_variance_linear_chain_length() << '\n';
  //std::cout << "  % = " 
  //          << (100.0*g.get_number_linear_chain()/g.get_number_vertices()) 
  //          << '\n';

  if (argc < 3) {
    std::cerr << "wrong parameters\n";
  }

  int counts_vertices = atoi(argv[1]);
  int length = atoi(argv[2]);
 
  // std::cout << counts_vertices << " , " << length << '\n'; 
  
  std::cout << "generating benchmark in directory = ../benchmark/s526_design1" << '\n';
  std::string p = "../benchmark/s526_design1";
  Graph g{p};

  g.generate_new_graph(counts_vertices, length);

  return 0;
}
