#include <string>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include "rungekutta4schritt.h"

double g = 9.81;
double t = 0;
std::string file;

/*///////////
	Basic Values for time step dt, max time
	tmax and number of samples n written to the outfile
	Note that minimizing the time step dt doesnt affect the size of the produced outfile, only changing n does!
///////////*/

double dt=1e-6;
int	tmax=20;
int n=10000;

/*///////////
	Lengths and Masses of the double pendulum
///////////*/

double l1 = 1;
double l2 = 1;
double m1 = 2;
double m2 = 1;

double x10 = M_PI_2;
double x20 = 0;//M_PI_2;
double p10 = 0;
double p20 = 0;


double func1(double x1, double x2, double p1, double p2)
{
	double a = m2/(m1+m2);
	double b = l2/l1;
	return pow(1-a*pow(cos(x1-x2),2),-1)*(a*b*cos(x1-x2)*(pow(p1,2)*sin(x1-x2)+g/l2*sin(x2))-a*b*pow(p2,2)*sin(x1-x2)-g/l1*sin(x1));
}


double func2(double x1, double x2, double p1, double p2)
{
	double a = m2/(m1+m2);
	double b = l2/l1;
	return pow(1-a*pow(cos(x1-x2),2),-1)*(cos(x1-x2)*(a*pow(p2,2)*sin(x1-x2)+1/b*g/l1*sin(x1))+1/b*pow(p1,2)*sin(x1-x2)-g/l2*sin(x2));
}


bool fexists(const std::string& filename)
{
	std::ifstream file(filename);
  return !file.fail();
}


void file_write(const std::string& filename,
								const std::vector<std::tuple<double, double, double, double, double, double, double, double, double>> data,
								int n)
{
	std::ofstream file_stream(filename);

	file_stream << "#Values:" << std::endl
		<< "#Time step dt:" << '\t' << dt << std::endl
		<< "#Max Time tmax:" << '\t' << tmax << std::endl
		<< "#Samplesize:" << '\t' << n << std::endl
		<< "#Outfile:" << '\t' << file << std::endl
		<< "#l1: " << l1 << '\t' << "l2: " << l2 << '\t' << "m1: " << m1 << '\t' << "m2: " << m2 << std::endl
		<< "#Mass 1: " << '\t' << "starting angle: " << x10 << ", " << "starting vel: " << p10 << std::endl
		<< "#Mass 2: " << '\t' << "starting angle: " << x20 << ", " << "starting vel: " << p20 << std::endl
		<< "#Time t \t Angle1 in rad \t Angle2 in rad \t AngVel1 in rad/s \t AngVel2 in rad/s \t Cart Positions x1 \t y1 \t x2 \t y2 \n";

	int i = 0;

	int N = data.size() / n;

	for(auto[time, first, second, third, forth, fifth, sixth, seventh, eighth] : data){
		if(i++ % N != 0) continue;

		file_stream	
					<< time << '\t'
					<< first << '\t'
					<< second << '\t'
					<< third << '\t'
					<< forth << '\t'
					<< fifth << '\t'
					<< sixth << '\t'
					<< seventh << '\t'
					<< eighth << '\n';
	}
}



int main() {


	/*///////////
	Numbering for Outfiles, produces outfiles
	with increasing number 'data#.dat'
	///////////*/

	int i = 1;
 	file = std::string("./Data/data")+std::to_string(i)+std::string(".dat");

 	while (fexists(file))
 	{
 		i+=1;
 		file = std::string("./Data/data")+std::to_string(i)+std::string(".dat");
 	}



	/*///////////
	Verbose all values
	///////////*/

	std::cout << "Values:" << std::endl
		<< "Time step dt:" << '\t' << dt << " s" << std::endl
		<< "Max time tmax:" << '\t' << tmax << " s" << std::endl
		<< "Samplesize:" << '\t' << n << std::endl
		<< "Outfile:" << '\t' << file << std::endl
		<< "l1: " << l1 << " m" << ", " << "l2: " << l2 << " m" << '\t' << "m1: " << m1 << " kg" << ", " << "m2: " << m2 << " kg" << std::endl
		<< "Starting values:" << std::endl
		<< "Mass 1: " << '\t' << "starting angle: " << x10 << " rad" << ", " << "starting velocity: " << p10 << " rad/s" << std::endl
		<< "Mass 2: " << '\t' << "starting angle: " << x20 << " rad" << ", " << "starting velocity: " << p20 << " rad/s" << std::endl;

	std::cout << "Calculating..." << std::endl;

		// X und Y Position 

	double cartx1 = l1 * sin(x10);
	double carty1 = -l1 * cos(x10);
	double cartx2 = l1 * sin(x10) + l2 * sin(x20);
	double carty2 = -l1 * cos(x10) - l2 * cos(x20);

	std::vector<std::tuple<double,double,double,double,double,double,double,double,double>> results;
	results.emplace_back(t, x10, x20, p10, p20, cartx1, carty1, cartx2, carty2);

	results.reserve(static_cast<int>(tmax / dt));

	//for (int i = 0;t<=tmax; t+=dt, i++)
	for (;t<=tmax;t+=dt)
	{
		auto [_, x1, x2, p1, p2,a,b,c,d] = results.back();

		auto [new_x1, new_x2, new_p1, new_p2] = Runge_Kutta4Schritt(x1, x2, p1, p2, dt, func1, func2);

		cartx1 = l1 * sin(new_x1);
		carty1 = -l1 * cos(new_x1);
		cartx2 = l1 * sin(new_x1) + l2 * sin(new_x2);
		carty2 = -l1 * cos(new_x1) - l2 * cos(new_x2);

		results.emplace_back(t, new_x1, new_x2, new_p1, new_p2, cartx1, carty1, cartx2, carty2);
	}

	std::cout << "Done calculating, writing to file..." << std::endl;

	file_write(file, results, n);

	std::cout << "Done." << std::endl;

	return 0;
}