#ifndef __TA3D_GRID_HXX__
#define __TA3D_GRID_HXX__

#include "grid.h"

template<class T>
Grid<T>::Grid() : w(0), h(0), data(NULL)
{
	resize(1, 1);
}

template<class T>
Grid<T>::Grid(int w, int h) : w(0), h(0), data(NULL)
{
	resize(w, h);
}

template<class T>
void Grid<T>::resize(int w, int h)
{
	if (data)
		delete[] data;
	this->w = w;
	this->h = h;
	data = new T[w * h];
}

template<class T>
void Grid<T>::clear()
{
	if (data)
		memset(data, 0, w * h * sizeof(T));
}

template<class T>
Grid<T>::~Grid()
{
	if (data)
		delete[] data;
}

template<class T>
const T& Grid<T>::operator()(int x, int y) const
{
	return data[x + y * w];
}

template<class T>
T& Grid<T>::operator()(int x, int y)
{
	return data[x + y * w];
}

template<class T>
void Grid<T>::add(const Grid<T> &grid, int x, int y)
{
	for(int j = 0 ; j < grid.getHeight() && j + y < h ; ++j)
		if (j + y >= 0)
			for(int i = 0 ; i < grid.getWidth() && i + x < w ; ++i)
				if (i + x >= 0)
					(*this)(i + x, j + y) += grid(i, j);
}

template<class T>
void Grid<T>::sub(const Grid<T> &grid, int x, int y)
{
	for(int j = 0 ; j < grid.getHeight() && j + y < h ; ++j)
		if (j + y >= 0)
			for(int i = 0 ; i < grid.getWidth() && i + x < w ; ++i)
				if (i + x >= 0)
					(*this)(i + x, j + y) -= grid(i, j);
}

#endif
