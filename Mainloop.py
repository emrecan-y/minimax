import subprocess
import datetime
import pygame
from StartScreen import startScreenLoop
from TicTacToe import TicTacToe
from VierGewinnt import VierGewinnt



SIZE= (700, 700)
(WIDTH, HEIGHT) = SIZE

background_colour = (255,255,255)
screen = pygame.display.set_mode((WIDTH, HEIGHT))

pygame.display.set_caption('TicTacToe')
screen.fill(background_colour)
pygame.display.flip()


running = True

while running:
    running, gameMode, gridSize, searchDepth = startScreenLoop(SIZE)
    if not running:
        break
    screen.fill("White")
    #startGameLoop(SIZE,gameMode)
    if "TTT" in gameMode:
        running = TicTacToe(SIZE,gridSize,3,searchDepth).startGameLoop(gameMode)
    elif "4G" in gameMode:
        running = VierGewinnt(screen,(7,6),searchDepth).startGameLoop(gameMode)
    screen.fill("White")
    
   
    
                

  
    