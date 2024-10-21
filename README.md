# Python & C++ Games Project

This project was developed during a **Python crash course** to help familiarize myself with Python syntax, especially in comparison to my main programming language, Java.

The project is built using **Pygame** and includes two classic games:

- **Tic-Tac-Toe**
- **Connect Four**

## Game Modes

Both games support the following modes:

- **Single-player**: Play against the computer using the **Minimax algorithm**.
- **Two-player**: Local multiplayer mode for playing against another human.

### Single-player Mode

In the single-player mode, the **Minimax algorithm** is used for AI. The algorithm has been implemented twice:

1. **Python** implementation.
2. **C++** implementation (to minimize the execution time of the algorithm).

## C++ Integration

The C++ version of the **Minimax algorithm** is compiled as a shared library and used within the Python project for faster execution.

Additionally, an older implementation of the Minimax algorithm in C++ accepts inputs via command-line arguments and outputs results directly to the terminal (though slower);

```python
result = subprocess.run(args, capture_output=True, universal_newlines=True)
# Example: "./a.out 3 3 o x 3 3 ... .ox ..."
```

## Compiling the C++ Shared Library

To compile the C++ code as a shared library, run the following commands:

```bash
g++ -c -fPIC MiniMaxAsLibrary.cpp -o mini_max.o -std=c++17
g++ -shared -Wl,-install_name,mini_max.so -o mini_max.so mini_max.o
```

Once compiled, the shared library (mini_max.so) can be used within the Python project for faster AI decision-making in the games.
