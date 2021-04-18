/*******************************************************************************
 * Name    : waterjugpuzzle.cpp
 * Author  : Adrian Gomes
 * Date    : March 19, 2021
 * Description : Waterjug Puzzle.
 * Pledge  : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <sstream>
#include <queue>
#include <vector>

using namespace std;

// Struct to represent state of water in the jugs.
struct State {
    int a, b, c;
    string directions;
    State *parent;

    State(int _a, int _b, int _c, string _directions) :
        a(_a), b(_b), c(_c), directions(_directions), parent(nullptr) {}

    State(int _a, int _b, int _c, string _directions, State *_parent) :
        a(_a), b(_b), c(_c), directions(_directions), parent(_parent) {}

    // String representation of state in tuple form.
    string to_string() {
        ostringstream oss;
        oss << directions << (directions.compare("") ? " " : "" ) << "(" << a << ", " << b << ", " << c << ")";
        return oss.str();
    }

    bool equals(State s) {
        return (a == s.a && b == s.b && c == s.c);
    }
};

string print_string(State *ptr) {
    if (ptr == nullptr) return "";
    if (ptr->parent == nullptr) return ptr->to_string();

    return print_string(ptr->parent) + "\n" + ptr->to_string();
}

void empty_trash(queue<State*> trash) {
    while (!trash.empty()) {
        delete trash.front();
        trash.pop();
    }
}

string bfs(int a, int b, int c, State goal) {
    vector<vector<State*> > visited(a + 1, vector<State*>(b + 1, nullptr));
    queue<State*> states;
    queue<State*> trash;

    State* init = new State(0, 0, c, "Initial state.");
    states.push(init);
    trash.push(init);

    while (!states.empty()) {
        State *current = states.front();
        states.pop();

        if (current->equals(goal)) {
            string directions = print_string(current);
            empty_trash(trash);
            return directions;
        }

        int curr_jugs[] = {current->a, current->b, current->c};

        if (visited[curr_jugs[0]][curr_jugs[1]] != nullptr)
            continue;

        visited[curr_jugs[0]][curr_jugs[1]] = current;

        for (int i = 3, j = 0; j < 3; j++) {
            for(int k = 0; k < 2; k++) {
                i--;
                if (i == j) i--;

                int cap = (j == 0) ? a : (j == 1) ? b : c;

                if (curr_jugs[i] > 0 && curr_jugs[j] < cap) {
                    int pour = min(curr_jugs[i], cap - curr_jugs[j]);
                    int new_jugs[] = {curr_jugs[0], curr_jugs[1], curr_jugs[2]};

                    new_jugs[i] -= pour;
                    new_jugs[j] += pour;

                    if (visited[new_jugs[0]][new_jugs[1]] != nullptr)
                        continue;

                    ostringstream dir;
                    dir << "Pour " << pour << " gallon" << (pour > 1 ? "s" : "") << " from " << char(i + 65) << " to " << char(j + 65) << ".";

                    State* new_state = new State(new_jugs[0], new_jugs[1],
                        new_jugs[2], dir.str(), current);
                    states.push(new_state);
                    trash.push(new_state);
                }
            }
            i += 2;
        }
    }
    empty_trash(trash);
    return "No solution.";
}

int main(int argc, char * const argv[]) {
    int len = 6;

    if (argc != len + 1) {
        printf("Usage: %s <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>\n", argv[0]);
        return 1;
    }

    int input[6];

    for (int i = 1; i < argc; i++) {

        istringstream iss(argv[i]);

        if ( !(iss >> input[i - 1]) ||
             input[i - 1] < 0 ||
             (i == len/2 && input[i - 1] < 1)
        ) {
            printf("Error: Invalid %s '%s' for jug %c.\n",
                ((i <= len / 2) ? "capacity" : "goal"),
                argv[i], ((i - 1) % (len / 2)) + 65);
            return 1;
        }
    }

    int full_cap = 0;

    for (int i = 0; i < len; i++) {
        if (i < len / 2 && input[i + len / 2] > input[i]) {
            printf("Error: Goal cannot exceed capacity of jug %c.\n", i + 65);
            return 1;
        }

        if (i >= len / 2)
            full_cap += input[i];
    }

    if (full_cap != input[len/2 - 1]) {
        cout << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
        return 1;
    }

    State goal(input[3], input[4], input[5], "");

    cout << bfs(input[0], input[1], full_cap, goal) << endl;

    return 0;
}
