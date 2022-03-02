#include <taskflow/taskflow.hpp>
#include <taskflow/algorithm/pipeline.hpp>
#include "graph.hpp"
#include <CLI11/CLI11.hpp>
#include <unordered_map>
#include <set>
#include <string>
#include <chrono>
#include <thread>


void pipe_work(const size_t t) {
  std::this_thread::sleep_for(std::chrono::nanoseconds(t));
}

//void pipe_work2(const size_t t) {
//  std::this_thread::sleep_for(std::chrono::nanoseconds(t));
//}
//
//void pipe_work3(const size_t t) {
//  std::this_thread::sleep_for(std::chrono::nanoseconds(t));
//}
//
//void pipe_work4(const size_t t) {
//  std::this_thread::sleep_for(std::chrono::nanoseconds(t));
//}
//
//void pipe_work5(const size_t t) {
//  std::this_thread::sleep_for(std::chrono::nanoseconds(t));
//}

int is_linear_chain(const int candidate, 
  std::vector<std::vector<int>>& linear_chain) {
  for (int i = 0; i < linear_chain.size(); ++i) {
    for (int j = 0; j < linear_chain[i].size(); ++j) {
      if (linear_chain[i][j] == candidate) {
        return i;
      }
    }
  }
  return -1;
}

struct first_pipe {
  size_t num_lines;
  size_t stime;

  void operator()(tf::Pipeflow& pf) {
    if (pf.token() >= num_lines) {
      pf.stop();
    }
    else {
      pipe_work(stime);
    }
  }
};

struct other_pipe {
  size_t stime;

  void operator()(tf::Pipeflow& pf) {
    pipe_work(stime);
  }
};


int main() {

  CLI::App app{"Circuit Pipeline"};

  std::string benchmark = "../benchmark/simple";
  app.add_option("-b, --benchmark", benchmark, "the benchmark (default=s526)");

  size_t time1 = 100;
  app.add_option("--time1", time1, "the sleep time for task1 (default=100ns)");

  size_t time2 = 100;
  app.add_option("--time2", time2, "the sleep time for task2 (default=100ns)");
  
  size_t time3 = 100;
  app.add_option("--time3", time3, "the sleep time for task3 (default=100ns)");

  size_t time4 = 100;
  app.add_option("--time4", time4, "the sleep time for task4 (default=100ns)");
  
  size_t time5 = 100;
  app.add_option("--time5", time5, "the sleep time for task5 (default=100ns)");
  
  size_t threshold = 8;
  app.add_option("--threshold", threshold, 
                 "the threshold of length of linear chain (default=8)");

  std::array<size_t, 5> time_array;
  time_array[0] = time1;
  time_array[1] = time2;
  time_array[2] = time3;
  time_array[3] = time4;
  time_array[4] = time5;

  Graph g{benchmark};
  
  std::vector<std::vector<int>> adjacency_list = g.get_adjacency_list();
  //std::unordered_map<int, std::set<int>> adjacency_list = g.get_adjacency_list();

  std::vector<std::vector<int>> linear_chain = g.get_linear_chain();

  int number_vertices = g.get_number_vertices();
   
  std::vector<tf::Task> tasks(number_vertices);
  
  std::vector<bool> visited(linear_chain.size());
  std::vector<int> lc2sps(number_vertices);
  
  size_t num_lines = 0; 
    
  tf::Executor executor;
  tf::Taskflow taskflow;

  // definition of pipe_callable, used in scalable pipeline
  auto pipe_callable = 
    [&time_array, &num_lines](tf::Pipeflow& pf) mutable {
    
    switch(pf.pipe()) {
      case 0: {
        if (pf.token() >= num_lines) {
          pf.stop();
        }
        else {
          pipe_work(time_array[0]);
        }
        return;
      }
      break;
  
      default: {
        pipe_work(time_array[pf.pipe()]);    
      }
      break;
    }
  };
  

  std::vector< tf::Pipe<std::function<void(tf::Pipeflow&)>> > pipes;
  // 5 pipes
  for(size_t i = 0; i < 5; i++) {
    pipes.emplace_back(tf::PipeType::SERIAL, pipe_callable);
  }

  std::vector<tf::ScalablePipeline<decltype(pipes)::iterator>> sps;

  for (int i = 0; i < number_vertices; ++i) {
    int index = is_linear_chain(i, linear_chain);
    
    // vertex in a linear chain and is the head of the chain
    // construct a pipeline task
    // index is the index in the linear_chain
    if (index != -1) { 
      if (visited[index] == false) {
        visited[index] = true;

        num_lines = linear_chain[index].size(); 
             
        //tf::ScalablePipeline pl(num_lines, pipes.begin(), pipes.end());
        sps.emplace_back(num_lines, pipes.begin(), pipes.end());
        lc2sps[linear_chain[index][0]] = sps.size()-1;
        std::cout << "sps.size()-1 = " << sps.size() - 1 << " , linear_chain[index][0] = " << linear_chain[index][0] << '\n';
        std::cout << "lc2sps["<<linear_chain[index][0] <<"] = " << lc2sps[linear_chain[index][0]] << '\n';
        std::cout << "lc2sps.size() = " << lc2sps.size() << '\n';
        
        tasks[linear_chain[index][0]] 
          = taskflow.composed_of(sps[lc2sps[linear_chain[index][0]]])
                    .name(std::to_string(linear_chain[index][0]));
      }
      continue;
    }

    // vertex not in a linear chain, construct a taskflow task
    else {
      tasks[i] = 
        taskflow.emplace([i, &time_array]() { 
          pipe_work(time_array[0]);
          pipe_work(time_array[1]);
          pipe_work(time_array[2]);
          pipe_work(time_array[3]);
          pipe_work(time_array[4]);
        }).name(std::to_string(i));
    }
  }

  std::vector<bool> built(number_vertices);

  for (int key = 0; key < adjacency_list.size(); ++key) {
  //for (auto& [key, values] : adjacency_list) {
    int index = is_linear_chain(key, linear_chain);
    if (index != -1) {
      if (built[linear_chain[index][0]] == false) {
        int head = linear_chain[index][0];
        int tail = linear_chain[index][linear_chain[index].size()-1];

        for (auto& child : adjacency_list[tail]) {
          tasks[head].precede(tasks[child]);
        }
        built[head] = true;
      }
      continue;
    }

    for (auto& value : adjacency_list[key]) {
      tasks[key].precede(tasks[value]);
    }
  }
  
  taskflow.dump(std::cout);
  auto beg = std::chrono::high_resolution_clock::now();
  executor.run(taskflow).wait();
  auto end = std::chrono::high_resolution_clock::now();
  
  std::cout << "elapsed time = " 
            << std::chrono::duration_cast<std::chrono::microseconds>(end-beg).count() 
            << "ms\n";
  
  return 0;
}


