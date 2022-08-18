/// @copyright 2021 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Esteban Castañeda Blanco <esteban.castaneda@ucr.ac.cr>
/// @author Daniel Lizano Morales <daniel.lizanomorales@ucr.ac.cr>
/// @author Andrea Ramírez Rojindexas <andrea.ramirezrojas@ucr.ac.cr>
/// @author Carlos Ramírez Masís <carlos.ramirezmasis@ucr.ac.cr>

#include "HeatSimulator.hpp"

HeatSimulator::HeatSimulator() {
  this->threadNumber = sysconf(_SC_NPROCESSORS_ONLN);
}

HeatSimulator::~HeatSimulator() {
}

void HeatSimulator::run(int argc, char* argv[]) {
  this->analyzeArguments(argc, argv);
  this->readFile();
  std::filesystem::create_directory("jobs/" + this->jobFolder + "/output/");
  #pragma omp parallel num_threads(this->threadNumber) \
    default(none) shared(plates)
  {
    #pragma omp for schedule(dynamic)
    for (size_t index = 0; index < this->plates.size(); ++index) {
      this->plates[index]->start();
      this->createPlateFile(this->plates[index]);
    }
  }
  this->createReportFile();
}

void HeatSimulator::analyzeArguments(int argc, char* argv[]) {
  if (argc >= 2)
    this->route = argv[1];
  if (argc >= 3)
    this->threadNumber = std::stoi(argv[2]);
  if (argc < 2)
    std::cout << "Wrong argument count" << std::endl;
  this->jobFolder = this->extractJobFolder();
}

void HeatSimulator::readFile() {
  this->inputFile = std::ifstream(this->route);
  std::string line;
  std::string binaryName;
  std::stringstream temp;
  std::vector<double> textData;
  std::vector<double> binaryData;

  int64_t rows;
  int64_t columns;
  std::vector<double> initialTemperatures;
  int64_t stageDuration;
  double thermalDiffusivity;
  int64_t dimensions;
  double epsilon;
  while (std::getline(this->inputFile, line)) {
    temp.str(line);
    std::getline(temp, binaryName, ' ');
    textData = this->extractTextData(line);
    binaryData = this->extractBinaryData(binaryName);

    rows = binaryData.back(); binaryData.pop_back();
    columns = binaryData.back(); binaryData.pop_back();
    initialTemperatures = binaryData;
    epsilon = textData.back(); textData.pop_back();
    dimensions = textData.back(); textData.pop_back();
    thermalDiffusivity = textData.back(); textData.pop_back();
    stageDuration = textData.back(); textData.pop_back();
    this->plates.push_back(new Plate(line, rows, columns, initialTemperatures,
      stageDuration, thermalDiffusivity, dimensions, epsilon));
  }
}

std::vector<double> HeatSimulator::extractTextData(
  std::string jobForAnalyze) {
  const std::string delimiter = " ";
  std::string token;
  double number;
  size_t index = 0;
  std::vector<double> textData;
  bool condition = true;
  jobForAnalyze += delimiter;
  int64_t count = 0;
  index = jobForAnalyze.find(delimiter);
  while (index != std::string::npos) {
    number = 0;
    token = jobForAnalyze.substr(0, index);
    if (count == 0) {
      ++count;
    } else {
      try {
        number = std::stod(token);
      } catch(...) {
        condition = false;
      }
      if (condition != false) {
        textData.push_back(number);
      }
      jobForAnalyze.erase(0, index + delimiter.length());
      index = jobForAnalyze.find(delimiter);
      condition = true;
      ++count;
    }
  }
  return textData;
}

std::string HeatSimulator::extractJobFolder() {
  std::stringstream temp;
  std::string jobFolder = "";
  for (int64_t index = 0; this->route[index]; ++index) {
    if (index > 4)
      jobFolder += this->route[index];
  }
  temp.str(jobFolder);
  std::getline(temp, jobFolder, '/');
  return jobFolder;
}

std::vector<double> HeatSimulator::extractBinaryData(
  std::string binaryFileName) {
  int64_t rows = 0;
  int64_t columns = 0;
  std::ifstream binaryFile;
  std::string route = "jobs/" + this->jobFolder + "/" + binaryFileName;
  double temperature = 0;
  std::vector<double> binaryData;
  binaryFile.open(route, std::ifstream::binary);
  binaryFile.read(reinterpret_cast<char*>(&rows), sizeof(rows));
  binaryFile.read(reinterpret_cast<char*>(&columns), sizeof(columns));
  for (int64_t index = 0; index < rows; ++index) {
    for (int64_t jindex = 0; jindex < columns; ++jindex) {
       binaryFile.read(reinterpret_cast<char*>(&temperature),
        sizeof(temperature));
       binaryData.push_back(temperature);
    }
  }
  binaryData.push_back(columns);
  binaryData.push_back(rows);
  binaryFile.close();
  return binaryData;
}

void HeatSimulator::createReportFile() {
  std::string report;
  std::string fileName = this->jobFolder + ".tsv";
  std::string route = "jobs/" + jobFolder + "/output/" + fileName;
  this->reportFile.open(route, std::ofstream::out);
  for (size_t index = 0; index < this->plates.size(); ++index) {
    report = this->plates[index]->getReport();
    this->reportFile << report;
    if (index < this->plates.size())
      this->reportFile << std::endl;
  }
  this->reportFile.close();
}

void HeatSimulator::createPlateFile(Plate* plate) {
  std::ofstream binaryFile;
  std::string route;
  std::string fileName;
  std::stringstream plateName;
  std::string state = std::to_string(plate->getState());
  double temperature = 0;
  int64_t rows = plate->getRows();
  int64_t columns = plate->getColumns();
  std::vector<std::vector<double>> matrix = plate->getTemperaturesMatrix();

  plateName.str(plate->getName());
  std::getline(plateName, fileName, ' ');
  plateName.str(fileName);
  std::getline(plateName, fileName, '.');
  fileName += "-" + state + ".bin";
  route = "jobs/" + this->jobFolder + "/output/" + fileName;
  binaryFile.open(route, std::ifstream::binary);
  binaryFile.write(reinterpret_cast<char*>(&rows), sizeof(rows));
  binaryFile.write(reinterpret_cast<char*>(&columns), sizeof(columns));
  for (int64_t index = 0; index < rows; ++index) {
    for (int64_t jindex = 0; jindex < columns; ++jindex) {
      temperature = matrix[index][jindex];
      binaryFile.write(reinterpret_cast<char*>(&temperature),
        sizeof(temperature));
    }
  }
  binaryFile.close();
}
