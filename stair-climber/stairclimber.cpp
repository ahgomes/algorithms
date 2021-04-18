/*******************************************************************************
 * Name    : stairclimber.cpp
 * Author  : Adrian Gomes
 * Date    : March 8, 2021
 * Description : Lists the number of ways to climb n stairs.
 * Pledge  : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

vector< vector<int> > get_ways(int num_stairs) {
    vector< vector<int> > ways;
    if (num_stairs <= 0) {
        ways.push_back(vector<int>());
    } else {
        for (int i = 1; i < 4; i++) {
            if (num_stairs >= i) {
                vector< vector<int> > result = get_ways(num_stairs - i);
                for (size_t j = 0, len = result.size(); j < len; j++) {
                    result[j].insert(result[j].begin(), i);
                }
                ways.insert(ways.end(), result.begin(), result.end());
            }
        }
    }
    return ways;
}

void display_ways(const vector< vector<int> > &ways) {
    size_t num_ways = ways.size();
    if (num_ways == 1) {
        cout << "1 way to climb 1 stair." << endl;
        cout << "1. [1]" << endl;
        return;
    }

    cout << num_ways << " ways to climb ";
    cout << ways[0].size() << " stairs." << endl;

    int space_width = 1,
        dnum = num_ways;
    while (dnum >= 10) {
        space_width++;
        dnum /= 10;
    }

    for (size_t i = 0; i < num_ways; i++) {
        if (space_width > 1) cout << setw(space_width);
        cout << i + 1 << ". [";
        for (size_t j = 0, len = ways[i].size(); j < len; j++) {
            cout << ways[i][j] << (j + 1 == len ? "]" : ", ");
        }
        cout << endl;
    }
}

int main(int argc, char * const argv[]) {
    int num_stairs;
    istringstream iss;

    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <number of stairs>" << endl;
        return 1;
    }

    iss.str(argv[1]);
    if (!(iss >> num_stairs) || num_stairs <= 0) {
        cerr << "Error: Number of stairs must be a positive integer." << endl;
        return 1;
    }

    vector< vector<int> > ways = get_ways(num_stairs);
    display_ways(ways);

    return 0;
}
