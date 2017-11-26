#pragma once

#include <iostream>
#include <ctime>
#include <vector>

#include <omp.h>

#include "additional.h"
#include "selection_sort.h"
#include "selection_sort_mpi.h"

using namespace std;

template<typename F> double cacl(F function, double* a, int c);
Stat do_calculate_selection_sort_openmp(void (*f) (double*, int), double* array, int count, int num_threads);
Stat do_calculate_selection_sort_mpi(void (*f) (double*, int), double* array, int count, int num_threads);
vector<Stat> calculate_selection_sort(Setting setting, Type type);