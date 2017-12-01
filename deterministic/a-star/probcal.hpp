#include<iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std ;


bool horizontalWallCollision(int cols, int col)
{
    return col < 0 || col == cols;
}

bool verticalWallCollision(int rows, int row)
{
    return row < 0 || row == rows;
}

int coordsToNum(int cols, int row, int col)
{
    return row * cols + col;
}

void printMatrix(int** grid){
    for (int r = 0; r <6; r++) {
        for (int c = 0; c < 6; c++)
            cout << grid[r][c] << " ";
        cout << "\n";
    }
}

/**
  * Every move has a 20% chance at each step
  * @param n_hist - Size of history
  * @param history - X and Y values of past moves
  * @param rows - Number of rows
  * @param cols - Number of columns
  * @param prob - Pass by ref matrix of dynamic obstacle probability (size)x3
  */
void getProb (int n_hist, int history[][2], int rows, int cols, double prob [][3], int steps)
{
    // Without using the history...
    int currCol = history[0][0];
    int currRow = history[0][1];

    int** grid;
    grid=new int *[rows];
    for (int i = 0; i <rows; ++i)
        grid[i]=new int [cols];


    // Set initial prob
    grid[currRow][currCol] = 1;

    int leftCol = currCol - 1;
    int rightCol = currCol + 1;
    int upRow = currRow + 1;
    int downRow = currRow - 1;

    if (horizontalWallCollision(rows, upRow)) {
        grid[currRow][currCol]++;
    } else {
        grid[upRow][currCol]++;
    }
    if (horizontalWallCollision(rows, downRow)) {
        grid[currRow][currCol]++;
    } else {
        grid[downRow][currCol]++;
    }
    if (verticalWallCollision(cols, leftCol)) {
        grid[currRow][currCol]++;
    } else {
        grid[currRow][leftCol]++;
    }
    if (verticalWallCollision(cols, rightCol)) {
        grid[currRow][currCol]++;
    } else {
        grid[currRow][rightCol]++;
    }

    for(int row = 0; row < rows; row ++) {
        for (int col = 0; col < cols; col++) {
            if(grid[row][col] > 0) {
                prob[coordsToNum(cols, row, col)][0] = grid[row][col] / 5.0;
            }
        }
    }

    int lastCount = 5;

    for(int step = 2; step <= steps; step++) {
        int** newGrid;
        newGrid=new int *[rows];
        for (int i = 0; i <rows; ++i)
            newGrid[i]=new int [cols];

        // Loop through all grid spaces and inc counters
        int countTotal = lastCount;
        for(int row = 0; row < rows; row ++) {
            for(int col = 0; col < cols; col ++) {
                if(grid[row][col] > 0) {
                    leftCol = col - 1;
                    rightCol = col + 1;
                    upRow = row + 1;
                    downRow = row - 1;

                    newGrid[row][col] += grid[row][col] + 1;
                    countTotal++;

                    if (horizontalWallCollision(rows, upRow)) {
                        newGrid[row][col]++;
                        countTotal++;
                    } else {
                        newGrid[upRow][col]++;
                        countTotal++;
                    }
                    if (horizontalWallCollision(rows, downRow)) {
                        newGrid[row][col]++;
                        countTotal++;
                    } else {
                        newGrid[downRow][col]++;
                        countTotal++;
                    }
                    if (verticalWallCollision(cols, leftCol)) {
                        newGrid[row][col]++;
                        countTotal++;
                    } else {
                        newGrid[row][leftCol]++;
                        countTotal++;
                    }
                    if (verticalWallCollision(cols, rightCol)) {
                        newGrid[row][col]++;
                        countTotal++;
                    } else {
                        newGrid[row][rightCol]++;
                        countTotal++;
                    }
                }
            }
        }

        lastCount = countTotal;
        for(int row = 0; row < rows; row ++) {
            for (int col = 0; col < cols; col++) {
                if(newGrid[row][col] > 0) {
                    prob[coordsToNum(cols, row, col)][0] = newGrid[row][col] / (double)countTotal;
                }
            }
        }

        grid = newGrid;
    }
}

/*vector<vector<double>> getProb (int n_hist, int history[][2], int rows, int cols)
{
    //vector<vector<double>> prob (vector<double>, rows);
    vector<double> rowVect (0.0, rows);
    vector<vector<double>> prob (rowVect, cols);

    // Without using the history...
    int currCol = history[0][0];
    int currRow = history[0][1];

    int grid [rows][cols];

    // Set initial prob
    grid[currRow][currCol] = 1;

    int leftCol = currCol - 1;
    int rightCol = currCol + 1;
    int upRow = currRow + 1;
    int downRow = currRow - 1;

    if (horizontalWallCollision(rows, upRow)) {
        grid[currRow][currCol]++;
    } else {
        grid[upRow][currCol]++;
    }
    if (horizontalWallCollision(rows, downRow)) {
        grid[currRow][currCol]++;
    } else {
        grid[downRow][currCol]++;
    }
    if (verticalWallCollision(cols, leftCol)) {
        grid[currRow][currCol]++;
    } else {
        grid[currRow][leftCol]++;
    }
    if (verticalWallCollision(cols, rightCol)) {
        grid[currRow][currCol]++;
    } else {
        grid[currRow][rightCol]++;
    }

    for(int row = 0; row < rows; row ++) {
        for (int col = 0; col < cols; col++) {
            if(grid[row][col] > 0) {
                prob[coordsToNum(cols, row, col)][0] = grid[row][col] / 5.0;
                prob.push_back()
            }
        }
    }

    int lastCount = 5;

    for(int step = 1; step < 3; step++) {
        // Loop through all grid spaces and inc counters
        int countTotal = lastCount;
        for(int row = 0; row < rows; row ++) {
            for(int col = 0; col < cols; col ++) {
                if(grid[row][col] > 0) {
                    leftCol = col - 1;
                    rightCol = col + 1;
                    upRow = row + 1;
                    downRow = row - 1;

                    if (horizontalWallCollision(rows, upRow)) {
                        grid[row][col]++;
                        countTotal++;
                    } else {
                        grid[upRow][col]++;
                        countTotal++;
                    }
                    if (horizontalWallCollision(rows, downRow)) {
                        grid[row][col]++;
                        countTotal++;
                    } else {
                        grid[downRow][col]++;
                        countTotal++;
                    }
                    if (verticalWallCollision(cols, leftCol)) {
                        grid[row][col]++;
                        countTotal++;
                    } else {
                        grid[row][leftCol]++;
                        countTotal++;
                    }
                    if (verticalWallCollision(cols, rightCol)) {
                        grid[row][col]++;
                        countTotal++;
                    } else {
                        grid[row][rightCol]++;
                        countTotal++;
                    }
                }
            }
        }

        lastCount = countTotal;

        double sum = 0;
        for(int row = 0; row < rows; row ++) {
            for (int col = 0; col < cols; col++) {
                if(grid[row][col] > 0) {
                    prob[coordsToNum(cols, row, col)][0] = grid[row][col] / (double)countTotal;
                    sum += grid[row][col] / (double)countTotal;
                }
            }
        }
    }
}*/

/**
 * Get the transition probability
 * @param state - string representation of the state
 * @param rows - Number of rows in the grid world
 * @param cols - Number of columns in the grid world
 * @return vector of transition probabilities
 */
std::vector<pair<double, string>> getTransProb (string state, int rows, int cols)
{
    const int girdSize = rows*cols;

    vector<pair<double, string>> result;
    pair<int, int> coord;
    std::stringstream ss(state);

    //vector<> probs;

    int i;
    int count = 0;
    while (ss >> i)
    {
        count++;

        if(count == 1)
            coord.first = i;
        else
            coord.second = i;

        if (ss.peek() == ',')
            ss.ignore();

        // New coord
        if (count >= 2) {
            count = 0;

            int hist[1][2]={{coord.first, coord.second}};

            // TODO Hard coded! This is not good
            double prob[36][3];
            getProb(3, hist, rows, cols, prob, 3);

        }
    }

    //TODO Use probs to calc trans prob

    return result;
}
