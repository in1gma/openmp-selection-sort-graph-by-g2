#include "selection_sort.h"

void selection_sort(double* array, int count)
{
    for (size_t idx_i = 0; idx_i < count - 1; idx_i++)
    {
        size_t min_idx = idx_i;
		double m;
		#pragma omp prallel for reduction(min: m)
        for (size_t idx_j = idx_i + 1; idx_j < count; idx_j++)
        {
            if (array[idx_j] < m)
            {
                min_idx = idx_j;
				m = array[min_idx];
            }
        }
	
        if (min_idx != idx_i)
        {
            swap(array[idx_i], array[min_idx]);
        }
    }
}