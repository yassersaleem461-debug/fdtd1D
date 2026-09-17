#include <algorithm>
#include <fstream>
#include <cassert>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cmath>

constexpr double permitivity_freespace = 8.8541878128e-12; //m^-3 kg^-1 s^4 A^2
constexpr double permiability_freespace = 1.25663706212e-6; // m kg s^-2 A^-2
constexpr double light_speed = 299792458 ; // m/2
constexpr double courant_number = 0.99; 
using namespace std;
vector<double> GenerateDiscreteGrid(double min,double max, int N)
{
	vector<double> Grid(N);
	double delta = (max-min)/(N-1); // minus 1 to include the end point
	for (int i=0;i<N;i++)
	{
		Grid[i] = min+delta*i;
	}
	return Grid;

}
double GaussianSource(double time, double amplitude, double width, double center)
{
	const double u = (time-center)/width;
	return amplitude*exp(-0.5*pow(u,2.0));
}
//To be written
/*vector<double> StepWisePermitivitySpace(int Nx_grid, double x_I)
{
	double x_I=24;
	vector<double> epsilon;
	for (int i=0;i<Nx_grid;i++)
	{
		if()
		epsilon=1;
	}
}*/
void WriteRealSpaceGrid(const string& S,const vector<double>& Grid )
{
	ofstream Write;
	Write.open(S);
	for(int i=0;i<Grid.size();i++)
		Write << Grid[i]<<endl;
}

int main()
{
	const int Nx = 100;
	const double x_I=24e-6; // micrometers interface point
	const double n_dielectric=1.5;; //dielectric refractive index
	const double x_Min =0; // Defines computational domain
	const double x_Max=40e-6; // Defines computational domain
	double t_max=200e-15; // max time in seconds.
	const double dx = (x_Max-x_Min)/(Nx-1);
	const double dt = courant_number*dx/light_speed; // from the so called Courant stability  (or CFL Courant–Friedrichs–Lewy condition)
	const int Nt = ceil(t_max/dt);
	const double source_amplitude = 1.0;
	const double source_width = 8e-15;
	const double source_centre = 6.0 * source_width;
	const int source_index=0; // for next time write function that determines source index from x_I, x_Max, and dx;
	cout << "Number of Grid points in x = "<<Nx <<endl;
	cout << "Number of Grid points in t = "<<Nt <<endl;
	vector<double> Ez(Nx, 0.0); //  for x, and t. // we need to define this at t=0 first
	vector<double> Hy(Nx-1,0.0); //  for x, and t.
	vector<vector<double>> FullE(Nx,Nt);
	//vector<double> epsilon(Nx, permitivity_freespace);
	//vector<double> mu(Nx-1,permiability_freespace); 
	vector<double> SpaceGrid = GenerateDiscreteGrid(x_Min,x_Max,Nx); 
	WriteRealSpaceGrid("RealspaceGrid.txt",SpaceGrid );
	ofstream Write;
	double mu = permiability_freespace;
	double epsilon = permitivity_freespace; // must be intialized
	for(int i=0;i<Nx;i++)
		FullE[i][0]=Ez[i]; // save t=0 space field, is there a better way to do this?
	

	for(int n=0;n<Nt;n++)
	{
		
		for(int i=0;i<Nx-1;i++)
		{
			if(i*dx<x_I)
			epsilon = permitivity_freespace; // for now this is fine.
			else
			epsilon = permitivity_freespace*pow(1.5,2.0);
			Hy[i] = Hy[i] + (dt/(dx*mu))*(Ez[i+1] - Ez[i]);

		}
		for(int i=1;i<Nx;i++)
		{
			if(i*dx<x_I)
			epsilon = permitivity_freespace; // for now this is fine.
			else
			epsilon = permitivity_freespace*pow(1.5,2.0);
			Ez[i] = Ez[i] + (dt/(dx*epsilon))*(Hy[i+1] - Hy[i]);

		}
	}





}