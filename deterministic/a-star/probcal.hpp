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

string numToCoordStr(int num, int cols)
{
    int row = num / cols;
    int col = num % cols;

    return to_string(row) + "," + to_string(col);
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
void getProb (int n_hist, int history[][2], int rows, int cols, double **prob, int steps)
{
    // Without using the history...
    int currCol = history[0][1];
    int currRow = history[0][0];

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

vector<double> getProbVect (int n_hist, int history[][2], int rows, int cols, int steps)
{
    vector<double> prob(rows*cols, 0.0);

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
                prob[coordsToNum(cols, row, col)] = grid[row][col] / 5.0;
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
        double sum = 0;
        for(int row = 0; row < rows; row ++) {
            for (int col = 0; col < cols; col++) {
                if(newGrid[row][col] > 0) {
                    prob[coordsToNum(cols, row, col)] = newGrid[row][col] / (double)countTotal;
                    sum += newGrid[row][col] / (double)countTotal;
                }
            }
        }

        cout << "\n sum: " << sum;

        grid = newGrid;
    }

    return prob;
}

/**
 * Recursive function to calculate trans prob
 * @param currProb
 * @param state
 * @param probIndex
 * @param probs
 * @param cols
 * @param result
 */
vector<pair<double, string>> calcTransProb (double currProb, string state, int probIndex, vector<vector<double>> probs, int cols, vector<pair<double, string>> thisResult)
{
    vector<double> thisProb = probs[probIndex];
    //vector<pair<double, string>> thisResult;
    for(int thisPos = 0; thisPos < thisProb.size(); thisPos++) {
        if(thisProb[thisPos] > 0) {
            double thisPosProb = thisProb[thisPos] * currProb;
            string test = numToCoordStr(thisPos, cols);
            string newState;
            if (state.size() == 0) {
                newState = test;
            } else {
                newState = state + "," + test;
            }
            probIndex += 1;
            if (probIndex < probs.size()) {
                thisResult = calcTransProb(thisPosProb, newState, probIndex, probs, cols, thisResult);
            } else {
                pair<double, string> transProb;
                transProb.first = thisPosProb;
                transProb.second = newState;

                thisResult.push_back(transProb);
            }
            probIndex -= 1;
        }
    }

    return thisResult;
}

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

    //vector<> probs;
    int** newGrid;
    newGrid=new int *[rows];
    for (int i = 0; i <rows; ++i)
        newGrid[i]=new int [cols];

    std::stringstream ss(state);

    vector<vector<double>> probs;

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

            int hist[1][2] = {{coord.first, coord.second}};

            vector<double> prob = getProbVect(3, hist, rows, cols, 1);
            probs.push_back(prob);
        }
    }

    //TODO Use probs to calc trans prop. Need to use recursion!
    /*for(int currP = 0; currP < probs.size(); currP++){
        vector<double> currProb = probs[currP];

        for(int currPos = 0; currPos < currProb.size(); currPos++){
            double currPosProb = currProb[currPos];
            if(currPosProb > 0){

                for(int othP = 0; othP < probs.size(); othP++) {
                    if(othP == currP)
                        continue;

                    vector<double> othProb = probs[othP];
                    for(int othPos = 0; othPos < othProb.size(); othPos++) {
                        double othPosProb = othProb[othPos];
                        if (othPosProb > 0) {
                            double transProb = currPosProb * othPosProb;
                            string state =
                            pair<double, string>
                        }
                    }
                }

            }
        }
    }*/

    result = calcTransProb(1, "", 0, probs, cols, result);

    // Check
    /*cout << "\nResult size: " << result.size();

    double sum = 0;
    for(int r = 0; r < result.size(); r++){
        sum += result[r].first;
        cout << "\nState: " << result[r].second;
    }

    cout << "\nResult Sum: " << sum;*/

    return result;
}
