#include <taskflow/taskflow.hpp>
#include <taskflow/algorithm/pipeline.hpp>
#include <CLI11/CLI11.hpp>
#include <string>
#include <chrono>
#include <thread>
#include <array>
#include "taskflow.hpp"
#include "pipeline.hpp"


int main(int argc, char* argv[]) {

  CLI::App app{"Circuit Pipeline"};

  unsigned design {0};
  app.add_option("-b,--benchmark", design, "the benchmark id (default=0), 0-10 only");

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
  app.add_option("-t,--threshold", threshold, 
                 "the threshold of length of linear chain (default=2)");

  size_t num_rounds = 1;
  app.add_option("-r,--rounds", num_rounds, "the round of executions (default=1)");

  std::string model = "tf";
    app.add_option("-m,--model", model, "model name tf|pl (default=tf)")
      ->check([] (const std::string& m) {
        if(m != "pl" && m != "tf") {
          return "model name should be \"pl\" or \"tf\"";
        }
        return "";
     });

  CLI11_PARSE(app, argc, argv);

  std::string benchmark = "s526";
  if (design > 0) {
    benchmark = benchmark + "_design" + std::to_string(design);
  }
  
  std::vector<size_t> time_array;
  time_array.push_back(time1);
  time_array.push_back(time2);
  time_array.push_back(time3);
  time_array.push_back(time4);
  time_array.push_back(time5);


  double runtime = {0.0};
 
  std::string path = "../benchmark/" + benchmark; 

  for (size_t round = 0; round < num_rounds; ++round) {
    if (model == "tf") {
      runtime += measure_time_taskflow(path, time_array).count();
    }
    else {
      runtime += measure_time_pipeline(path, time_array, threshold).count();
    }
  }

  std::cout << std::setw(6) << "Model"
            << std::setw(14) << "Benchmark"
            << std::setw(8) << "Rounds"
            << std::setw(14) << "Runtime (us)"
            << std::endl;

  std::cout << std::setw(6) << model
            << std::setw(14) << benchmark
            << std::setw(8) << num_rounds
            << std::setw(14) << runtime / num_rounds
            << std::endl;


  
  //std::cout << "elapsed time = " 
  //          << runtime 
  //          << " ms\n";
  
  return 0;
}


