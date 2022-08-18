/// @copyright 2021 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Esteban Castañeda Blanco <esteban.castaneda@ucr.ac.cr>
/// @author Daniel Lizano Morales <daniel.lizanomorales@ucr.ac.cr>
/// @author Andrea Ramírez Rojas <andrea.ramirezrojas@ucr.ac.cr>
/// @author Carlos Ramírez Masís <carlos.ramirezmasis@ucr.ac.cr>

#include "Plate.hpp"

Plate::Plate(std::string name, int64_t rows, int64_t columns,
  std::vector<double> initialTemperatures, int64_t stageDuration,
  double thermalDiffusivity, int64_t dimensions, double epsilon) {
  this->name = name;
  this->rows = rows;
  this->initialTemperatures = initialTemperatures;
  this->columns = columns;
  this->stageDuration = stageDuration;
  this->thermalDiffusivity = thermalDiffusivity;
  this->dimensions = dimensions;
  this->epsilon = epsilon;
  this->breakeven = false;
  this->state = 0;
}

Plate::~Plate() {
}

void Plate::start() {
  this->createBorderMatrix();
  this->createTemperaturesMatrix();
  while (!this->breakeven) {
    this->updatePlate();
    ++this->state;
  }
  this->createReport();
}

void Plate::createTemperaturesMatrix() {
  int64_t number = 0;
  double temperature;
  this->temperaturesMatrix.resize(this->rows);
  for (int64_t index = 0; index < this->rows; ++index) {
    this->temperaturesMatrix[index].resize(this->columns);
    for (int64_t jindex = 0; jindex < this->columns; ++jindex) {
      temperature = this->initialTemperatures[number];
      this->temperaturesMatrix[index][jindex] = temperature;
      ++number;
    }
  }
}

void Plate::createBorderMatrix() {
  this->borderMatrix.resize(this->rows);
  for (int64_t index = 0; index < this->rows; ++index) {
    this->borderMatrix[index].resize(this->columns);
    for (int64_t jindex = 0; jindex < this->columns; ++jindex) {
      if (index == 0 || index == this->rows - 1 ||
        jindex == 0 || jindex == this->columns - 1)
        this->borderMatrix[index][jindex] = true;
      else
        this->borderMatrix[index][jindex] = false;
    }
  }
}

void Plate::updatePlate() {
  std::vector<std::vector<double>> updatedMatrix = this->temperaturesMatrix;
  double difference = 0;
  this->breakeven = true;
  for (int64_t index = 0; index < this->rows; ++index) {
    for (int64_t jindex = 0; jindex < this->columns; ++jindex) {
      if (!this->borderMatrix[index][jindex])
        updatedMatrix[index][jindex] =
          this->updateTemperature(index, jindex);
      difference = updatedMatrix[index][jindex] -
        this->temperaturesMatrix[index][jindex];
      if (difference > this->epsilon)
        this->breakeven = false;
    }
  }
  this->temperaturesMatrix = updatedMatrix;
}

double Plate::updateTemperature(int64_t row, int64_t column) {
  double currentTemperature = this->temperaturesMatrix[row][column];
  double otherTemperature1 = this->temperaturesMatrix[row - 1][column];
  double otherTemperature2 = this->temperaturesMatrix[row + 1][column];
  double otherTemperature3 = this->temperaturesMatrix[row][column - 1];
  double otherTemperature4 = this->temperaturesMatrix[row][column + 1];
  double otherTemperatures = otherTemperature1 + otherTemperature2 +
    otherTemperature3 + otherTemperature4;
  double newTemperature = currentTemperature +
    ((this->stageDuration * this->thermalDiffusivity)
    / pow(this->dimensions, 2)) * (otherTemperatures - 4 * currentTemperature);
  return newTemperature;
}

std::string Plate::formatTime() {
  std::string time = "";
  int64_t seconds = this->state * this->stageDuration;
  int64_t minutes = seconds / 60;
  int64_t hours = minutes / 60;
  int64_t days = hours / 24;
  int64_t months = days / 30;
  int64_t years = months / 12;

  std::string secondsString;
  std::string minutesString;
  std::string hoursString;
  std::string daysString;
  std::string monthsString;
  std::string yearsString;

  secondsString = seconds % 60 > 9 ? std::to_string(seconds % 60) :
    "0" + std::to_string(seconds % 60);
  minutesString = minutes % 60 > 9 ? std::to_string(minutes % 60) :
    "0" + std::to_string(minutes % 60);
  hoursString = hours % 24 > 9 ? std::to_string(hours % 24) :
    "0" + std::to_string(hours % 24);
  daysString = days % 30 > 9 ? std::to_string(days % 30) :
    "0" + std::to_string(days % 30);
  monthsString = months % 12 > 9 ? std::to_string(months % 12) :
    "0" + std::to_string(months % 12);
  yearsString = years > 9 ? std::to_string(years) :
    "0" + std::to_string(years);
  if ( years > 99 && years < 999)
    yearsString = "0" + yearsString;

  time = yearsString + "/" + monthsString + "/" + daysString + " "
    + hoursString + ":" +
    minutesString + ":" + secondsString;

  return time;
}

void Plate::createReport() {
  std::string time = this->formatTime();
  this->report = this->name + " " +
    std::to_string(this->state) + " " +time;
}

std::string Plate::getReport() {
  return this->report;
}

std::string Plate::getName() {
  return this->name;
}

int64_t Plate::getState() {
  return this->state;
}

int64_t Plate::getRows() {
  return this->rows;
}

int64_t Plate::getColumns() {
  return this->columns;
}

std::vector<std::vector<double>> Plate::getTemperaturesMatrix() {
  return this->temperaturesMatrix;
}
