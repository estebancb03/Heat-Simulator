/// @copyright 2021 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Esteban Castañeda Blanco <esteban.castaneda@ucr.ac.cr>
/// @author Daniel Lizano Morales <daniel.lizanomorales@ucr.ac.cr>
/// @author Andrea Ramírez Rojas <andrea.ramirezrojas@ucr.ac.cr>
/// @author Carlos Ramírez Masís <carlos.ramirezmasis@ucr.ac.cr>

#include "HeatSimulator.hpp"

int main(int argc, char* argv[]) {
  HeatSimulator* heatSimulator = new HeatSimulator();
  struct timespec start_time, finish_time;
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  heatSimulator->run(argc, argv);
  clock_gettime(CLOCK_MONOTONIC, &finish_time);
  double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
    (finish_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;
  printf("\nElapsed time: %.9lfs\n", elapsed_time);
  delete heatSimulator;
  return EXIT_SUCCESS;
}
