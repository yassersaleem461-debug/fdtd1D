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
vector<double> GenerateDiscreteGrid(double min,double max, int N,double x_s, int& sourceindex)
{
	vector<double> Grid(N);
	double delta = (max-min)/(N-1); // minus 1 to include the end point
	double d=1e300;
	sourceindex=-1;
	for (int i=0;i<N;i++)
	{
		Grid[i] = min+delta*i;
		if(abs(Grid[i]-x_s)<d)
		{
			d=abs(Grid[i]-x_s);
			sourceindex=i;
		}
	}
	return Grid;

}
double GaussianSource(double time, double amplitude, double width, double center)
{
	const double u = (time-center)/width;
	return amplitude*exp(-0.5*pow(u,2.0));
}
//To be written
vector<double> StepWisePermitivitySpace(vector<double> SpaceGrid, double x_I)
{
	vector<double> epsilon(SpaceGrid.size());
	for (size_t i=0;i<SpaceGrid.size();i++)
	{
		double x = SpaceGrid[i];
		if(x<x_I)
			epsilon[i] = permitivity_freespace; // for now this is fine.
		else
			epsilon[i] = permitivity_freespace*pow(1.5,2.0);
		
	}
	return epsilon;
}
template <typename T>
void WriteToFile(const string& S, const vector<T>& values)
{
	ofstream Write;
	Write.open(S);
	for(size_t i=0;i<values.size();i++)
		Write << values[i]<<endl;
}
template <typename T>
void WriteToFile(const string& S,const vector<vector<T>>& matrix)
{
	ofstream Write;
	Write.open(S);
	const size_t num_rows = matrix.size();
	const size_t num_cols = matrix.empty() ? 0 : matrix[0].size();
	for(size_t i=0;i<num_rows;i++)
	{
		for(size_t j=0;j<num_cols;j++)
			Write << matrix[i][j]<<" ";
		Write<<endl;
	}

}

int main()
{
	const int Nx = 400;
	const double x_I=24e-6; // micrometers interface point
	const double x_s=8e-6;
	const double x_Min =0; // Defines computational domain
	const double x_Max=40e-6; // Defines computational domain
	double t_max=200e-15; // max time in seconds.
	const double dx = (x_Max-x_Min)/(Nx-1);
	const double dt = courant_number*dx/light_speed; // from the so called Courant stability  (or CFL Courant–Friedrichs–Lewy condition)
	const int Nt = ceil(t_max/dt);
	const double source_amplitude = 1.0;
	const double source_width = 8e-15; 
	const double source_centre = 6.0 * source_width;
	int source_index=0; // for next time write function that determines source index from x_I, x_Max, and dx;
	cout << "Number of Grid points in x = "<<Nx <<endl;
	cout << "Number of Grid points in t = "<<Nt <<endl;
	vector<double> Ez(Nx, 0.0); //  for x, and t. // we need to define this at t=0 first
	vector<double> Hy(Nx-1,0.0); //  for x, and t.
	vector<vector<double>> FullE(Nt+1,vector<double>(Nx,0.0));
	vector<double> epsilon(Nx, permitivity_freespace);
	vector<double> mu(Nx-1,permiability_freespace); 
	
	// Intiializations 

	vector<double> SpaceGrid = GenerateDiscreteGrid(x_Min,x_Max,Nx,x_s,source_index); // Define the real space grid.
	cout << "Source Position in (micro meters) = "<< SpaceGrid[source_index]*1e6<<endl;
	// Set the permitivities and permiabilities
	epsilon = StepWisePermitivitySpace(SpaceGrid,  x_I);


	WriteToFile("RealspaceGrid.txt",SpaceGrid );
	WriteToFile("Epsilon_x.txt",epsilon);
	ofstream Write;

		FullE[0]=Ez; // save t=0 space field, is there a better way to do this?


		for(int n=0;n<Nt;n++)
		{
			double time = n*dt;
			for(size_t i=0;i<Nx-1;i++)
			{

				Hy[i] = Hy[i] + (dt/(dx*mu[i]))*(Ez[i+1] - Ez[i]);

			}
			for(size_t i=1;i<Nx-1;i++)
			{
				Ez[i] = Ez[i] + (dt/(dx*epsilon[i]))*(Hy[i] - Hy[i-1]);

			}
			//Apply Source
			Ez[source_index] += GaussianSource( time,  source_amplitude, source_width, source_centre);
			//Apply Boundary condition
			Ez.front()=0;
			Ez.back()=0;
			FullE[n+1] = Ez;
		}

		WriteToFile("FullEfield.txt",FullE);



	}