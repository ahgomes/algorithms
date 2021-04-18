/*******************************************************************************
 * Name    : sqrt.cpp
 * Author  : Adrian Gomes
 * Version : 1.0
 * Date    : February 8, 2021
 * Description : Computes the square root of a double using Newton's method.
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <limits>

using namespace std;

double sqrt(double num, double epsilon) {
    if (num < 0) return numeric_limits<double>::quiet_NaN();
    if (num == 0 || num == 1) return num;

    double last_guess, next_guess = num;

    do {
        last_guess = next_guess;
        next_guess = (last_guess + num/last_guess) / 2;
    } while (abs(last_guess - next_guess) > epsilon);

    return next_guess;
}

int main(int argc, char *argv[]) {
    double value, epsilon;
    istringstream iss;

    if (argc != 2 && argc != 3) {
        cerr << "Usage: " << argv[0] << " <value> [epsilon]" << endl;
        return 1;
    }

    iss.str(argv[1]);
    if (!(iss >> value)) {
        cerr << "Error: Value argument must be a double." << endl;
        return 1;
    }

    if (argc == 3) {
        iss.clear();
        iss.str(argv[2]);
        if (!(iss >> epsilon) || epsilon <= 0) {
            cerr << "Error: Epsilon argument must be a positive double." << endl;
            return 1;
        }
    } else epsilon = 1e-7;

    cout << fixed << setprecision(8) << sqrt(value, epsilon) << endl;
    return 0;
}
