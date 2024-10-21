import subprocess
import pygame
from MiniMax import miniMax_connectFour
from Functions import checkEvents, makeMove_connectFour, togglePlayer_connectFour
from WinCheck import checkForWin

class GameField:
  def __init__(self):
    self.value = ' '
    self.rect = {}
    self.won = False

class VierGewinnt:
    GRID_COLOR = (200, 200, 200)
    GRID_WON_COLOR = (100, 255, 100)
    X_COLOR = (100, 100, 255)
    O_COLOR = (255, 100, 100) 

    def __init__(self,screen,GRID_COUNT,searchDepth):
        self.screen = screen
        (self.WIDTH, self.HEIGHT) = screen.get_size()

        self.COUNT_TO_WIN = 4
        (self.GRID_COUNT_X,self.GRID_COUNT_Y) = GRID_COUNT

        self.X_OFFSET_BETWEEN_BUTTONS = 5
        self.Y_OFFSET_BETWEEN_BUTTONS = 5

        self.GRID_BUTTON_SIZE = self.WIDTH/(self.GRID_COUNT_X+2) - self.X_OFFSET_BETWEEN_BUTTONS
        self.MOVE_SIZE = self.GRID_BUTTON_SIZE - (self.GRID_BUTTON_SIZE/10)

        self.X_OFFSET = (self.WIDTH - ((self.GRID_BUTTON_SIZE+self.X_OFFSET_BETWEEN_BUTTONS)*self.GRID_COUNT_X))/2 + self.GRID_BUTTON_SIZE/2
        self.Y_OFFSET = (self.HEIGHT - ((self.GRID_BUTTON_SIZE+self.Y_OFFSET_BETWEEN_BUTTONS)*self.GRID_COUNT_Y))/2

        self.STROKE_WIDTH = int( self.MOVE_SIZE / 10)
        self.SEARCH_DEPTH = searchDepth
        self.gameState = [[GameField() for x in range(self.GRID_COUNT_X)] for y in range(self.GRID_COUNT_Y+1)]

        self.currentPlayer = 'Y'
        self.winner = ' '
        self.moveCount = 0


    def draw(self):
        screen = pygame.display.get_surface()
        for y in range(len(self.gameState)):
            for x in range(len(self.gameState[y])):
                x_begin = self.X_OFFSET+(x*(self.GRID_BUTTON_SIZE+self.X_OFFSET_BETWEEN_BUTTONS))
                y_begin = self.Y_OFFSET+(y*(self.GRID_BUTTON_SIZE+self.Y_OFFSET_BETWEEN_BUTTONS))
                if y == 0:
                    self.gameState[y][x].rect = pygame.Rect(x_begin-self.GRID_BUTTON_SIZE/2, y_begin-self.GRID_BUTTON_SIZE/2, self.GRID_BUTTON_SIZE, self.GRID_BUTTON_SIZE)
                    continue
                if self.gameState[y][x].won:
                    pygame.draw.circle(screen, self.GRID_WON_COLOR, (x_begin,y_begin), self.GRID_BUTTON_SIZE/2+2,4)
                else:
                    pygame.draw.circle(screen, self.GRID_COLOR, (x_begin,y_begin), self.GRID_BUTTON_SIZE/2+2,4)

                if self.gameState[y][x].value == 'Y':
                    pygame.draw.circle(screen, "Yellow", (x_begin,y_begin), self.GRID_BUTTON_SIZE/2)
                    
                if self.gameState[y][x].value == 'R':
                    pygame.draw.circle(screen, "Red", (x_begin,y_begin), self.GRID_BUTTON_SIZE/2)
        pygame.display.update()  



    def updateGameState(self):
        mouse = pygame.mouse.get_pos()
        running,mouseClicked = checkEvents()
        for x in range(len(self.gameState[0])):
            if self.gameState[0][x].rect.collidepoint(mouse):
                x_begin = self.X_OFFSET+(x*(self.GRID_BUTTON_SIZE + self.X_OFFSET_BETWEEN_BUTTONS))
                y_begin = self.Y_OFFSET
                if self.currentPlayer == 'Y':
                    pygame.draw.circle(pygame.display.get_surface(), "Yellow", (x_begin,y_begin), self.GRID_BUTTON_SIZE/2)
                elif self.currentPlayer == 'R':
                    pygame.draw.circle(pygame.display.get_surface(), "Red", (x_begin,y_begin), self.GRID_BUTTON_SIZE/2)
                pygame.display.update() 
                if self.winner == ' ' and mouseClicked and makeMove_connectFour(self.gameState,x,self.currentPlayer):
                    self.moveCount += 1
                    self.currentPlayer = togglePlayer_connectFour(self.currentPlayer)
        return running



    def getAiInput(self):
        return miniMax_connectFour(self.gameState,self.SEARCH_DEPTH,self.currentPlayer,self.COUNT_TO_WIN)
    
    def getAiInputCPP(self):
        from ctypes import c_char ,c_int, c_char_p, cdll
        import ctypes
        mini_max = cdll.LoadLibrary('./cpp/mini_max.so')

        out = ""
        for y in range(1,len(self.gameState)):
            for c in self.gameState[y]:
                out+= c.value
        #(int search_depth, int count_to_win, int y, int x, char ai_marker, char opponent_marker, char *array_begin)
        mini_max.get_input_connectfour.argtypes = [c_int ,c_int, c_int, c_int, c_char, c_char, c_char_p]
        mini_max.get_input_connectfour.restype = c_char_p
        result = mini_max.get_input_connectfour(self.SEARCH_DEPTH,self.COUNT_TO_WIN,self.GRID_COUNT_Y,self.GRID_COUNT_X,c_char('Y'.encode('utf-8')),c_char('R'.encode('utf-8')),ctypes.c_char_p(out.encode('utf-8')))
        result = result.decode("utf_8")
        result = result.split("-")
        result =  (int(result[0]),int(result[1]))
        return result 
    

    def startGameLoop(self,gameMode):
        running = True
        while running:
            self.screen.fill("White")
            self.draw()
            if self.moveCount < self.GRID_COUNT_X * self.GRID_COUNT_Y and self.winner == ' ':
                if self.currentPlayer == 'Y' and gameMode == "4G_SINGLE_PLAYER_P":
                    input = self.getAiInput()
                    print(input)
                    makeMove_connectFour(self.gameState,input[1],self.currentPlayer)
                    self.moveCount += 1
                    self.currentPlayer = togglePlayer_connectFour(self.currentPlayer)
                if self.currentPlayer == 'Y' and gameMode == "4G_SINGLE_PLAYER_C":
                    input = self.getAiInputCPP()
                    print(input)
                    makeMove_connectFour(self.gameState,input[1],self.currentPlayer)
                    self.moveCount += 1
                    self.currentPlayer = togglePlayer_connectFour(self.currentPlayer)
                running = self.updateGameState()
                self.winner = checkForWin(self.gameState,True,self.COUNT_TO_WIN)
                continue
            running,mouseClicked = checkEvents()
            if mouseClicked:
                break
        return running

