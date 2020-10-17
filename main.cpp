#include <string>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include "rungekutta4schritt.h"

double g = 9.81;
double l1 = 1;
double l2 = 1;
double m1 = 1;
double m2 = 1;
int n, tmax;
double dt;
std::string file;


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

	file_stream << "#Werte:" << std::endl
		<< "#Zeitschritt dt:" << '\t' << dt << std::endl
		<< "#Maximale Zeit tmax:" << '\t' << tmax << std::endl
		<< "#Samplegroesse:" << '\t' << n << std::endl
		<< "#Ausgabedatei:" << '\t' << file << std::endl
		<< "#Zeit t\tWinkel1 in rad\tWinkel2 in rad\tWinkelgeschw1 in rad/s\tWinkelgeschw2 in rad/s\tKartesische Positionen x1\ty1\tx2\ty2\n";

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

	double x10 = M_PI_2;
	double x20 = M_PI_2;
	double p10 = 0;
	double p20 = 0;
	double t = 0;


	std::cout << "Defaultgroessen (dt=1e-5,tmax=15,n=10000) ändern? [j/n] ";

	char s;
	std::cin >> s;

	switch(s){

		case 'n':
			dt=1e-5;
			tmax=15;
			n=10000;
			break;

		case 'j':
				std::cout << "Schrittgroesse(z.B. 1e-5): ";
				std::cin >> dt;

				std::cout << "Betrachtungszeit tmax(z.B. 15): ";
				std::cin >> tmax;

				std::cout << "Samplegroesse n (z.B. 10000 >= tmax): ";
				std::cin >> n;
				break;

		default:
			dt=1e-5;
			tmax=15;
			n=10000;
			break;
	}
	

	int i = 1;
 	file = std::string("./Daten/data")+std::to_string(i)+std::string(".dat");

 	while (fexists(file))
 	{
 		i+=1;
 		file = std::string("./Daten/data")+std::to_string(i)+std::string(".dat");
 	}

	// Gebe benutzte Werte in Terminal aus

	std::cout << "Werte:" << std::endl
		<< "Zeitschritt dt:" << '\t' << dt << std::endl
		<< "Maximale Zeit tmax:" << '\t' << tmax << std::endl
		<< "Samplegroesse:" << '\t' << n << std::endl
		<< "Ausgabedatei:" << '\t' << file << std::endl;

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

		// Statusanzeige --> Flush zu langsam!
		//std::cout << "\33[2K\r" << int(t/tmax*100) << '%' << '\t' << '[' << std::string(int(t/tmax*100), '#') << std::string(100-int(t/tmax*100), ' ') << ']'  << std::flush;
		//}
	}

	std::cout << "Done calculating, writing to file..." << std::endl;

	file_write(file, results, n);

	std::cout << "Done." << std::endl;

	return 0;
}