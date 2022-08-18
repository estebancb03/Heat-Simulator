/// @copyright 2021 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Esteban Castañeda Blanco <esteban.castaneda@ucr.ac.cr>
/// @author Daniel Lizano Morales <daniel.lizanomorales@ucr.ac.cr>
/// @author Andrea Ramírez Rojas <andrea.ramirezrojas@ucr.ac.cr>
/// @author Carlos Ramírez Masís <carlos.ramirezmasis@ucr.ac.cr>

#ifndef PLATE_HPP
#define PLATE_HPP

#include <math.h>
#include <inttypes.h>
#include<iostream>
#include <string>
#include <vector>

class Plate {
 public:
  /// Indicates when the plate is in breakeven point
  bool breakeven;
  /// The rows of the plate
  int64_t rows;
  /// The columns of the plate
  int64_t columns;
  /// Indicates the number of the state of the plate
  int64_t state;
  /// Indicates the time a state of the plate was calculated
  int64_t stageDuration;
  /// Indicates the dimensions of the plate
  int64_t dimensions;
  /// Indicates the epsilon value
  double epsilon;
  /// Indicates the thermal diffusivity value
  double thermalDiffusivity;
  /// Stores the name of the plate
  std::string name;
  /// Stores the report message
  std::string report;
  /// Indicates if a cell of the matrix (plate) is in a border
  std::vector<std::vector<bool>> borderMatrix;
  /// Stores all the actual temperatures of the plate
  std::vector<std::vector<double>> temperaturesMatrix;
  /// Stores all the initial temperatures of the plate
  std::vector<double> initialTemperatures;

 private:
  /**
   * @brief Creates the plate with the initial temperatures
   */
  void createTemperaturesMatrix();

  /**
   * @brief Creates the plate with the border cells
   */
  void createBorderMatrix();

  /**
   * @brief Pdates the plate to the next state
   */
  void updatePlate();

  /**
   * @brief Creates the report message
   */
  void createReport();

  /**
   * @brief Brings format to the time line, for the report message
   * @return A string with the time to report
   */
  std::string formatTime();

  /**
   * @brief Update a single temperature
   * @param row A number of row
   * @param column A number of column
   * @return The new temperature
   */
  double updateTemperature(int64_t row, int64_t column);

 public:
  /**
   * @brief The constructor of the class Plate
   * @param name The name of the plate
   * @param rows Total amount of rows in the plate
   * @param columns Total amount of columns in the plate
   * @param initialTemperatures A vector which contains the initial temperatures of the plate
   * @param stageDuration Indicates the time a state of the plate was calculated
   * @param thermalDiffusivity Indicates the thermal diffusivity value
   * @param dimensions Indicates the dimensions of the plate
   * @param epsilon Indicates the epsilon value
   */
  Plate(std::string name, int64_t rows, int64_t columns,
    std::vector<double> initialTemperatures, int64_t stageDuration,
    double thermalDiffusivity, int64_t dimensions, double epsilon);

  /**
   * @brief The destructor of the class Plate
   */
  ~Plate();

  /**
   * @brief Create the matrix which represents the plate and does the calculations
   */
  void start();

  /**
   * @brief Returns the report string
   * @return The report string
   */
  std::string getReport();

  /**
   * @brief Returns the name of the plate
   * @return The name of the plate
   */
  std::string getName();

  /**
   * @brief Returns the actual state of the plate
   * @return The actual state of the plate
   */
  int64_t getState();

  /**
   * @brief Returns the total amount of rows in the plate
   * @return The total amount of rows in the plate
   */
  int64_t getRows();

  /**
   * @brief Returns the total amount of columns in the plate
   * @return The total amount of columns in the plate
   */
  int64_t getColumns();

  /**
   * @brief Returns the actual matrix of temperatures in the plate
   * @return The actual matrix of temperatures in the plate
   */
  std::vector<std::vector<double>> getTemperaturesMatrix();
};

#endif  // !PLATE_HPP
