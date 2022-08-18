/// @copyright 2021 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Esteban Castañeda Blanco <esteban.castaneda@ucr.ac.cr>
/// @author Daniel Lizano Morales <daniel.lizanomorales@ucr.ac.cr>
/// @author Andrea Ramírez Rojas <andrea.ramirezrojas@ucr.ac.cr>
/// @author Carlos Ramírez Masís <carlos.ramirezmasis@ucr.ac.cr>

#ifndef HEATSIMULATOR_HPP
#define HEATSIMULATOR_HPP

#include <omp.h>
#include <unistd.h>

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>

#include "Plate.hpp"

/**
 * @brief Creates and runs the simulation by extracting the data from the files
 */
class HeatSimulator {
 private:
  /// A vector of Plate objects
  std::vector<Plate*> plates;
  /// Indicates the number of threads required
  int64_t threadNumber;
  /// Indicates the route or path when is located the file to open
  std::string route;
  /// Indicates the job folder
  std::string jobFolder;
  /// Stores the input file which will be opened and analyzed
  std::ifstream inputFile;
  /// Stores the report file which contains the solution obtained
  std::ofstream reportFile;

  /**
   * @brief Read the input file and extract all the data
   */
  void readFile();

  /**
   * @brief Analyze the arguments given to the program
   * @param argc Indicates the total count of arguments
   * @param argv A vector of the arguments
   */
  void analyzeArguments(int argc, char* argv[]);

  /**
   * @brief Gets the job folder given by argument
   * @return The job folder given by argument
   */
  std::string extractJobFolder();

  /**
   * @brief Extract the data from the text file
   * @param jobForAnalyze The text file to analyze
   * @return A vector with the data extracted
   */
  std::vector<double> extractTextData(std::string jobForAnalyze);

  /**
   * @brief Extract the data from the binary file
   * @param binaryFileName The binary file to analyze
   * @return A vector with the data extracted
   */
  std::vector<double> extractBinaryData(std::string binaryFileName);

  /**
   * @brief Creates the report file with the general results
   */
  void createReportFile();

  /**
   * @brief Creates the plate file with the results obtained from the calculations
   * @param plate A plate ready to be put in the plate file
   */
  void createPlateFile(Plate* plate);

 public:
  /**
   * @brief The constructor of HeatSimulator class
   */
  HeatSimulator();

  /**
   * @brief The destructor of HeatSimulator class
   */
  ~HeatSimulator();

  /**
   * @brief Starts the simulation
   * @param argc Indicates the total count of arguments
   * @param argv A vector of the arguments
   */
  void run(int argc, char* argv[]);
};


#endif  // !HEATSIMULATOR_HPP

