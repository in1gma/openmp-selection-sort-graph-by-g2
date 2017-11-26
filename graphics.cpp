#include "graphics.h"

Color BACKGROUND = { 249, 205, 142 };
Color BLACK = { 0, 0, 0 };
Color RED = { 255, 12, 73 };
Color PINK = { 249, 195, 212 };

void set_color_by_value(int dev, int r, int g, int b)
{
	int const base = 255;
	int color = g2_ink(dev, r/(double) base, g/(double) base, b/(double) base);
	g2_pen(dev, color);
}

void set_color(int dev, Color color)
{
	set_color_by_value(dev, color.r, color.g, color.b);
}

void set_random_color(int dev)
{
	double c1 = static_cast<double>(rand() % 255);
	double c2 = static_cast<double>(rand() % 255);
	double c3 = static_cast<double>(rand() % 255);
	set_color_by_value(dev, c1, c2, c3);
}

void draw_text(int dev, int x, int y, string s)
{
	g2_string(dev, x, y, s.c_str());
}

void draw_string(int dev, int x, int y, string str)
{
	draw_text(dev, x, y, str.c_str());
}

template <typename F> void draw_number(int dev, int x, int y, F value)
{
	draw_text(dev, x, y, to_string(value));
}

template <typename F> void draw_number_format(int dev, int x, int y, F value)
{
	char* c = new char[to_string(value).length()];

	sprintf(c, "%.3f", value);

	draw_text(dev, x, y, c);
}

void draw_graphic(int dev, int real_height, int real_width, map<int, vector<Stat>> grouped, int index)
{
	int height = real_height - 0.15*real_height;
	int width = real_width;
	
	/*FILL BACKGROUND*/
	set_color(dev, BACKGROUND);
	g2_filled_rectangle(dev, 0, 0, width, height);

	/*DRAW AXIS*/
	set_color(dev, BLACK);
	int x0 = 45, y0 = 30, axis_thickness = 1;
	int axis_width = width - 2*x0;
	int axis_height = height - 2*y0;

	g2_filled_rectangle(dev, x0, y0, x0 + axis_thickness, height - y0);
	g2_filled_rectangle(dev, x0, y0, width - x0, y0 + axis_thickness);

	draw_string(dev, width - x0 + 5, y0 - axis_thickness, "threads");
	draw_string(dev, x0 - axis_thickness - 7, height - y0 + 10, "time");

	/*DRAW COORDINATES*/
	int interval_count_x = 4; 
	int interval_count_y = 10;
	double interval_x = axis_width/static_cast<double> (interval_count_x);
	double interval_y = axis_height/static_cast<double> (interval_count_y);
	draw_number(dev, x0, y0 - 15, 0);

	/*SELECT VALUES*/
	pair<int, double> max = find_max_from_stat_map(grouped, index); 
	double max_x = max.first; 
	double min_x = max_x/static_cast<double> (interval_count_x);
	double max_y = max.second; 
	double min_y = max_y/static_cast<double> (interval_count_y);

	/*DRAW BORDER*/
	set_color(dev, PINK);
	g2_filled_rectangle(dev, width - 5*x0, height - 3*y0, width - x0/5, height - x0/5);
	set_color(dev, BACKGROUND);
	g2_filled_rectangle(dev, width - 5*x0 + 5, height - 3*y0 + 5, width - x0/5 - 5, height - x0/5 - 5);

	/*DRAW INFO*/
	set_color(dev, BLACK);
	stringstream ss;
	ss << "MAX X = " << max_x << "; MIN X = " << min_x;
	draw_string(dev, width - 5*x0 + 10, height - y0 + 5, ss.str());

	ss.clear();
	ss.str(string());
	ss << "MAX Y = " << max_y << "; MIN Y = " << min_y;
	draw_string(dev, width - 5*x0 + 10, height - y0 - 10, ss.str());

	ss.clear();
	ss.str(string());
	ss << "COUNT = " << grouped.begin()->second[index].count;
	draw_string(dev, width - 5*x0 + 10, height - y0 - 25, ss.str());

	/*DRAW*/

	double small_axis_heigth = 20;

	for (int i = 1; i <= interval_count_x; ++i)
	{
		double x = i*interval_x + x0;
		draw_number_format(dev, x, y0 - 25, min_x*i);
		g2_filled_rectangle(dev, x, y0 - small_axis_heigth/2 + axis_thickness/2, x + axis_thickness, y0 + small_axis_heigth/2 + axis_thickness/2);
	}

	for (int i = 1; i <= interval_count_y; ++i)
	{
		double y = i*interval_y + y0;
		draw_number_format(dev, x0 - 40, y, min_y*i);
		g2_filled_rectangle(dev, x0 - small_axis_heigth/2 + axis_thickness/2, y, x0 + small_axis_heigth/2 + axis_thickness/2, y + axis_thickness);
	}

	/*DRAW POINTS*/
	set_color(dev, RED);
	double tmp_x = NULL, tmp_y = NULL;
	for (auto i = grouped.begin(); i != grouped.end(); ++i)
	{
		double y = axis_height*(1 - (max_y - i->second[index].time)/max_y) + y0;
		double x = axis_width*(1 - (max_x - i->first)/max_x) + x0;
		g2_filled_circle(dev, x, y, 3);

	 	if (tmp_x != NULL && tmp_y != NULL) g2_line(dev, x, y, tmp_x, tmp_y);
		tmp_x = x;
		tmp_y = y;
	}
}

void draw_stats(int dev, int height, int width, map<int, vector<Stat>> grouped, int index)
{
	/*SPEED-COEFF*/
	vector<double> coeffs;

	for (auto i = grouped.begin(); i != grouped.end(); ++i)
	{
		coeffs.push_back(grouped.begin()->second[index].time/i->second[index].time);
	}

	/*SUM*/
	double tmp_sum = 0; 
	for (auto i = coeffs.begin(); i != coeffs.end(); ++i)
	{
		tmp_sum += *i;
	}

	/*RECT-VALUES OF SPEED*/
	double tmp_last_x = 0;
	int tmp_k = 0;
	for (auto i = coeffs.begin(); i != coeffs.end(); ++i)
	{	
		set_random_color(dev);

		double current_x = width*(1 - (tmp_sum - (*i))/tmp_sum) + tmp_last_x;
		g2_filled_rectangle(dev, tmp_last_x, height - 0.15*height, current_x, height);
		
		set_color(dev, BLACK);
		auto cur = grouped.begin();
		advance(cur, tmp_k);

		draw_string(dev, tmp_last_x + 10, height - 0.25*height/6, "THREADS = ");
		draw_number(dev, tmp_last_x + 80, height - 0.25*height/6, cur->second[index].number_of_threads);

		draw_string(dev, tmp_last_x + 10, height - 0.25*height/6 - 15, "SPEEDUP = ");
		draw_number_format(dev, tmp_last_x + 75, height - 0.25*height/6 - 15, *i);

		draw_string(dev, tmp_last_x + 10, height - 0.25*height/6 - 30, "TIME = ");
		draw_number_format(dev, tmp_last_x + 55, height - 0.25*height/6 - 30, cur->second[index].time);

		draw_string(dev, tmp_last_x + 10, height - 0.25*height/6 - 45, "SPEED = ");
		draw_number_format(dev, tmp_last_x + 65, height - 0.25*height/6 - 45, cur->second[index].count/cur->second[index].time);

		draw_string(dev, tmp_last_x + 10, height - 0.25*height/6 - 60, "EFFECT = ");
		draw_number_format(dev, tmp_last_x + 70, height - 0.25*height/6 - 60, (*i)/cur->second[index].number_of_threads);

		tmp_last_x = current_x;
		tmp_k++;
	}
}

void draw(int height, int width, vector<Stat> stats)
{
	/*SELECT*/
	map<int, vector<Stat>> grouped;
	for (auto i = stats.begin(); i != stats.end(); ++i)
	{
		grouped[(*i).number_of_threads].push_back(*i);
	}

	map<int, vector<Stat>> grouped2;
	for (auto i = stats.begin(); i != stats.end(); ++i)
	{
		grouped2[(*i).count].push_back(*i);
	}

	/*DRAW*/
	int* devs = new int[grouped2.size()];
	for (int i = 0; i < grouped2.size(); ++i)
	{
		// int dev = g2_open_X11(width, height);
		// draw_graphic(dev, height, width, grouped, i); //!

		// devs[i] = dev;
	}

	int* devs2 = new int[grouped2.size()];
	for (int i = 0; i < grouped2.size(); ++i)
	{
		int dev = g2_open_X11(width, height);
		draw_graphic(dev, height, width, grouped, i); //!
		draw_stats(dev, height, width, grouped, i);

		devs2[i] = dev;
	}

	pause();
	// g2_close(dev);
}