#include <iostream>
#include <chrono>
#include <taskflow/taskflow.hpp>
#include "graph.hpp"
#include <vector>
#include <thread>


void task(const size_t t) {
  std::this_thread::sleep_for(std::chrono::nanoseconds(t));
}

std::chrono::microseconds circuit_taskflow(
  const std::string benchmark,
  const std::vector<size_t>& time_array) {

  Graph g{benchmark};
  
  std::vector<std::vector<int>> adjacency_list = g.get_adjacency_list();

  int number_vertices = g.get_number_vertices();
   
  std::vector<tf::Task> tasks;
  tasks.resize(number_vertices);
   
  tf::Executor executor;
  tf::Taskflow taskflow;

  for (int i = 0; i < number_vertices; ++i) {
    tasks[i] = 
      taskflow.emplace([i, &time_array]() { 
        task(time_array[0]);
        task(time_array[1]);
        task(time_array[2]);
        task(time_array[3]);
        task(time_array[4]);
      }).name(std::to_string(i));
  }

  // build dependency
  for (int key = 0; key < adjacency_list.size(); ++key) {
    for (auto& value : adjacency_list[key]) {
      tasks[key].precede(tasks[value]);
    }
  }

  //taskflow.dump(std::cout);
  // run the executor
  auto beg = std::chrono::high_resolution_clock::now();
  executor.run(taskflow).wait();
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(end - beg);
}

//void circuit_taskflow() {
//  std::cout << "this is a test\n";
//}

std::chrono::microseconds measure_time_taskflow(
  const std::string benchmark,
  const std::vector<size_t>& time_array){
  return circuit_taskflow(benchmark, time_array);
}
