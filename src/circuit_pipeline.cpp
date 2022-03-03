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


int main(int argc, char* argv[]) {

  CLI::App app{"Circuit Pipeline"};

  size_t design = 0;
  app.add_option("-b, --benchmark", design, "the benchmark id (default=0)");

  size_t time1 = 1736*4;
  //size_t time1 = 0;
  app.add_option("--time1", time1, "the sleep time for task1 (default=1736ns)");

  size_t time2 = 288*4;
  //size_t time2 = 0;
  app.add_option("--time2", time2, "the sleep time for task2 (default=288ns)");
  
  size_t time3 = 206*4;
  //size_t time3 = 0;
  app.add_option("--time3", time3, "the sleep time for task3 (default=206ns)");

  size_t time4 = 55*4;
  //size_t time4 = 0;
  app.add_option("--time4", time4, "the sleep time for task4 (default=55ns)");
  
  size_t time5 = 1235*4;
  //size_t time5 = 0;
  app.add_option("--time5", time5, "the sleep time for task5 (default=1235ns)");
  
  size_t threshold = 2;
  app.add_option("-t, --threshold", threshold, 
                 "the threshold of length of linear chain (default=2)");

  CLI11_PARSE(app, argc, argv);
  
  std::vector<size_t> time_array;
  time_array.push_back(time1);
  time_array.push_back(time2);
  time_array.push_back(time3);
  time_array.push_back(time4);
  time_array.push_back(time5);
  time_array.push_back(time1);
  time_array.push_back(time2);
  time_array.push_back(time3);
  time_array.push_back(time4);
  time_array.push_back(time5);
  time_array.push_back(time1);
  time_array.push_back(time2);
  time_array.push_back(time3);
  time_array.push_back(time4);
  time_array.push_back(time5);
  time_array.push_back(time1);
  time_array.push_back(time2);
  time_array.push_back(time3);
  time_array.push_back(time4);
  time_array.push_back(time5);

  std::string benchmark = "../benchmark/s526";
  if (design > 0) {
    benchmark = benchmark + "_design";
    benchmark = benchmark + std::to_string(design);
  }
  
  std::cout << "benchmark = " << benchmark << ", threshold = " << threshold << '\n';
  
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
  // 15 pipes
  for(size_t i = 0; i < 20; i++) {
    pipes.emplace_back(tf::PipeType::SERIAL, pipe_callable);
  }

  //std::vector<tf::ScalablePipeline<decltype(pipes)::iterator>> sps;
  std::list<tf::ScalablePipeline<decltype(pipes)::iterator>> sps;


  for (int i = 0; i < number_vertices; ++i) {
    int index = is_linear_chain(i, linear_chain);
    
    // vertex in a linear chain and is the head of the chain
    // construct a pipeline task
    // index is the index in the linear_chain
    if (index != -1 && linear_chain[index].size() >= threshold) { 
      if (visited[index] == false) {
        visited[index] = true;

        num_lines = linear_chain[index].size(); 
             
        //tf::ScalablePipeline pl(num_lines, pipes.begin(), pipes.end());
        auto& sp = sps.emplace_back(num_lines, pipes.begin(), pipes.end());
        //lc2sps[linear_chain[index][0]] = sps.size()-1;
        //std::cout << "sps.size()-1 = " << sps.size() - 1 << " , linear_chain[index][0] = " << linear_chain[index][0] << '\n';
        //std::cout << "lc2sps["<<linear_chain[index][0] <<"] = " << lc2sps[linear_chain[index][0]] << '\n';
        //std::cout << "lc2sps.size() = " << lc2sps.size() << '\n';
        
        //tasks[linear_chain[index][0]] 
        //  = taskflow.composed_of(sps[lc2sps[linear_chain[index][0]]])
        //            .name(std::to_string(linear_chain[index][0]));
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
          pipe_work(time_array[5]);
          pipe_work(time_array[6]);
          pipe_work(time_array[7]);
          pipe_work(time_array[8]);
          pipe_work(time_array[9]);
          pipe_work(time_array[10]);
          pipe_work(time_array[11]);
          pipe_work(time_array[12]);
          pipe_work(time_array[13]);
          pipe_work(time_array[14]);
          pipe_work(time_array[15]);
          pipe_work(time_array[16]);
          pipe_work(time_array[17]);
          pipe_work(time_array[18]);
          pipe_work(time_array[19]);
        }).name(std::to_string(i));
    }
  }

  std::vector<bool> built(number_vertices);

  for (int key = 0; key < adjacency_list.size(); ++key) {
  //for (auto& [key, values] : adjacency_list) {
    int index = is_linear_chain(key, linear_chain);
    if (index != -1 && linear_chain[index].size() >= threshold) {
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


