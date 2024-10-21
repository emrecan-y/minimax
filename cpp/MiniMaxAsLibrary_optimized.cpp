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
tuple<int, tuple<int, int>> miniMaxConnectFour(vector<vector<char>> &game_state, int depth, char current_player, int &count_to_win);
tuple<int, tuple<int, int>> miniMaxTicTacToe(vector<vector<char>> &game_state, int depth, char current_player, int &count_to_win);
void print(vector<vector<char>> &game_state);

char *getInputTicTacToe(int search_depth, int count_to_win, int y_length, int x_length, char ai_marker, char opponent_marker, char *array_begin)
{
    AI_MARKER = ai_marker;
    OPPONENT_MARKER = opponent_marker;
    vector<vector<char>> game_state(y_length, vector<char>(x_length, ' '));
    for (int y = 0; y < y_length; y++)
    {
        for (int x = 0; x < x_length; x++)
        {
            char c = *(array_begin + y * x_length + x);
            if (c == '.')
            {
                game_state[y][x] = ' ';
            }
            else
            {
                game_state[y][x] = c;
            }
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
    for (int y = 0; y < game_state.size(); y++)
    {
        for (int x = 0; x < game_state[y].size(); x++)
        {
            char c = *(array_begin + y * x_length + x);
            if (c == '.')
            {
                game_state[y][x] = ' ';
            }
            else
            {
                game_state[y][x] = c;
            }
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

    if (current_player == AI_MARKER)
    {
        int max = -2147483648;
        tuple<int, int> maxFieldIndex = tuple<int, int>(-1, -1);
        for (int y = 0; y < game_state.size(); y++)
        {
            for (int x = 0; x < game_state[y].size(); x++)
            {
                if (game_state[y][x] == ' ')
                {
                    game_state[y][x] = current_player;
                    auto temp = miniMaxTicTacToe(game_state, depth - 1, togglePlayer(current_player), count_to_win);
                    if (get<0>(temp) > max)
                    {
                        max = get<0>(temp);
                        maxFieldIndex = tuple<int, int>(y, x);
                    }
                    game_state[y][x] = ' ';
                }
            }
        }
        return tuple<int, tuple<int, int>>(max, maxFieldIndex);
    }
    else
    {
        int min = 2147483647;
        tuple<int, int> minFieldIndex = tuple<int, int>(-1, -1);
        for (int y = 0; y < game_state.size(); y++)
        {
            for (int x = 0; x < game_state[y].size(); x++)
            {
                if (game_state[y][x] == ' ')
                {
                    game_state[y][x] = current_player;
                    auto temp = miniMaxTicTacToe(game_state, depth - 1, togglePlayer(current_player), count_to_win);
                    if (get<0>(temp) < min)
                    {
                        min = get<0>(temp);
                        minFieldIndex = tuple<int, int>(y, x);
                    }
                    game_state[y][x] = ' ';
                }
            }
        }
        return tuple<int, tuple<int, int>>(min, minFieldIndex);
    }
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
        return tuple<int, tuple<int, int>>(2, tuple<int, int>(0, -1));
    }

    if (current_player == AI_MARKER)
    {
        int max = -2147483648;
        tuple<int, int> maxFieldIndex = tuple<int, int>(-1, -1);
        for (int x = 0; x < game_state[0].size(); x++)
        {
            if (game_state[0][x] == ' ')
            {
                makeMove_connectFour(game_state, x, current_player);
                auto temp = miniMaxConnectFour(game_state, depth - 1, togglePlayer(current_player), count_to_win);
                if (get<0>(temp) > max)
                {
                    max = get<0>(temp);
                    maxFieldIndex = tuple<int, int>(0, x);
                }
                deleteMove_connectFour(game_state, x, current_player);
            }
        }
        return tuple<int, tuple<int, int>>(max, maxFieldIndex);
    }
    else
    {
        int min = 2147483647;
        tuple<int, int> minFieldIndex = tuple<int, int>(-1, -1);
        for (int x = 0; x < game_state[0].size(); x++)
        {
            if (game_state[0][x] == ' ')
            {
                makeMove_connectFour(game_state, x, current_player);
                auto temp = miniMaxConnectFour(game_state, depth - 1, togglePlayer(current_player), count_to_win);
                if (get<0>(temp) < min)
                {
                    min = get<0>(temp);
                    minFieldIndex = tuple<int, int>(0, x);
                }
                deleteMove_connectFour(game_state, x, current_player);
            }
        }
        return tuple<int, tuple<int, int>>(min, minFieldIndex);
    }
}

void getConsecutiveCount(char &possibleWinner, const char &c, int &consecutiveCount)
{
    if (c == ' ')
    {
        consecutiveCount = 0;
        possibleWinner = ' ';
    }
    else if (c == possibleWinner)
    {
        consecutiveCount++;
    }
    else
    {
        possibleWinner = c;
        consecutiveCount = 1;
    }
}

char checkForWin(const vector<vector<char>> &gameState, int &countToWin)
{
    int consecutiveCount = 0;
    char possibleWinner = ' ';
    // Horizontal check

    for (size_t y = 0; y < gameState.size(); y++)
    {
        consecutiveCount = 0;
        possibleWinner = ' ';
        for (size_t x = 0; x < gameState[y].size(); x++)
        {
            getConsecutiveCount(possibleWinner, gameState[y][x], consecutiveCount);
            if (consecutiveCount == countToWin && possibleWinner != ' ')
            {
                return possibleWinner;
            }
        }
    }

    // Vertical check
    for (size_t x = 0; x < gameState[0].size(); x++)
    {
        consecutiveCount = 0;
        possibleWinner = ' ';
        for (size_t y = 0; y < gameState.size(); y++)
        {
            getConsecutiveCount(possibleWinner, gameState[y][x], consecutiveCount);
            if (consecutiveCount == countToWin && possibleWinner != ' ')
            {
                return possibleWinner;
            }
        }
    }

    int rows = gameState.size();
    int cols = gameState[0].size();

    // Diagonal check /
    for (int col = 0; col < cols; ++col)
    {
        int y = 0;   // row
        int x = col; // column
        consecutiveCount = 0;
        possibleWinner = ' ';
        while (y < rows && x >= 0)
        {
            getConsecutiveCount(possibleWinner, gameState[y][x], consecutiveCount);
            if (consecutiveCount == countToWin && possibleWinner != ' ')
            {
                return possibleWinner;
            }
            y++;
            x--;
        }
    }

    // Diagonal check /
    for (int row = 1; row < rows; ++row)
    {
        int y = row;      // row
        int x = cols - 1; // column
        consecutiveCount = 0;
        possibleWinner = ' ';
        while (y < rows && x >= 0)
        {
            getConsecutiveCount(possibleWinner, gameState[y][x], consecutiveCount);
            if (consecutiveCount == countToWin && possibleWinner != ' ')
            {
                return possibleWinner;
            }
            y++;
            x--;
        }
    }

    // Diagonal check \ .
    for (int row = rows - 1; row >= 0; --row)
    {
        int y = row; // row
        int x = 0;   // column
        consecutiveCount = 0;
        possibleWinner = ' ';
        while (y < rows && x < cols)
        {
            getConsecutiveCount(possibleWinner, gameState[y][x], consecutiveCount);
            if (consecutiveCount == countToWin && possibleWinner != ' ')
            {
                return possibleWinner;
            }
            y++;
            x++;
        }
    }
    // Diagonal check \ .
    for (int col = 1; col < cols; ++col)
    {
        int y = 0;   // row
        int x = col; // column
        consecutiveCount = 0;
        possibleWinner = ' ';
        while (y < rows && x < cols)
        {
            getConsecutiveCount(possibleWinner, gameState[y][x], consecutiveCount);
            if (consecutiveCount == countToWin && possibleWinner != ' ')
            {
                return possibleWinner;
            }
            y++;
            x++;
        }
    }

    return ' ';
}
