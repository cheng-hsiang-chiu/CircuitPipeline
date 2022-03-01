#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include "graph.hpp"

//void generate_statistics(const std::filesystem::path& d) {
//  std::filesystem::path target = d / d.filename();
//  target += ".dependencies";
//  std::cout << static_cast<std::string>(target) << '\n'; 
//  std::ifstream file;
//  file.open(static_cast<std::string>(target));
//  
//  std::string line;
//  int num_edge = -1; // because the first line is heading
//
//  if(file.is_open()) {
//    while(std::getline(file, line)) {
//      num_edge++;
//      //std::cout << line << '\n';
//    }
//  }
//  else {
//    std::cout << "can not open\n";
//  }
//  
//  std::cout << "|E| = " << num_edge << '\n';
//  file.close();
//}

//class Graph {
//public:
//  Graph(const std::filesystem::path& path) {
//    _graph_path = path / path.filename();
//    _graph_path += ".graph_new";
//    
//    _edge_path = path / path.filename();
//    _edge_path += ".edges";
//    
//    _vertex_path = path / path.filename();
//    _vertex_path += ".vertices";
//
//    _statistics_path = path / path.filename();
//    _statistics_path += ".statistics";
//     
//    _build_adjacency_list();
//
//    _generate_edge_file();
//
//    _generate_vertex_file();
//
//    _generate_statistics_file();
//  }
//
//  int get_number_edges() const {
//    return _num_edges;
//  }
//
//  int get_number_vertices() const {
//    return _num_vertices;
//  }
//
//private:
//  int _num_edges {0};
//  
//  int _num_vertices {0};
//
//  std::filesystem::path _graph_path;
//  
//  std::filesystem::path _edge_path;
//  
//  std::filesystem::path _vertex_path;
//  
//  std::filesystem::path _statistics_path;
//
//  std::set<std::string> _vertices;
//    
//  std::unordered_map<std::string, std::set<std::string>> _adjacency_list;
//  
//  void _build_adjacency_list();
//    
//  void _generate_edge_file();
//
//  void _generate_vertex_file();
//  
//  void _generate_statistics_file();
//};
//
//void Graph::_build_adjacency_list() {
//
//  std::ifstream gfile;
//  gfile.open(static_cast<std::string>(_graph_path));
//
//  std::string line;
//  std::string delimiter = "\" -> \"";
// 
//  std::set<std::string> vertices;
//   
//  if (gfile.is_open()) {
//    while (std::getline(gfile, line)) {
//      size_t spos = 0;
//      size_t epos = line.find(delimiter);
//      std::string token;
//      std::string key;
//      
//      while (epos != std::string::npos) {
//        //std::cout << line << '\n'; 
//        token = line.substr(spos, epos);
//        //std::cout << "token = " << token << std::endl;
//        
//        key = token.substr(3, token.length());
//        if (_adjacency_list.find(key) == _adjacency_list.end()) {
//          _adjacency_list[key] = std::set<std::string>{};
//        }
//
//        spos = epos + delimiter.length();
//        epos = line.find(delimiter, spos);
//        token = line.substr(spos, epos);
//        _adjacency_list[key].insert(token.substr(0, token.length()-2));
//
//        _vertices.insert(key);
//        _vertices.insert(token.substr(0, token.length()-2));
//
//        break;
//      }
//    }
//  }
//  else {
//    std::cout << "can not open " << gpath << '\n';
//  }
//
//  gfile.close();
//
//  for (auto& [key, values] : _adjacency_list) {
//    //std::cout << key << " : ";
//    for (auto& v : values) {
//      ++_num_edges;
//      //std::cout << v << ", ";
//    }
//    //std::cout << '\n';
//  }
//  
//  _num_vertices = _vertices.size();
//}
//
//void Graph::_generate_edge_file() {
//  std::ofstream file;
//  file.open(static_cast<std::string>(_edge_path));
//
//  for (auto& [key, values] : _adjacency_list) {
//    for (auto& v : values) {
//      file << key << "->" << v << '\n';
//    }
//  }
//
//  file.close();
//}
//
//void Graph::_generate_vertex_file() {
//  std::ofstream file;
//  file.open(static_cast<std::string>(_vertex_path));
//
//  for (auto& v : _vertices) {
//    file << v << '\n';
//  }
//
//  file.close();
//}
//
//void Graph::_generate_statistics_file() {
//  std::ofstream file;
//  file.open(static_cast<std::string>(_statistics_path));
//
//  file.close();
//}

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
    std::cout << "directory = " << p << '\n';
    //std::cout << "what the hell\n";
    Graph g{p};
    std::cout << g.get_number_edges() << '\n';
    std::cout << g.get_number_vertices() << '\n';
  }
  
  //Graph g{"../benchmark/simple_6"};
  //std::cout << "directory = ../benchmark/simple_6" << '\n';
  //std::cout << g.get_number_edges() << '\n';
  //std::cout << g.get_number_vertices() << '\n';
  //std::cout << g.get_average_linear_chain_length() << '\n';


  return 0;
}


