#pragma once

#include <sstream>
#include <unistd.h>
#include <map>
#include <vector>
#include <utility>
#include <cmath>
#include <numeric>

#include "additional.h"

#include <g2.h>
#include <g2_X11.h>

using namespace std;

struct Color
{
	int r;
	int g;
	int b;
};

void draw_graphic(int dev, int height, int width, map<int, vector<Stat>> grouped, int index);
void draw_stats(int dev, int height, int width, map<int, vector<Stat>> grouped, int index);
void draw(int height, int width, vector<Stat> stats);
void set_color_by_value(int dev, int r, int g, int b);
void set_color(int dev, Color color);
void set_random_color(int dev);