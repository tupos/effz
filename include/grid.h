#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <cmath>


class Grid {
	int size_grid;
	double *grid;

	public:
	Grid();
	Grid(int s);
	~Grid();
	Grid(const Grid &g);
	int get_size_grid() const;

	double *get_grid();
	double *get_grid() const;

	void print() const;

	double &operator[](int i);
	const double &operator[](int i) const;
	Grid operator=(const Grid &g);
	const double &at(int i) const;
	double &at(int i);

	void init_log_0_xf(double x0, double xf);
	void init_unif_x0_xf(double x0, double xf);
	void init_log_mxf_xf(double x0, double xf);
	void init_with_spacing_x0_xf(double x0, double xf, double s);

	double h(int i) const;
	double h_c(int i) const;
	double inv_hch(int i) const;
	double inv_hch1(int i) const;
	double inv_2h(int i) const;
};

Grid join(const Grid &g1, const Grid &g2);

#endif /* GRID_H */
