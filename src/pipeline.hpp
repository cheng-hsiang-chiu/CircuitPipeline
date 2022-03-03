#include <iostream>
#include <chrono>
#include <taskflow/taskflow.hpp>
#include <taskflow/algorithm/pipeline.hpp>
#include "graph.hpp"
#include <vector>
#include <thread>

void pipe_work(const size_t t) {
  std::this_thread::sleep_for(std::chrono::nanoseconds(t));
}

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

void circuit_pipeline(
  const std::string benchmark,
  const std::vector<size_t>& time_array) {

  Graph g{benchmark};
  
  std::vector<std::vector<int>> adjacency_list = g.get_adjacency_list();

  std::vector<std::vector<int>> linear_chain = g.get_linear_chain();

  int number_vertices = g.get_number_vertices();
   
  std::vector<tf::Task> tasks(number_vertices);
  
  std::vector<bool> visited(linear_chain.size());
  
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

  std::list<tf::ScalablePipeline<decltype(pipes)::iterator>> sps;

  for (int i = 0; i < number_vertices; ++i) {
    int index = is_linear_chain(i, linear_chain);
    
    // vertex in a linear chain and is the head of the chain
    // construct a pipeline task
    // index is the index in the linear_chain
    if (index != -1) { 
      if (visited[index] == false) {
        visited[index] = true;

        num_lines = linear_chain[index].size(); 
             
        auto& sp = sps.emplace_back(num_lines, pipes.begin(), pipes.end());
        tasks[linear_chain[index][0]] 
          = taskflow.composed_of(sp);
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
  
  executor.run(taskflow).wait();
}

void circuit_pipeline() {
  std::cout << "this is a test\n";
}

std::chrono::microseconds measure_time_pipeline(
  const std::string benchmark,
  const std::vector<size_t>& time_array){
  auto beg = std::chrono::high_resolution_clock::now();
  circuit_pipeline(benchmark, time_array);
  //circuit_pipeline();
  auto end = std::chrono::high_resolution_clock::now();
  
  return std::chrono::duration_cast<std::chrono::microseconds>(end - beg);
}
