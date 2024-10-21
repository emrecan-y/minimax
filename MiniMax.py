import random
from WinCheck import checkForWin
from Functions import deleteMove_connectFour, makeMove_connectFour, togglePlayer, togglePlayer_connectFour



def miniMax(gameState,depth,currentPlayer,countToWin):
    if depth < 0:
        return (0,{})
    winner = checkForWin(gameState,False,countToWin)
    if winner == 'x':
        return (-1,{})
    elif winner == 'o':
        return (2,{})
    
    if currentPlayer == 'o':
        max = -2147483648
        maxField = {}
        for array in gameState:
            for gameField in array:
                if gameField.value == ' ':
                    gameField.value = currentPlayer
                    temp = miniMax(gameState,depth-1,togglePlayer(currentPlayer),countToWin)
                    if temp[0] > max:
                        max = temp[0]
                        maxField = gameField
                    gameField.value = ' '
        return (max,maxField)
    else:
        min = 2147483647
        minField = {}
        for array in gameState:
            for gameField in array:
                if gameField.value == ' ':
                    gameField.value = currentPlayer
                    temp = miniMax(gameState,depth-1,togglePlayer(currentPlayer),countToWin)
                    if temp[0] < min:
                        min = temp[0]
                        minField = gameField
                    gameField.value = ' '
        return (min,minField)
    
def miniMax_connectFour(gameState,depth,currentPlayer,countToWin):
    if depth < 0:
        return (0,{})
    
    winner = checkForWin(gameState,False,countToWin)
    if winner == 'R':
        return (-1,{})
    elif winner == 'Y':
        return (2,{})
    
    if currentPlayer == 'Y':
        max = -2147483648
        bestMove = -1
        for index in range(len(gameState[0])):
            if gameState[0][index].value == ' ':
                makeMove_connectFour(gameState, index, currentPlayer)
                temp = miniMax_connectFour(gameState,depth-1,togglePlayer_connectFour(currentPlayer),countToWin)
                if temp[0] > max:
                    max = temp[0]
                    bestMove = index
                deleteMove_connectFour(gameState, index, currentPlayer)  
        return (max,bestMove)
    else:
        min = 2147483647
        bestMove = -1
        for index in range(len(gameState[0])):
            if gameState[0][index].value == ' ':
                makeMove_connectFour(gameState, index, currentPlayer)
                temp = miniMax_connectFour(gameState,depth-1,togglePlayer_connectFour(currentPlayer),countToWin)
                if temp[0] < min:
                    min = temp[0]
                    bestMove = index
                deleteMove_connectFour(gameState, index, currentPlayer)  
        return (min,bestMove)
