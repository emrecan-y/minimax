import string
import pygame

from Functions import checkEvents

GRID_COLOR = (200, 200, 200)
gridSize = 3
searchDepth = 3
pygame.font.init()

class StartScreenButton:
  def __init__(self,text,action,width,yPos,xOffset):
    self.text = text
    self.action = action
    font = pygame.font.SysFont('Arial', 20)
    self.surface = font.render(text, [1,1], (0, 0, 0),GRID_COLOR)
    self.rect = self.surface.get_rect()
    size :tuple = self.surface.get_rect().size
    posX = (width/2 - size[1]/2) + xOffset
    posY = yPos
    self.rect.center = (posX,posY)
    

    
def checkStartScreenButtons(startScreenTextSurfaces):
    global gridSize, searchDepth
    running, mouseClicked = checkEvents()
    gameMode = ""
    mouse = pygame.mouse.get_pos()
    for startScreenButton in startScreenTextSurfaces:
        if startScreenButton.action == "":
            continue
        if startScreenButton.action == "show":
            startScreenButton.text = gridSize
        if startScreenButton.rect.collidepoint(mouse) and mouseClicked:
            mouseClicked = False
            if startScreenButton.action == "-G":
                if gridSize > 3:
                    gridSize -= 1
                break
            if startScreenButton.action == "+G":
                if gridSize < 21:
                    gridSize += 1
                break
            if startScreenButton.action == "-AI":
                if searchDepth > 0:
                    searchDepth -= 1
                break
            if startScreenButton.action == "+AI":
                if searchDepth < 10:
                    searchDepth += 1
                break
            gameMode = startScreenButton.action
            break
    return running, gameMode


def drawStartScreen(WIDTH,startScreenTextSurfaces):
    screen = pygame.display.get_surface()
    screen.fill("White")
    for startScreenButton in startScreenTextSurfaces:
        screen.blit(startScreenButton.surface,startScreenButton.rect)
    pygame.display.update()

def createStartScreenSurfaces(WIDTH):
    startScreenTextSurfaces = []
    startScreenTextSurfaces.append(StartScreenButton(" TicTacToe ","",WIDTH,100,0))
    startScreenTextSurfaces.append(StartScreenButton(" Single Player Python ","TTT_SINGLE_PLAYER_P",WIDTH,150,0))
    startScreenTextSurfaces.append(StartScreenButton(" Single Player C++ ","TTT_SINGLE_PLAYER_C",WIDTH,180,0))
    startScreenTextSurfaces.append(StartScreenButton("  Two Players  ","TTT_TWO_PLAYERS",WIDTH,210,0))
    
    startScreenTextSurfaces.append(StartScreenButton("    Grid Size    ","",WIDTH,240,0))
    startScreenTextSurfaces.append(StartScreenButton(" - ","-G",WIDTH,265,-25))
    startScreenTextSurfaces.append(StartScreenButton(str(gridSize),"show",WIDTH,265,0))
    startScreenTextSurfaces.append(StartScreenButton(" + ","+G",WIDTH,265,25)) 
    
    startScreenTextSurfaces.append(StartScreenButton(" Search Depth AI ","",WIDTH,300,0))
    startScreenTextSurfaces.append(StartScreenButton(" - ","-AI",WIDTH,325,-25)) 
    startScreenTextSurfaces.append(StartScreenButton(str(searchDepth),"show",WIDTH,325,0))
    startScreenTextSurfaces.append(StartScreenButton(" + ","+AI",WIDTH,325,25))
    
    startScreenTextSurfaces.append(StartScreenButton(" ConnectFour ","",WIDTH,450,0))
    startScreenTextSurfaces.append(StartScreenButton(" Single Player Python ","4G_SINGLE_PLAYER_P",WIDTH,500,0))
    startScreenTextSurfaces.append(StartScreenButton(" Single Player C++ ","4G_SINGLE_PLAYER_C",WIDTH,530,0))
    startScreenTextSurfaces.append(StartScreenButton("  Two Players  ","4G_TWO_PLAYERS",WIDTH,560,0))
    return startScreenTextSurfaces

def startScreenLoop(SIZE):
    (WIDTH, HEIGHT) = SIZE
    while True:
        startScreenTextSurfaces = createStartScreenSurfaces(WIDTH)
        drawStartScreen(WIDTH,startScreenTextSurfaces)
        running, gameMode = checkStartScreenButtons(startScreenTextSurfaces)
        if not running or gameMode != "":
            return running, gameMode, gridSize, searchDepth
