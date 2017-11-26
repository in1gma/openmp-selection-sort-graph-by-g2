#include "main.h"

using namespace std;

int main(int argc, char const *argv[])
{
	int max = 1000, min = -1000;
	/*-------------------------------*/

	int num_array_count = 5;
	int* num_array = new int[num_array_count] { 10000, 20000, 50000, 100000, 200000 };

	vector<Stat> stats;
	vector<Stat> stats2;

	for (int i = 0; i < num_array_count; ++i)
	{
		int count = num_array[i];

		int num_threads_count = 5;
		int* num_threads = new int[num_threads_count] { 1, 2, 3, 4, 8 };

		Setting setting {
			array_create(count, min, max),
			count,
			num_threads,
			num_threads_count
		};

		vector<Stat> s = calculate_selection_sort(setting, OPENMP);
		stats.insert(stats.end(), s.begin(), s.end());		
	}

	/*-------------------------------*/
	int height = 600;
	int width = 1000;

	draw(height, width, stats);
	/*-------------------------------*/

	return 0;
}