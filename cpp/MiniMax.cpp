#include <iostream>
#include <vector>
#include <list>
using namespace std;

char ai_marker = 0;
char opponent_marker = 0;

// Function declarations
char checkForWin(const vector<vector<char>> &gameState, int &countToWin);
char linearWinnerCheck(const vector<vector<char>> &gameState, int &countToWin);
char diagonalWinnerCheck(const vector<vector<char>> &gameState, int &countToWin);
char arrayWinnerCheck(const vector<char> &array, int countToWin);
tuple<int, tuple<int, int>> miniMax(vector<vector<char>> &game_state, int depth, char current_player, int &count_to_win);

//  ./ a.out 3 3 o x 5 5 .........................
int main(int search_depth, const char *arguments[])
{
    int count_to_win = stoi(arguments[2]);
    ai_marker = *arguments[3];
    opponent_marker = *arguments[4];

    int vector_size_y = stoi(arguments[5]);
    int vector_size_x = stoi(arguments[6]);

    vector<vector<char>> game_state(vector_size_y, vector<char>(vector_size_x, ' '));
    for (int y = 0; y < game_state.size(); y++)
    {
        for (int x = 0; x < game_state[y].size(); x++)
        {
            if (arguments[y + 7][x] == '.')
            {
                game_state[y][x] = ' ';
            }
            else
            {
                game_state[y][x] = arguments[y + 7][x];
            }
        }
    }
    tuple<int, tuple<int, int>> result = miniMax(game_state, search_depth, ai_marker, count_to_win);
    tuple<int, int> coordinates = get<1>(result);
    cout << get<0>(coordinates) << " " << get<1>(coordinates) << endl;
    return 0;
}

char togglePlayer(char current_player)
{
    if (current_player == ai_marker)
    {
        return opponent_marker;
    }
    else
    {
        return ai_marker;
    }
}

tuple<int, tuple<int, int>> miniMax(vector<vector<char>> &game_state, int depth, char current_player, int &count_to_win)
{
    if (depth < 0)
    {
        return tuple<int, tuple<int, int>>(2, tuple<int, int>(-1, -1));
    }
    char winner = checkForWin(game_state, count_to_win);
    if (winner == opponent_marker)
    {
        return tuple<int, tuple<int, int>>(-2, tuple<int, int>(-1, -1));
    }
    else if (winner == ai_marker)
    {
        return tuple<int, tuple<int, int>>(1, tuple<int, int>(-1, -1));
    }
    else if (winner == ' ')
    {
        int max = -2147483648;
        int total = 0;
        tuple<int, int> maxFieldIndex = tuple<int, int>(-1, -1);
        for (int y = 0; y < game_state.size(); y++)
        {
            for (int x = 0; x < game_state[y].size(); x++)
            {
                if (game_state[y][x] == ' ')
                {
                    game_state[y][x] = current_player;
                    auto temp = miniMax(game_state, depth - 1, togglePlayer(current_player), count_to_win);
                    total += get<0>(temp);
                    if (get<0>(temp) > max)
                    {
                        max = get<0>(temp);
                        maxFieldIndex = tuple<int, int>(y, x);
                    }
                    game_state[y][x] = ' ';
                }
            }
        }
        return tuple<int, tuple<int, int>>(total, maxFieldIndex);
    }
    return tuple<int, tuple<int, int>>(-1, tuple<int, int>(-1, -1));
}

// Function to check for a win in the game
char checkForWin(const vector<vector<char>> &gameState, int &countToWin)
{
    char winner = ' ';

    // Horizontal check
    if (winner == ' ')
    {
        winner = linearWinnerCheck(gameState, countToWin);
    }

    // Vertical check (transpose the gameState)
    if (winner == ' ')
    {
        vector<vector<char>> transposed(gameState[0].size(), vector<char>(gameState.size()));
        for (size_t i = 0; i < gameState.size(); ++i)
        {
            for (size_t j = 0; j < gameState[0].size(); ++j)
            {
                transposed[j][i] = gameState[i][j];
            }
        }
        winner = linearWinnerCheck(transposed, countToWin);
    }

    // Diagonal check
    if (winner == ' ')
    {
        winner = diagonalWinnerCheck(gameState, countToWin);
    }

    // Flipped diagonal check
    if (winner == ' ')
    {
        vector<vector<char>> flipped(gameState);
        for (auto &row : flipped)
        {
            reverse(row.begin(), row.end());
        }
        winner = diagonalWinnerCheck(flipped, countToWin);
    }

    return winner;
}

// Function to check for a win along rows or columns
char linearWinnerCheck(const vector<vector<char>> &gameState, int &countToWin)
{
    for (const auto &row : gameState)
    {
        char winner = arrayWinnerCheck(row, countToWin);
        if (winner != ' ')
        {
            return winner;
        }
    }
    return ' ';
}

// Function to check diagonals for a winner
char diagonalWinnerCheck(const vector<vector<char>> &gameState, int &countToWin)
{
    int n = gameState.size();
    for (int offset = 0 - n; offset < n; ++offset)
    {
        vector<char> diagArray;
        for (int i = 0; i < n; ++i)
        {
            int j = i + offset;
            if (j >= 0 && j < n)
            {
                diagArray.push_back(gameState[i][j]);
            }
        }
        if (diagArray.size() >= countToWin)
        {
            char winner = arrayWinnerCheck(diagArray, countToWin);
            if (winner != ' ')
            {
                return winner;
            }
        }
    }
    return ' ';
}

char arrayWinnerCheck(const vector<char> &array, int countToWin)
{
    int consecutiveCount = 0;
    char possibleWinner = ' ';

    for (size_t i = 0; i < array.size(); ++i)
    {
        if (array[i] == ' ')
        {
            consecutiveCount = 0;
            possibleWinner = ' ';
        }
        else if (array[i] == possibleWinner)
        {
            consecutiveCount++;
        }
        else
        {
            possibleWinner = array[i];
            consecutiveCount = 1;
        }
        if (consecutiveCount == countToWin)
        {
            return possibleWinner;
        }
    }
    return ' ';
}