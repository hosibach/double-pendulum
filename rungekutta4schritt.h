#pragma once

#include <utility>

template<class F>
auto Runge_Kutta4Schritt(
		double x1, 
		double x2, 
		double p1,
		double p2,	
		double dt, 
		F&& func1,
		F&& func2)
	-> std::tuple<double, double, double, double>
{
	// Runge-Kutta-Parameter für Rechenschritte
		double k1[4], k2[4], k3[4], k4[4];

		k1[0] = p1;
		k2[0] = p1 + 0.5 * dt * k1[0];
		k3[0] = p1 + 0.5 * dt * k2[0];
		k4[0] = p1 + 0.5 * dt * k3[0];

		k1[1] = p2;
		k2[1] = p2 + 0.5 * dt * k1[1];
		k3[1] = p2 + 0.5 * dt * k2[1];
		k4[1] = p2 + 0.5 * dt * k3[1];

		k1[2] = func1(x1,x2,p1,p2);
		k2[2] = func1(x1 + 0.5 * dt * k1[2],x2 + 0.5 * dt * k1[2],p1 + 0.5 * dt * k1[2],p2 + 0.5 * dt * k1[2]);
		k3[2] = func1(x1 + 0.5 * dt * k2[2],x2 + 0.5 * dt * k2[2],p1 + 0.5 * dt * k2[2],p2 + 0.5 * dt * k2[2]);
		k4[2] = func1(x1 + 0.5 * dt * k3[2],x2 + 0.5 * dt * k3[2],p1 + 0.5 * dt * k3[2],p2 + 0.5 * dt * k3[2]);

		k1[3] = func2(x1,x2,p1,p2);
		k2[3] = func2(x1 + 0.5 * dt * k1[3],x2 + 0.5 * dt * k1[3],p1 + 0.5 * dt * k1[3],p2 + 0.5 * dt * k1[3]);
		k3[3] = func2(x1 + 0.5 * dt * k2[3],x2 + 0.5 * dt * k2[3],p1 + 0.5 * dt * k2[3],p2 + 0.5 * dt * k2[3]);
		k4[3] = func2(x1 + 0.5 * dt * k3[3],x2 + 0.5 * dt * k3[3],p1 + 0.5 * dt * k3[3],p2 + 0.5 * dt * k3[3]);

		x1 += dt * (1/6.f) * (k1[0] + 2 * k2[0] + 2 * k3[0] + k4[0]);
		x2 += dt * (1/6.f) * (k1[1] + 2 * k2[1] + 2 * k3[1] + k4[1]);
		p1 += dt * (1/6.f) * (k1[2] + 2 * k2[2] + 2 * k3[2] + k4[2]);
		p2 += dt * (1/6.f) * (k1[3] + 2 * k2[3] + 2 * k3[3] + k4[3]);

		return std::tuple{x1, x2, p1, p2};

}