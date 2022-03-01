/*******************************************************************************
 * Name: mazesolver.cpp
 * Author: Adrian Gomes
 * Date: May 17, 2021
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System.
 * Description: Uses recursive backtracking to solve a maze.
 ******************************************************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <queue>

using namespace std;

enum direction_t {
    NORTH, EAST, SOUTH, WEST
};

direction_t directions[] = {NORTH, EAST, SOUTH, WEST};

/**
 * Simple struct that represents a cell within the text-based maze.
 */
struct Cell {
    int row, col;

    /**
     * No-args constructor.
     */
    Cell() : row{0}, col{0} { }

    /**
     * Constructor that takes in (row, col) coordinates.
     */
    Cell(int _row, int _col) : row{_row}, col{_col} { }

    /**
     * Checks if two cells are equal.
     */
    bool operator==(const Cell &other) const {
        return row == other.row && col == other.col;
    }

    /**
     * Returns that cell reached from the current cell when moving in the
     * specified direction.
     */
    Cell adjacent_cell(const direction_t direction) const {
        switch (direction) {
            case NORTH:
                return Cell(row - 1, col);
            case EAST:
                return Cell(row, col + 1);
            case SOUTH:
                return Cell(row + 1, col);
            case WEST:
                return Cell(row, col - 1);
            default:
            	// Unknown direction -- just return a cell with the
            	// coordinates of the current cell.
                return Cell(row, col);
        }
    }

    /**
     * Allows for printing a cell from a cout statement.
     */
    friend std::ostream& operator<<(std::ostream& os, const Cell &cell) {
        os << "(" << cell.row << ", " << cell.col << ")";
        os.flush();
        return os;
    }
};

class MazeSolver {
public:
	/**
	 * Constructor for creating a MazeSolver. Takes as argument a string
	 * representing the filename of the text file containing the maze.
	 */
    explicit MazeSolver(const string &filename) :
        filename_{filename}, input_file_{filename.c_str()} {
        if (!input_file_) {
            throw runtime_error("File '" + filename + "' not found.");
        }
        input_file_.exceptions(std::ifstream::badbit);
        parse_file();
    }


    /**
     * Destructor. Used to free up memory allocated inside a MazeSolver.
     */
    ~MazeSolver() {
        for (int i = 0; i < num_rows_; i++)
            delete [] maze_[i];
        delete [] maze_;
    }

    /**
     * Returns the path length of the solution.
     */
    int path_length() const {
        return path_length_;
    }

    /**
     * Attempts to solve the maze and returns whether or not a solution exists.
     */
    bool solve() {
        return solve(start_);
    }

    /**
     * Prints the ASCII maze on the screen with cout statements.
     */
    void display_maze() const {
        for (int row = 0; row < num_rows_; row++) {
            for (int col = 0; col < num_cols_; col++)
                cout << maze_[row][col];
            cout << endl;
        }
    }

    static const int
		WALL_CHARACTER = '#',
		MARK_CHARACTER = '.',
		SPACE_CHARACTER = ' ';

private:
    string filename_;
    ifstream input_file_;
    char **maze_;
    int num_rows_, num_cols_, path_length_;
    Cell start_, end_;

    /**
     * Parses the text file so that upon successful completion of this method,
     * maze_ is a 2D array of characters, start_ and end_ represent the two
     * open cells in the exterior of the maze, and path_length_ is initialized
     * to 0.
     */
    void parse_file() {
        vector<string> v;
        string line;
        while (getline(input_file_, line)) {
            v.push_back(line);
        }

        num_rows_ = v.size();
        if (num_rows_ == 0) {
            throw runtime_error("File '" + filename_ + "' is empty.");
        }
        num_cols_ = v[0].length();
        if (num_cols_ == 0) {
            throw runtime_error("File '" + filename_ + "' is empty.");
        }

        vector<Cell> points = start_and_end_points(v);

        maze_ = new char*[num_rows_];
        for (int row = 0; row < num_rows_; row++) {
            maze_[row] = new char[num_cols_];
            for (int col = 0; col < num_cols_; col++) {
                maze_[row][col] = v[row][col];
                if (maze_[row][col] == '\0')
                    maze_[row][col] = SPACE_CHARACTER;
            }
        }
        start_ = points[0];
        end_ = points[1];
        path_length_ = 0;
    }

    /**
     * Takes as input a vector of strings and returns a vector of exactly two
     * cells, representing the starting and ending points in the exterior walls
     * in the maze.
     */
    vector<Cell> start_and_end_points(const vector<string> &v) const {
        vector<Cell> points;
        unsigned int last_row = num_rows_ - 1, last_col = num_cols_ - 1;

        size_t col, row = 0;
        if ((col = v[row].find(SPACE_CHARACTER)) != string::npos)
            points.push_back(Cell(row, col));

        row = last_row;
        if ((col = v[row].find(SPACE_CHARACTER)) != string::npos)
            points.push_back(Cell(row, col));

        row = 0, col = 0;
        for (; (int)row < num_rows_;) {
            if (v[row][col] != WALL_CHARACTER)
                points.push_back(Cell(row, col));
            if ((int)++row == num_rows_ && col == 0) {
                row = 0;
                col = last_col;
            }
        }

        size_t num_openings = points.size();
        if (num_openings == 0) {
            throw runtime_error("Maze has no openings.");
        } else if (num_openings == 1) {
            throw runtime_error("Maze has only 1 opening.");
        } else if (num_openings > 2) {
            throw runtime_error(
                    "Ambiguous path. Maze has more than 2 openings.");
        }
        return points;
    }

    /**
     * Attempts to solve the maze and mark the path from the starting cell to
     * the ending cell. Uses recursive backtracking to mark the path.
     */
    bool solve(Cell cell) {
        Cell null = Cell(-1, -1);
        vector<vector<Cell> > visited(num_rows_,
            vector<Cell>(num_cols_, null));
        queue<Cell> q;

        visited[cell.row][cell.col] = start_;
        q.push(cell);

        while (!q.empty()) {
            Cell curr = q.front();
            q.pop();
            if (curr == end_) {
                while (!(curr == start_)) {
                    maze_[curr.row][curr.col] = MARK_CHARACTER;
                    path_length_++;
                    curr = visited[curr.row][curr.col];
                }
                maze_[curr.row][curr.col] = MARK_CHARACTER;
                path_length_++;
                return true;
            }
            vector<Cell> edges = adj_edges(curr);
            for (auto e : edges) {
                if (visited[e.row][e.col] == null) {
                    visited[e.row][e.col] = curr;
                    q.push(e);
                }
            }
        }

        return false;
    }

    vector<Cell> adj_edges(Cell cell) {
        vector<Cell> edges;

        for (int i = 0; i < 4; i++) {
            Cell a = cell.adjacent_cell(directions[i]);
            int row = a.row, col = a.col;
            if (
                row >= 0 && row < num_rows_ &&
                col >= 0 && col < num_cols_ &&
                maze_[row][col] != WALL_CHARACTER
            ) {
                edges.push_back(a);
            }
        }

        return edges;
    }
};

/**
 * Main function for reading the command line arguments and producing output.
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <maze file>" << endl;
        return 1;
    }
    MazeSolver *maze_solver = NULL;
    try {
        maze_solver = new MazeSolver(argv[1]);
        if (maze_solver->solve()) {
            cout << "Solution exists: YES" << endl
                 << "Total moves:     " << maze_solver->path_length()
                 << endl << endl;
            maze_solver->display_maze();
        } else {
            cout << "Solution exists: NO" << endl;
        }
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    // Don't forget to delete the pointer to the maze solver.
    delete maze_solver;
    return 0;
}
