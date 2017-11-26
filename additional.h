#pragma once

#include <iostream>
#include <ctime>
#include <vector>
#include <map>
#include <utility>

using namespace std;

struct Stat
{
	int number_of_threads;
	int count;
	double time;
};

struct Setting
{
	double* array;
	int array_count;
	int* num_threads;
	int num_threads_count;
};

enum Type {
	OPENMP,
	MPI
};

double* array_create(int count, int min, int max);
void array_print(double* array, int count);
double* duplicate_array(double* array, int count);
pair<int, double> find_max_from_stat_map(map<int, vector<Stat>> m, int index);