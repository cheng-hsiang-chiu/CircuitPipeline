#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <stack>
#include <cassert>

class Vertex {
public:
  int in_edge{0};

  int out_edge{0};

  std::string name;
};

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

    find_linear_chain();
    
    _generate_edge_file();

    _generate_vertex_file();

    _generate_statistics_file();
  }

  int get_number_edges() const {
    return _num_edges;
  }

  int get_number_vertices() const {
    return _num_vertices;
  }

  int get_number_linear_chain() const {
    return _num_linear_chain;
  }

  void find_linear_chain();

  void dump_stack(std::stack<int>) const;

  double get_average_linear_chain_length() const {
    return _avg_linear_chain_length;
  }

private:
  int _num_edges {0};
  
  int _num_vertices {0};

  int _num_linear_chain{0};

  double _avg_linear_chain_length{0.0};
  
  std::filesystem::path _graph_path;
  
  std::filesystem::path _edge_path;
  
  std::filesystem::path _vertex_path;
  
  std::filesystem::path _statistics_path;

  std::vector<Vertex> _vertices;
  
  std::unordered_map<std::string, int> _name2id;
  
  std::unordered_map<int, std::set<int>> _adjacency_list;
  
  std::vector<std::vector<int>> _linear_chain;

  void _build_adjacency_list();
    
  void _generate_edge_file();

  void _generate_vertex_file();
  
  void _generate_statistics_file();

  void _find_zero_in_edge(std::vector<int>&);

  //int _calculate_in_edge(std::string&) const;
};

void Graph::_build_adjacency_list() {

  std::ifstream gfile;
  gfile.open(static_cast<std::string>(_graph_path));

  std::string line;
  std::string delimiter = "\" -> \"";
  int uid = 0;
 
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
        Vertex ktemp;
        // key is not in the _name2id yet
        if (_name2id.find(key) == _name2id.end()) {
          ktemp.name = key;
          _name2id[key] = uid;
          assert(_vertices.size() == uid);
          ++uid;
          _vertices.push_back(ktemp);
        }
        
        spos = epos + delimiter.length();
        epos = line.find(delimiter, spos);
        token = line.substr(spos, epos);
        std::string value = token.substr(0, token.length()-2);
        Vertex vtemp;
       
        // value is not in the _name2id yet 
        if (_name2id.find(value) == _name2id.end()) {
          assert(_vertices.size() == uid);
          vtemp.name = value;
          _name2id[value] = uid;
          ++uid;
          _vertices.push_back(vtemp);
        }

        int len1 = _adjacency_list[_name2id[key]].size();
        _adjacency_list[_name2id[key]].insert(_name2id[value]);
        if (_adjacency_list[_name2id[key]].size() != len1) {
          ++_vertices[_name2id[value]].in_edge;
          ++_vertices[_name2id[key]].out_edge;
        }

        break;
      }
    }
  }
  else {
    std::cout << "can not open " << _graph_path << '\n';
  }

  gfile.close();

  //std::cout << "adjacency_list : \n";
  for (auto& [key, values] : _adjacency_list) {
    //std::cout << _vertices[key].name << " -> ";
    for (auto& v : values) {
      //std::cout << _vertices[v].name << ' ';
      ++_num_edges;
    }
    //std::cout << '\n';
  }
  
  _num_vertices = _vertices.size();
  //std::cout << "vertices = \n";
  //for (auto& v : _vertices) {
  //  std::cout << "name = " << v.name 
  //            << " has in = " << v.in_edge 
  //            << " and out = " << v.out_edge << '\n';
  //}
}

void Graph::_generate_edge_file() {
  std::ofstream file;
  file.open(static_cast<std::string>(_edge_path));
  
  for (auto& [key, values] : _adjacency_list) {
    for (auto& v : values) {
      file << _vertices[key].name << "->" << _vertices[v].name << '\n';
    }
  }

  file.close();
}

void Graph::_generate_vertex_file() {
  std::ofstream file;
  file.open(static_cast<std::string>(_vertex_path));

  for (auto& v : _vertices) {
    file << v.name << '\n';
  }

  file.close();
}

void Graph::_generate_statistics_file() {
  std::ofstream file;
  file.open(static_cast<std::string>(_statistics_path));
  file << std::setw(8)  << "|V|"
       << std::setw(8)  << "|E|"
       << std::setw(8)  << "|P|"
       << std::setw(10) << "%" 
       << std::setw(12) << "avg length" << '\n';

  file << std::setw(8)  << _num_vertices
       << std::setw(8)  << _num_edges
       << std::setw(8)  << _num_linear_chain
       << std::setw(10) << 100.0*_num_linear_chain/_num_vertices
       << std::setw(12) << _avg_linear_chain_length
       << '\n';

  file.close();
}


void Graph::_find_zero_in_edge(std::vector<int>& v) {
  for (auto& vertex : _vertices) {
    if (vertex.in_edge == 0) {
      v.push_back(_name2id[vertex.name]);
    }
  }
}

//int Graph::_calculate_in_edge(std::string& str) const {
//  int in_edge = 0;
//
//  for (auto& [key, values] : _adjacency_list) {
//    if (std::find(values.begin(), values.end(), str) != values.end()) {
//      ++in_edge;
//    }
//  }
//  return in_edge;
//}

void Graph::dump_stack(std::stack<int> stk) const {
  std::cout << "elements in stack : ";
  while (!stk.empty()) {
    std::cout << _vertices[stk.top()].name << ' ';
    stk.pop();
  }
  std::cout << '\n';
}

void Graph::find_linear_chain() {
  // vertices of zero in edge
  std::vector<int> v_zero_in;
  _find_zero_in_edge(v_zero_in);

  std::stack<int> stk; 
  std::vector<int> visited;

  for (auto& v : v_zero_in) {
    stk.push(v);
    visited.push_back(v);
  }
  
  std::vector<int> lchain;
    
  while (!stk.empty()) {
    //dump_stack(stk);

    int s = stk.top();
    stk.pop();
    
    if (_vertices[s].in_edge > 1) {
      if (lchain.size() > 1) { 
        _linear_chain.push_back(lchain);
      }
      lchain.clear();
    }
    
    lchain.push_back(s);

    // s has no child
    if (_adjacency_list.find(s) == _adjacency_list.end()) {
      if (lchain.size() > 1) { 
        _linear_chain.push_back(lchain);
      }
      lchain.clear();
    }

    int child;
    // s has children
    while (_adjacency_list.find(s) != _adjacency_list.end()) {
      // has only one child
      if ((_adjacency_list[s]).size() == 1) {
        child = *(_adjacency_list[s].begin());
        // the child has more than one in_edge
        if (_vertices[child].in_edge > 1) {
        //if (_calculate_in_edge(child) > 1) {
          if (lchain.size() > 1) {
            _linear_chain.push_back(lchain);
          }
          lchain.clear();
          
          if (std::find(visited.begin(), visited.end(), child) == visited.end()) {
            stk.push(child);
            visited.push_back(child);
          }
          break;
        }
        // the child has only one in_edge
        else {
          lchain.push_back(child);
          s = child;
        }
      }
      // has more than one children
      else {
        for (auto& c : _adjacency_list[s]) {
          if (std::find(visited.begin(), visited.end(), c) == visited.end()) {
            stk.push(c);
            visited.push_back(c);
          }
        }

        if (lchain.size() > 1) {
          _linear_chain.push_back(lchain);
        }
        lchain.clear();
        break;
      }
    }
    if (_adjacency_list.find(s) == _adjacency_list.end()) {
      if (lchain.size() > 1) { 
        _linear_chain.push_back(lchain);
      }
      lchain.clear();
    }
  }
  
   
  std::cout << "linear chain :::\n"; 
  for (auto& chain : _linear_chain) {
    for (auto& c : chain) {
      ++_num_linear_chain;
      std::cout << _vertices[c].name << ' ';
    }
    std::cout << '\n';
  }

  _avg_linear_chain_length = static_cast<double>(_num_linear_chain)/_linear_chain.size();
}
