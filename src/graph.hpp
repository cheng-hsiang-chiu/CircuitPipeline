#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <stack>
#include <cassert>
#include <limits>
#include <cmath>
#include <ctime>


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

    _adjacency_list.resize(10000);
         
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

  int get_max_linear_chain_length() const {
    return _max_linear_chain_length;
  }

  int get_min_linear_chain_length() const {
    return _min_linear_chain_length;
  }

  double get_variance_linear_chain_length() const {
    return _variance_linear_chain_length;
  }

  void generate_new_graph(
    const int num_vertices,
    const int length);

  //std::unordered_map<int, std::set<int>> get_adjacency_list() const {
  //  return _adjacency_list;
  //}

  std::vector<std::vector<int>> get_adjacency_list() const {
    return _adjacency_list;
  }

  std::vector<std::vector<int>> get_linear_chain() const {
    return _linear_chain;
  }

private:
  int _num_edges {0};
  
  int _num_vertices {0};

  int _num_linear_chain{0};

  double _avg_linear_chain_length{0.0};
  
  int _max_linear_chain_length{0};
  
  int _min_linear_chain_length{0};
  
  double _variance_linear_chain_length{0.0};
  
  std::filesystem::path _graph_path;
  
  std::filesystem::path _edge_path;
  
  std::filesystem::path _vertex_path;
  
  std::filesystem::path _statistics_path;

  std::vector<Vertex> _vertices;
  
  std::unordered_map<std::string, int> _name2id;
  
  //std::unordered_map<int, std::set<int>> _adjacency_list;
  
  std::vector<std::vector<int>> _adjacency_list;
  
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
  size_t uid = 0;
 
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

        //size_t len1 = _adjacency_list[_name2id[key]].size();
        //_adjacency_list[_name2id[key]].insert(_name2id[value]);

        if (std::find(
            _adjacency_list[_name2id[key]].begin(),
            _adjacency_list[_name2id[key]].end(),
            _name2id[value]) == _adjacency_list[_name2id[key]].end()) {
          _adjacency_list[_name2id[key]].push_back(_name2id[value]);
          ++_vertices[_name2id[value]].in_edge;
          ++_vertices[_name2id[key]].out_edge;
        }

        //if (_adjacency_list[_name2id[key]].size() != len1) {
        //  ++_vertices[_name2id[value]].in_edge;
        //  ++_vertices[_name2id[key]].out_edge;
        //}

        break;
      }
    }
  }
  else {
    std::cout << "can not open " << _graph_path << '\n';
  }

  gfile.close();

  //std::cout << "adjacency_list : \n";
  //for (auto& [key, values] : _adjacency_list) {
  //  //std::cout << _vertices[key].name << " -> ";
  //  for (auto& v : values) {
  //    //std::cout << _vertices[v].name << ' ';
  //    ++_num_edges;
  //  }
  //  //std::cout << '\n';
  //}
 
  for (auto& lists : _adjacency_list) {
    if (lists.size() == 0) {
      continue;
    }
    for (auto& l : lists) {
      ++_num_edges;
    }
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
  
  //for (auto& [key, values] : _adjacency_list) {
  //  for (auto& v : values) {
  //    file << _vertices[key].name << "->" << _vertices[v].name << '\n';
  //  }
  //}

  for (int key = 0; key < _adjacency_list.size(); ++key) {
    for (auto& v : _adjacency_list[key]) {
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
       << std::setw(12) << "avg_length" 
       << std::setw(12) << "max_length" 
       << std::setw(12) << "min_length"
       << std::setw(12) << "var_length" << '\n';

  file << std::setw(8)  << _num_vertices
       << std::setw(8)  << _num_edges
       << std::setw(8)  << _num_linear_chain
       << std::setw(10) << 100.0*_num_linear_chain/_num_vertices
       << std::setw(12) << _avg_linear_chain_length
       << std::setw(12) << _max_linear_chain_length
       << std::setw(12) << _min_linear_chain_length
       << std::setw(12) << _variance_linear_chain_length
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
    if (_adjacency_list[s].size() == 0) {
    //if (_adjacency_list.find(s) == _adjacency_list.end()) {
      if (lchain.size() > 1) { 
        _linear_chain.push_back(lchain);
      }
      lchain.clear();
    }

    int child;
    // s has children
    while (_adjacency_list[s].size() > 0) {
    //while (_adjacency_list.find(s) != _adjacency_list.end()) {
      // has only one child
      if ((_adjacency_list[s]).size() == 1) {
        child = _adjacency_list[s][0];
        //child = *(_adjacency_list[s].begin());
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
    if (_adjacency_list[s].size() == 0) {
    //if (_adjacency_list.find(s) == _adjacency_list.end()) {
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
      //std::cout << _vertices[c].name << ' ';
    }
    //std::cout << '\n';
  }

  // calculate the average length of linear chain
  _avg_linear_chain_length = static_cast<double>(_num_linear_chain)/_linear_chain.size();

  // calculate the maximum and minimum length of linear chain
  int max = 0;
  int min = std::numeric_limits<int>::max();

  for (auto& chain : _linear_chain) {
    //std::cout << "max = " << max << ", min = " << min << ", len = " << chain.size() << '\n';
    max = max > chain.size() ? max : chain.size();
    min = min < chain.size() ? min : chain.size();
  }
  _max_linear_chain_length = max;
  _min_linear_chain_length = min;

  // calculate the vaiance of length of linear chain
  int difference = 0;
  for (auto& chain : _linear_chain) {
    difference += std::pow(chain.size() - _avg_linear_chain_length, 2);
  }
  _variance_linear_chain_length = static_cast<double>(difference) / _linear_chain.size(); 

  // calculate the percentage of length with >= 4 and >= 8
  int cnt4 = 0;
  int cnt8 = 0;

  for (auto& chain : _linear_chain) {
    if (chain.size() >= 4) {
      ++cnt4;
    }
    if (chain.size() >= 8) {
      ++cnt8;
    }
  }
  
  std::cout << ">=4 has count = " << cnt4 << " and % = " << (400.0*cnt4/_num_vertices) << '\n';
  std::cout << ">=8 has count = " << cnt8 << " and % = " << (800.0*cnt8/_num_vertices) << '\n';

}


//std::vector<int> Graph::_find_parents(const int vertex) {
//  std::vector<int> parents;
//
//  for (auto& [key, values] : _adjacency_list) {
//    if (std::find(values.begin(), values.end(), str) != values.end()) {
//      parents.push_back(key);
//    }
//  }
//
//  return parents;
//}



void Graph::generate_new_graph(
  const int num_vertices,
  const int length) {
  
  std::srand(std::time(nullptr));
  
  std::vector<Vertex> new_vertices{_vertices};
  
  std::unordered_map<std::string, int> new_name2id{_name2id};
  
  //std::unordered_map<int, std::set<int>> new_adjacency_list{_adjacency_list};
  
  std::vector<std::vector<int>> new_adjacency_list{_adjacency_list};
  
  std::vector<std::vector<int>> new_linear_chain{_linear_chain};
 
  std::set<int> index;
  
  int counts_added = 0;
  int newid = _num_vertices;

  while (index.size() < num_vertices) {
    index.insert(std::rand()%_num_vertices);
  }
     
  for (auto& idx : index) {
    std::string newname = "thisisdummy:" + std::to_string(counts_added++); 
    for (auto& child : _adjacency_list[idx]) {
      new_adjacency_list[newid].push_back(child);
    }

    Vertex newv;
    newv.name = newname;
    newv.in_edge = 1;
    newv.out_edge = _adjacency_list[idx].size();
    new_vertices.push_back(newv);
    new_name2id[newname] = newid;

    new_adjacency_list[idx].clear();
    new_adjacency_list[idx].push_back(newid++);

    new_vertices[idx].out_edge = 1;

    for (int i = 2; i < length; ++i) { 
      Vertex temp;
      newname = "thisisdummy:" + std::to_string(counts_added++); 
      temp.name = newname;
      temp.in_edge = 1;
      temp.out_edge = 1;
      new_vertices.push_back(temp);
      new_name2id[newname] = newid;
      int original_child = new_adjacency_list[idx][0];
      //int original_child = *(new_adjacency_list[idx].begin());
      new_adjacency_list[idx].clear();
      new_adjacency_list[idx].push_back(newid);
      new_adjacency_list[newid++].push_back(original_child);
    }
  }

  _adjacency_list = new_adjacency_list;
  _name2id = new_name2id;
  _vertices = new_vertices;
  _linear_chain.clear();
  _num_edges += (length-1);
  _num_vertices += (length-1);
  _num_linear_chain = 0;
  find_linear_chain();
  _generate_edge_file();

  _generate_vertex_file();

  _generate_statistics_file();
}
