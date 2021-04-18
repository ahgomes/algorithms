/*******************************************************************************
 * Name    : unique.cpp
 * Author  : Adrian Gomes
 * Date    : Feb 28, 2021
 * Description : Determining uniqueness of chars with int as bit vector.
 * Pledge  : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <cctype>

using namespace std;

bool is_all_lowercase(const string &s) {
    for (size_t i = 0, len = s.length(); i < len; i++)
        if (!islower(s[i])) return false;

    return true;
}

bool all_unique_letters(const string &s) {
    unsigned int vector = 0;
    unsigned int setter;

    for (size_t i = 0, len = s.length(); i < len; i++) {
        setter = 1 << (s[i] - 'a');
        if ((vector & setter) != 0) return false;
        vector = vector | setter;
    }

    return true;
}

int main(int argc, char * const argv[]) {
    string str;

    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <string>" << endl;
        return 1;
    }

    str = argv[1];
    if (!is_all_lowercase(str)) {
        cerr << "Error: String must contain only lowercase letters." << endl;
        return 1;
    }

    if (all_unique_letters(str)) {
        cout << "All letters are unique." << endl;
    } else {
        cout << "Duplicate letters found." << endl;
    }

    return 0;
}
