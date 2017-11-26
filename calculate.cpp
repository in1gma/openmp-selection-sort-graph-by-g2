#include "calculate.h"

template<typename F> double cacl(F function, double* a, int c)
{
	clock_t start, end;
	start = clock();

	function(a, c);

	end = clock();
	return (end - start) / static_cast<double> (CLOCKS_PER_SEC);
}

Stat do_calculate_selection_sort_openmp(void (*function) (double*, int), double* array, int count, int num_threads)
{
	omp_set_dynamic(0);
	omp_set_num_threads(num_threads);

	return {
		omp_get_max_threads(),
		count,
		cacl(function, duplicate_array(array, count), count)
	};
}

Stat do_calculate_selection_sort_mpi(void (*function) (double*, int), double* array, int count, int num_threads)
{

	return {
		0,
		count,
		cacl(function, duplicate_array(array, count), count)
	};
}

vector<Stat> calculate_selection_sort(Setting setting, Type type)
{
	void (*function_alg) (double*, int);
	Stat (*function_calc) (void (*) (double*, int), double*, int, int);

	switch(type)
	{
		case OPENMP:
		{
			function_alg = &selection_sort;
			function_calc = &do_calculate_selection_sort_openmp;
			break;
		}
		case MPI:
		{
			function_alg = &selection_sort_mpi;
			function_calc = &do_calculate_selection_sort_mpi;
			break;
		}
	}

	vector<Stat> stats(setting.num_threads_count);
	for (int i = 0; i < setting.num_threads_count; ++i)
	{
		stats[i] = function_calc(function_alg, setting.array, setting.array_count, setting.num_threads[i]);

		cout << "Number of threads: " << stats[i].number_of_threads << "; Elapsed time: " << stats[i].time << "; Count: " << stats[i].count << endl;
	}

	return stats;
}