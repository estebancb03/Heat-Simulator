/// @copyright 2021 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Esteban Castañeda Blanco <esteban.castaneda@ucr.ac.cr>
/// @author Daniel Lizano Morales <daniel.lizanomorales@ucr.ac.cr>
/// @author Andrea Ramírez Rojindexas <andrea.ramirezrojas@ucr.ac.cr>
/// @author Carlos Ramírez Masís <carlos.ramirezmasis@ucr.ac.cr>

#include <fstream>
#include <iostream>
#include <math.h>
#include <string.h>

int main(int argc, char* argv[]) {
  std::string plate1;
  std::string plate2;
  double epsilon;

  if (argc >= 4) {
    plate1 = argv[1];
  }
  if (argc >= 3) {
    plate2 = argv[2];
  }
  if (argc >= 2) {
    epsilon = std::stod(argv[3]);
  }
  if (argc < 2) {
    std::cout << "\nWrong argument count" << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream binaryFile1;
  std::ifstream binaryFile2;
  int64_t rows1 = 0;
  int64_t columns1 = 0;
  int64_t rows2 = 0;
  int64_t columns2 = 0;
  double temperature1 = 0;
  double temperature2 = 0;
  bool areTheSame = true;

  binaryFile1.open(plate1, std::ifstream::binary);
  binaryFile1.read(reinterpret_cast<char*>(&rows1), sizeof(rows1));
  binaryFile1.read(reinterpret_cast<char*>(&columns1), sizeof(columns1));

  binaryFile2.open(plate2, std::ifstream::binary);
  binaryFile2.read(reinterpret_cast<char*>(&rows2), sizeof(rows2));
  binaryFile2.read(reinterpret_cast<char*>(&columns2), sizeof(columns2));

  if ((rows1 == rows2) && (columns1 == columns2)) {
    std::cout << "The dimensions are the same\n";
  } else {
    std::cout << "The dimensions are different\n";
    return EXIT_FAILURE;
  }

  for (int64_t index = 0; index < rows1; ++index) {
    for (int64_t jindex = 0; jindex < columns1; ++jindex) {
        binaryFile1.read(reinterpret_cast<char*>(&temperature1),
          sizeof(temperature1));
        binaryFile2.read(reinterpret_cast<char*>(&temperature2),
          sizeof(temperature2));
        if (fabs(temperature1-temperature2) > epsilon) {
          areTheSame = false;
          break;
        }
    }
  }
  if (areTheSame) {
    std::cout << "The temperatures in both plates are the same\n";
  } else {
    std::cout << "The temperatures in both plates are different\n";
  }
  return EXIT_SUCCESS;
}
