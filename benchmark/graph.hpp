#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <set>
#include <algorithm>


class Graph {
public:
  Graph(const std::filesystem::path& path) {
    _graph_path = path / path.filename();
    _graph_path += ".graph_new";
    
    _edge_path = path / path.filename();
    _edge_path += ".edges";
    
    _vertex_path = path / path.filename();
    _vertex_path += ".vertices";

    _statistics_path = path / path.filename();
    _statistics_path += ".statistics";
     
    _build_adjacency_list();

    //_generate_edge_file();

    //_generate_vertex_file();

    //_generate_statistics_file();

    find_linear_chain();
  }

  int get_number_edges() const {
    return _num_edges;
  }

  int get_number_vertices() const {
    return _num_vertices;
  }

  void find_linear_chain() const;

private:
  int _num_edges {0};
  
  int _num_vertices {0};

  std::filesystem::path _graph_path;
  
  std::filesystem::path _edge_path;
  
  std::filesystem::path _vertex_path;
  
  std::filesystem::path _statistics_path;

  std::set<std::string> _vertices;
    
  std::unordered_map<std::string, std::set<std::string>> _adjacency_list;
  
  void _build_adjacency_list();
    
  void _generate_edge_file();

  void _generate_vertex_file();
  
  void _generate_statistics_file();

  void _find_zero_in_edge(std::vector<std::string>&) const;
};

void Graph::_build_adjacency_list() {

  std::ifstream gfile;
  gfile.open(static_cast<std::string>(_graph_path));

  std::string line;
  std::string delimiter = "\" -> \"";
 
  if (gfile.is_open()) {
    while (std::getline(gfile, line)) {
      size_t spos = 0;
      size_t epos = line.find(delimiter);
      std::string token;
      std::string key;
      
      while (epos != std::string::npos) {
        //std::cout << line << '\n'; 
        token = line.substr(spos, epos);
        //std::cout << "token = " << token << std::endl;
        
        key = token.substr(3, token.length());
        if (_adjacency_list.find(key) == _adjacency_list.end()) {
          _adjacency_list[key] = std::set<std::string>{};
        }

        spos = epos + delimiter.length();
        epos = line.find(delimiter, spos);
        token = line.substr(spos, epos);
        _adjacency_list[key].insert(token.substr(0, token.length()-2));

        _vertices.insert(key);
        _vertices.insert(token.substr(0, token.length()-2));

        break;
      }
    }
  }
  else {
    std::cout << "can not open " << _graph_path << '\n';
  }

  gfile.close();

  for (auto& [key, values] : _adjacency_list) {
    for (auto& v : values) {
      ++_num_edges;
    }
  }
  
  _num_vertices = _vertices.size();
}

void Graph::_generate_edge_file() {
  std::ofstream file;
  file.open(static_cast<std::string>(_edge_path));

  for (auto& [key, values] : _adjacency_list) {
    for (auto& v : values) {
      file << key << "->" << v << '\n';
    }
  }

  file.close();
}

void Graph::_generate_vertex_file() {
  std::ofstream file;
  file.open(static_cast<std::string>(_vertex_path));

  for (auto& v : _vertices) {
    file << v << '\n';
  }

  file.close();
}

void Graph::_generate_statistics_file() {
  std::ofstream file;
  file.open(static_cast<std::string>(_statistics_path));
  file << std::setw(8) << "|V|"
       << std::setw(8) << "|E|"
       << std::setw(8) << "|P|"
       << std::setw(8) << "%" << '\n';

  file << std::setw(8) << _num_vertices
       << std::setw(8) << _num_edges
       << std::setw(8) << 6
       << std::setw(8) << 6 << '\n';

  file.close();
}


void Graph::_find_zero_in_edge(std::vector<std::string>& v) const {
  bool hit = false;

  for (auto& vertex : _vertices) {
    hit = false;
    for (auto& [key, values] : _adjacency_list) {
      if (std::find(values.begin(), values.end(), vertex) != values.end()) {
        hit = true;
        break;
      }
    }
    if (!hit) {
      v.push_back(vertex);
    }
  }
}

void Graph::find_linear_chain() const {

  // vertices of zero in edge
  std::vector<std::string> v_zero_in;

  _find_zero_in_edge(v_zero_in);

  for (auto& v: v_zero_in) {
    std::cout << v << '\n';
  }
}
