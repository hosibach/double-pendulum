Simulation of a double pendulum using a Runge-Kutta-Algorithm of 4th order
See [equation.pdf](equation.pdf) for functions for angle acceleration of both angles.

Run `make all` to compile and execute the program, run `python animation.py <name of outfile>` for the animation.

You can change the masses `m1, m2` and the lengths `l1, l2` as well as both starting angles `x10, x20`  and velocities `p10, p20` of the double pendulum by changing the respective values.
The same applies to the set time step `dt` used by the Runge-Kutta-Algorithm, the time span `tmax` and the number of samples `n` written to the outfile. Notice that changing `n` doesn't affect the error of the method, only changing `dt` does. That allows you to change the size of your outfile to your liking without ruining the algorithm.
