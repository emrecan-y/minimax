import pygame

def togglePlayer(currentPlayer):
    if currentPlayer == 'x':
        return 'o'
    else:
        return  'x'

def togglePlayer_connectFour(currentPlayer):
    if currentPlayer == 'R':
        return 'Y'
    else:
        return 'R'

def makeMove_connectFour(gameState, index, currentPlayer):
    for y in range(len(gameState)-1,-1,-1):
        if gameState[y][index].value == ' ':
            gameState[y][index].value = currentPlayer
            return True
    return False
            


def deleteMove_connectFour(gameState, index, currentPlayer):
    for y in range(len(gameState)):
        if gameState[y][index].value != ' ':
            gameState[y][index].value = ' '
            return True
    return False
            

def checkEvents():
    running = True
    mouseClicked = False
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.MOUSEBUTTONDOWN:
            mouseClicked = True
        #if event.type == pygame.MOUSEBUTTONUP:
            #mouseClicked = False
    return running,mouseClicked
