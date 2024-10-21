#include <iostream>
#include <vector>
#include <list>
using namespace std;

// g++ -c -fPIC MiniMaxAsLibrary.cpp -o mini_max.o -std=c++17
// g++ -shared -Wl,-install_name,mini_max.so -o mini_max.so  mini_max.o

char AI_MARKER;
char OPPONENT_MARKER = 0;

// Function declarations
char checkForWin(const vector<vector<char>> &gameState, int &countToWin);
char linearWinnerCheck(const vector<vector<char>> &gameState, int &countToWin);
char diagonalWinnerCheck(const vector<vector<char>> &gameState, int &countToWin);
char arrayWinnerCheck(const vector<char> &array, int countToWin);
tuple<int, tuple<int, int>> miniMaxConnectFour(vector<vector<char>> &game_state, int depth, char current_player, int &count_to_win);
tuple<int, tuple<int, int>> miniMaxTicTacToe(vector<vector<char>> &game_state, int depth, char current_player, int &count_to_win);

char *getInputTicTacToe(int search_depth, int count_to_win, int y_length, int x_length, char ai_marker, char opponent_marker, char *array_begin)
{
    AI_MARKER = ai_marker;
    OPPONENT_MARKER = opponent_marker;
    vector<vector<char>> game_state(y_length, vector<char>(x_length, ' '));
    for (int y = 0; y < y_length; y++)
    {
        for (int x = 0; x < x_length; x++)
        {
            game_state[y][x] = *(array_begin + y * x_length + x);
        }
    }

    tuple<int, tuple<int, int>> result = miniMaxTicTacToe(game_state, search_depth, ai_marker, count_to_win);
    tuple<int, int> coordinates = get<1>(result);
    string *output = new string(to_string(get<0>(coordinates)) + '-' + to_string(get<1>(coordinates)));
    return (*output).data();
}

char *getInputConnectFour(int search_depth, int count_to_win, int y_length, int x_length, char ai_marker, char opponent_marker, char *array_begin)
{
    AI_MARKER = ai_marker;
    OPPONENT_MARKER = opponent_marker;
    vector<vector<char>> game_state(y_length, vector<char>(x_length, ' '));
    for (int y = 0; y < y_length; y++)
    {
        for (int x = 0; x < x_length; x++)
        {
            game_state[y][x] = *(array_begin + y * x_length + x);
        }
    }

    tuple<int, tuple<int, int>> result = miniMaxConnectFour(game_state, search_depth, ai_marker, count_to_win);
    tuple<int, int> coordinates = get<1>(result);
    string *output = new string(to_string(get<0>(coordinates)) + '-' + to_string(get<1>(coordinates)));
    return (*output).data();
}

extern "C"
{
    char *get_input_tictactoe(int search_depth, int count_to_win, int y, int x, char ai_marker, char opponent_marker, char *array_begin) { return getInputTicTacToe(search_depth, count_to_win, y, x, ai_marker, opponent_marker, array_begin); }
    char *get_input_connectfour(int search_depth, int count_to_win, int y, int x, char ai_marker, char opponent_marker, char *array_begin) { return getInputConnectFour(search_depth, count_to_win, y, x, ai_marker, opponent_marker, array_begin); }
}

char togglePlayer(char current_player)
{
    if (current_player == AI_MARKER)
    {
        return OPPONENT_MARKER;
    }
    else
    {
        return AI_MARKER;
    }
}

void print(vector<vector<char>> &game_state)
{
    for (int y = 0; y < game_state.size(); y++)
    {
        for (int x = 0; x < game_state[y].size(); x++)
        {
            cout << game_state[y][x];
        }
        cout << endl;
    }
    cout << "---" << endl;
}

tuple<int, tuple<int, int>> miniMaxTicTacToe(vector<vector<char>> &game_state, int depth, char current_player, int &count_to_win)
{
    // print(game_state);
    if (depth < 0)
    {
        return tuple<int, tuple<int, int>>(0, tuple<int, int>(-1, -1));
    }
    char winner = checkForWin(game_state, count_to_win);
    if (winner == OPPONENT_MARKER)
    {
        return tuple<int, tuple<int, int>>(-1, tuple<int, int>(-1, -1));
    }
    else if (winner == AI_MARKER)
    {
        return tuple<int, tuple<int, int>>(2, tuple<int, int>(-1, -1));
    }
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
                auto temp = miniMaxTicTacToe(game_state, depth - 1, togglePlayer(current_player), count_to_win);
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

void makeMove_connectFour(vector<vector<char>> &game_state, int index, char current_player)
{
    for (int y = game_state.size() - 1; y >= 0; y--)
    {
        if (game_state[y][index] == ' ')
        {
            game_state[y][index] = current_player;
            break;
        }
    }
}

void deleteMove_connectFour(vector<vector<char>> &game_state, int index, char current_player)
{
    for (int y = 0; y < game_state.size(); y++)
    {
        if (game_state[y][index] != ' ')
        {
            game_state[y][index] = ' ';
            break;
        }
    }
}

tuple<int, tuple<int, int>> miniMaxConnectFour(vector<vector<char>> &game_state, int depth, char current_player, int &count_to_win)
{
    // print(game_state);
    if (depth < 0)
    {
        return tuple<int, tuple<int, int>>(0, tuple<int, int>(0, -1));
    }
    char winner = checkForWin(game_state, count_to_win);
    if (winner == OPPONENT_MARKER)
    {
        return tuple<int, tuple<int, int>>(-1, tuple<int, int>(0, -1));
    }
    else if (winner == AI_MARKER)
    {
        return tuple<int, tuple<int, int>>(5, tuple<int, int>(0, -1));
    }
    int max = -2147483648;
    int total = 0;
    tuple<int, int> maxFieldIndex = tuple<int, int>(-1, -1);
    for (int x = 0; x < game_state[0].size(); x++)
    {
        if (game_state[0][x] == ' ')
        {
            makeMove_connectFour(game_state, x, current_player);
            auto temp = miniMaxConnectFour(game_state, depth - 1, togglePlayer(current_player), count_to_win);
            total += get<0>(temp);
            if (get<0>(temp) > max)
            {
                max = get<0>(temp);
                maxFieldIndex = tuple<int, int>(0, x);
            }
            deleteMove_connectFour(game_state, x, current_player);
        }
    }
    return tuple<int, tuple<int, int>>(total, maxFieldIndex);
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