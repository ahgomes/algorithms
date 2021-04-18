/*******************************************************************************
 * Name: inversioncounter.cpp
 * Author: Adrian Gomes
 * Date: March 31, 2021
 * Description: Counts the number of inversions in an array.
 * Pledge: I pledge my honor that I have abided by the Steven Honor System.
 ******************************************************************************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

// Function prototype.
static long mergesort(int array[], int scratch[], int low, int high);

/**
 * Counts the number of inversions in an array in theta(n^2) time.
 */
long count_inversions_slow(int array[], int length) {
    long count = 0;

    for (int i = 0; i < length; i++)
        for (int j = i + 1; j < length; j++)
            if (array[i] > array[j])
                count++;

    return count;
}

/**
 * Counts the number of inversions in an array in theta(n lg n) time.
 */
long count_inversions_fast(int array[], int length) {
    vector<int> scratch(length, 0);
    return mergesort(array, &scratch[0], 0, length - 1);
}

static long mergesort(int array[], int scratch[], int low, int high) {
    long count = 0;

    if (low < high) {
        int mid = low + (high - low) / 2;

        count += mergesort(array, scratch, low, mid);
        count += mergesort(array, scratch, mid + 1, high);

        int l = low, h = mid + 1;

        for (int k = low; k <= high; k++) {
            if (l <= mid && (h > high || array[l] <= array[h]))
                scratch[k] = array[l++];
            else {
                scratch[k] = array[h];
                if (array[l] > array[h++])
                    count += h - k - 1;
            }

        }

        for (int k = low; k <= high; k++)
            array[k] = scratch[k];
    }

    return count;
}

int main(int argc, char *argv[]) {
    ostringstream usage;
    usage << "Usage: " << argv[0] << " [slow]";

    if (argc > 2) {
        cout << usage.str() << endl;
        return 1;
    }

    bool slow = false;

    if (argc == 2) {
        string opt = argv[1];
        if (opt.compare("slow") == 0)
            slow = true;
        else {
            cerr << "Error: Unrecognized option '" << opt << "'." << endl;
            return 1;
        }
    }

    cout << "Enter sequence of integers, each followed by a space: " << flush;

    istringstream iss;
    int value, index = 0;
    vector<int> values;
    string str;
    str.reserve(11);
    char c;
    while (true) {
        c = getchar();
        const bool eoln = c == '\r' || c == '\n';
        if (isspace(c) || eoln) {
            if (str.length() > 0) {
                iss.str(str);
                if (iss >> value) {
                    values.push_back(value);
                } else {
                    cerr << "Error: Non-integer value '" << str
                         << "' received at index " << index << "." << endl;
                    return 1;
                }
                iss.clear();
                ++index;
            }
            if (eoln) {
                break;
            }
            str.clear();
        } else {
            str += c;
        }
    }

    if (values.empty()) {
        cerr << "Error: Sequence of integers not received." << endl;
        return 1;
    }

    if (values.size() == 1) {
        cout << "Number of inversions: 0" << endl;
        return 0;
    }

    cout << "Number of inversions: " << (slow ?
        count_inversions_slow(&values[0], values.size()) :
        count_inversions_fast(&values[0], values.size())
    );
    cout << endl;

    return 0;
}
