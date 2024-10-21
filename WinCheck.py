import numpy as np

def checkForWin(gameState,markOnMap,countToWin):
    winner = ' '
    #horizontal check
    if winner == ' ':
        winner = linearWinnerCheck(gameState,markOnMap,countToWin)
    #vertical check 
    if winner == ' ':
        winner = linearWinnerCheck(np.transpose(gameState),markOnMap,countToWin)
    #diagonal check
    if winner == ' ':
        winner = diagonalWinnerCheck(gameState,markOnMap,countToWin)
    if winner == ' ':
        winner = diagonalWinnerCheck(np.fliplr(gameState),markOnMap,countToWin)  
    return winner  

def linearWinnerCheck(gameState,markOnMap,countToWin): 
    for array in gameState:
        winner = arrayWinnerCheck(array,markOnMap,countToWin)
        if winner != ' ':
            return winner
    return ' '
        
def diagonalWinnerCheck(gameState,markOnMap,countToWin):
    for y in range(0-len(gameState),len(gameState)):
        diagArray = np.diagonal(gameState,y)
        if len(diagArray) < countToWin:
            continue
        winner = arrayWinnerCheck(diagArray,markOnMap,countToWin)
        if winner != ' ':
            return winner
    return ' '

def markWinner(array,endIndex,countToWin):
    endIndex += 1
    for i in range(endIndex-countToWin,endIndex):
        array[i].won = True

def arrayWinnerCheck(array,markOnMap,countToWin):
    consecutiveCount = 0
    possibleWinner = ' '
    for i in range(len(array)):
            if array[i].value == ' ':
                consecutiveCount = 0
                possibleWinner = ' '
            elif array[i].value == possibleWinner:
                consecutiveCount += 1
            else:
                possibleWinner =  array[i].value
                consecutiveCount = 1

            if consecutiveCount == countToWin:
                if markOnMap:
                    markWinner(array,i,countToWin)
                return possibleWinner
    return ' ' 

