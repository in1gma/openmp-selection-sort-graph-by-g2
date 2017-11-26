#include "additional.h"

double* array_create(int count, int min, int max)
{
	srand(time(nullptr));
	double* array = new double[count];

	for (int i = 0; i < count; ++i)
	{
		array[i] = static_cast<double>(rand() % (max - min) + min);
	}

	return array;
}

void array_print(double* array, int count)
{
	for (int i = 0; i < count; ++i)
	{
		cout << array[i] << "\t";
	}
	cout << endl;
}

double* duplicate_array(double* array, int count)
{
	double* new_array = new double[count];
	for (int i = 0; i < count; ++i)
	{
		new_array[i] = array[i];
	}
	return new_array;
}

pair<int, double> find_max_from_stat_map(map<int, vector<Stat>> m, int index)
{
	int max = 0;
	double max2 = 0;
	for (auto i = m.begin(); i != m.end(); ++i)
	{
		if (i->first > max) max = i->first;
		if (i->second[index].time > max2) max2 = i->second[index].time;
	}

	return make_pair(max, max2);
}