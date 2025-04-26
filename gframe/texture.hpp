#pragma once
#include <stddef.h>

struct Pixel3
{
	char r, g, b;
};
struct Pixel4
{
	char r, g, b, a;
};

class Texture
{
private:
	union Data
	{
		void* raw = NULL;
		Pixel3* ch3;
		Pixel4* ch4;
	};
public:
	int width, height, nrChannels;
	Data data;
};