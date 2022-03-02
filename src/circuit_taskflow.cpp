#include <taskflow/taskflow.hpp>
#include "graph.hpp"
#include <CLI11/CLI11.hpp>
#include <unordered_map>
#include <set>
#include <string>
#include <chrono>
#include <thread>


void task1(const size_t t) {
  std::this_thread::sleep_for(std::chrono::nanoseconds(t));
}

void task2(const size_t t) {
  std::this_thread::sleep_for(std::chrono::nanoseconds(t));
}

void task3(const size_t t) {
  std::this_thread::sleep_for(std::chrono::nanoseconds(t));
}

void task4(const size_t t) {
  std::this_thread::sleep_for(std::chrono::nanoseconds(t));
}

void task5(const size_t t) {
  std::this_thread::sleep_for(std::chrono::nanoseconds(t));
}

int main() {

  CLI::App app{"Circuit Taskflow"};

  std::string benchmark = "../benchmark/s526";
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
  
  Graph g{benchmark};
  
  std::unordered_map<int, std::set<int>> adjacency_list = g.get_adjacency_list();

  int number_vertices = g.get_number_vertices();
   
  std::array<tf::Task, 10000> tasks;
   
  tf::Executor executor;
  tf::Taskflow taskflow;

  for (int i = 0; i < number_vertices; ++i) {
    tasks[i] = 
      taskflow.emplace([i, time1, time2, time3, time4, time5]() { 
        task1(time1);
        task2(time2);
        task3(time3);
        task4(time4);
        task5(time5);
      }).name(std::to_string(i));
  }

  for (auto& [key, values] : adjacency_list) {

    //tasks[key] = 
    //  taskflow.emplace([&key, time1, time2, time3, time4, time5]() { 
    //    //std::cout << "parent  = " << key << '\n'; 
    //    task1(time1);
    //    task2(time2);
    //    task3(time3);
    //    task4(time4);
    //    task5(time5);
    //  }).name(std::to_string(key));

    for (auto& value : values) {
      //tasks[value] = 
      //  taskflow.emplace([&value, time1, time2, time3, time4, time5](){
      //    //std::cout << "child = " << value << '\n';  
      //    task1(time1);
      //    task2(time2);
      //    task3(time3);
      //    task4(time4);
      //    task5(time5);
      //  }).name(std::to_string(value));  

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


