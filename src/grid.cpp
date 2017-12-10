#include "grid.h"

Grid::Grid() {
	size_grid = 0;
	grid = new double[size_grid];
}
Grid::Grid(int s) {
	size_grid = s;
	grid = new double[size_grid];
}
Grid::~Grid() { delete[] grid; }
Grid::Grid(const Grid& g) {
	size_grid = g.size_grid;
	grid = new double[size_grid];
	for (int i = 0; i < size_grid; ++i) {
		grid[i] = g.grid[i];
	}
}

int Grid::get_size_grid() const { return size_grid; }

double* Grid::get_grid() {
	double* res = new double[size_grid];
	for (int i = 0; i < size_grid; ++i) {
		res[i] = grid[i];
	}
	return res;
}

double* Grid::get_grid() const {
	double* res = new double[size_grid];
	for (int i = 0; i < size_grid; ++i) {
		res[i] = grid[i];
	}
	return res;
}

void Grid::print() const {
	for (int i = 0; i < size_grid; ++i) {
		printf("%f\n", grid[i]);
	}
}

double& Grid::operator[](int i) { return grid[i]; }

const double& Grid::operator[](int i) const { return grid[i]; }

Grid Grid::operator=(const Grid& g) {
	size_grid = g.size_grid;
	grid = new double[size_grid];
	for (int i = 0; i < size_grid; ++i) {
		grid[i] = g.grid[i];
	}
	return *this;
}

const double &Grid::at(int i) const {
	if ((i < 0) || (i > size_grid - 1)) {
		printf("Index of grid is out of range.\n");
		exit(1);
	}
	return grid[i];
}

double &Grid::at(int i) {
	if ((i < 0) || (i > size_grid - 1)) {
		printf("Index of grid is out of range.\n");
		exit(1);
	}
	return grid[i];
}



void Grid::init_log_0_xf(double x0, double xf) {
	if (size_grid == 0) {
		printf("Grid is empty. Can not init log grid.\n");
		exit(1);
	}
	for (int i = 0; i < size_grid; ++i) {
		grid[i] = exp(log(x0) + (log(xf) - log(x0)) 
						/ double(size_grid - 1) * double(i));
	}
}

void Grid::init_unif_x0_xf(double x0, double xf) {
	if (size_grid == 0) {
		printf("Grid is empty. Can not init log grid.\n");
		exit(1);
	}
	for (int i = 0; i < size_grid; ++i) {
		grid[i] = x0 + (xf - x0) / double(size_grid - 1) * double(i);
	}
}

void Grid::init_log_mxf_xf(double x0, double xf) {
	if (size_grid == 0) {
		printf("Grid is empty. Can not init log grid.\n");
		exit(1);
	}
	for (int i = (size_grid - 1) / 2 + 1; i < size_grid; ++i) {
		grid[i] = exp(log(x0) + (log(xf) - log(x0)) 
					  / (double(size_grid - 1)) 
					  * double(i));
		grid[(size_grid - 1) - i] = -exp(log(x0) + (log(xf) - log(x0)) 
											/ (double(size_grid - 1)) 
											* double(i));
	}
}

void init_with_spacing_x0_xf(double x0, double xf, double s){

}


double Grid::h(int i) const { return grid[i] - grid[i - 1]; }

double Grid::h_c(int i) const { return (h(i) + h(i + 1)) / 2.; }

double Grid::inv_hch(int i) const { return 1. / (h_c(i) * h(i)); }
double Grid::inv_hch1(int i) const { return 1. / (h_c(i) * h(i + 1)); }

double Grid::inv_2h(int i) const { return 1. / (2. * h(i)); }


Grid join(const Grid &g1, const Grid &g2){
	int resulting_size = g1.get_size_grid() + g2.get_size_grid();
	Grid joined_grid(resulting_size);
	for(int i = 0; i < g1.get_size_grid(); ++i){
		joined_grid.at(i) = g1.at(i);
	}
	for(int i = 0; i < g2.get_size_grid(); ++i){
		int ii = g1.get_size_grid() + i;
		joined_grid.at(ii) = g2.at(i);
	}

	return joined_grid;
}
