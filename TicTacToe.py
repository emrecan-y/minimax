import subprocess
import pygame
from MiniMax import miniMax
from Functions import checkEvents, togglePlayer
from WinCheck import checkForWin

class GameField:
  def __init__(self):
    self.value = ' '
    self.rect = {}
    self.won = False

class TicTacToe:
    GRID_COLOR = (200, 200, 200)
    GRID_WON_COLOR = (100, 255, 100)
    X_COLOR = (100, 100, 255)
    O_COLOR = (255, 100, 100)

    def __init__(self,SIZE,GRID_COUNT, COUNT_TO_WIN, searchDepth):
        (self.WIDTH, self.HEIGHT) = SIZE

        self.GRID_COUNT = GRID_COUNT
        self.COUNT_TO_WIN = COUNT_TO_WIN

        self.X_OFFSET_BETWEEN_BUTTONS = 5
        self.Y_OFFSET_BETWEEN_BUTTONS = 5

        self.GRID_BUTTON_SIZE = self.HEIGHT/(GRID_COUNT+2) - self.Y_OFFSET_BETWEEN_BUTTONS
        self.MOVE_SIZE = self.GRID_BUTTON_SIZE - (self.GRID_BUTTON_SIZE/10)

        self.X_OFFSET = (self.WIDTH - ((self.GRID_BUTTON_SIZE+self.X_OFFSET_BETWEEN_BUTTONS)*self.GRID_COUNT))/2
        self.Y_OFFSET = (self.HEIGHT - ((self.GRID_BUTTON_SIZE+self.Y_OFFSET_BETWEEN_BUTTONS)*self.GRID_COUNT))/2

        self.STROKE_WIDTH = int( self.MOVE_SIZE / 10)
        self.SEARCH_DEPTH = searchDepth
        self.gameState = [[GameField() for x in range(self.GRID_COUNT)] for y in range(self.GRID_COUNT)]

        self.currentPlayer = 'o'
        self.winner = ' '
        self.moveCount = 0

    def draw(self):
        screen = pygame.display.get_surface()
        for y in range(len(self.gameState)):
            for x in range(len(self.gameState[y])):

                x_begin = self.X_OFFSET+(x*(self.GRID_BUTTON_SIZE+self.X_OFFSET_BETWEEN_BUTTONS))
                y_begin = self.Y_OFFSET+(y*(self.GRID_BUTTON_SIZE+self.Y_OFFSET_BETWEEN_BUTTONS))
                self.gameState[y][x].rect = pygame.Rect(x_begin, y_begin, self.GRID_BUTTON_SIZE, self.GRID_BUTTON_SIZE)

                if self.gameState[y][x].won:
                    pygame.draw.rect(screen, self.GRID_WON_COLOR, self.gameState[y][x].rect, 3)
                else:
                    pygame.draw.rect(screen, self.GRID_COLOR, self.gameState[y][x].rect, 2)

                if self.gameState[y][x].value == 'x':
                    x_begin_x = x_begin + (self.GRID_BUTTON_SIZE - self.MOVE_SIZE)/2
                    y_begin_x = y_begin + (self.GRID_BUTTON_SIZE - self.MOVE_SIZE)/2
                    pygame.draw.line(screen, self.X_COLOR, [x_begin_x, y_begin_x], [x_begin_x+self.MOVE_SIZE, y_begin_x+self.MOVE_SIZE],self.STROKE_WIDTH)
                    pygame.draw.line(screen, self.X_COLOR, [x_begin_x, y_begin_x + self.MOVE_SIZE], [x_begin_x + self.MOVE_SIZE, y_begin_x],self.STROKE_WIDTH)
                    
                if self.gameState[y][x].value == 'o':
                    pygame.draw.circle(screen,self.O_COLOR,(x_begin + self.GRID_BUTTON_SIZE / 2,y_begin + self.GRID_BUTTON_SIZE / 2),self.MOVE_SIZE / 2,self.STROKE_WIDTH)
                    
        pygame.display.update()  


    def updateGameState(self):
        mouse = pygame.mouse.get_pos()
        running,mouseClicked = checkEvents()
        for y in range(len(self.gameState)):
            for x in range(len(self.gameState[y])):
                if self.gameState[y][x].rect.collidepoint(mouse) and mouseClicked:
                    if self.winner == ' ' and self.gameState[y][x].value == ' ':
                        self.gameState[y][x].value = self.currentPlayer
                        self.moveCount += 1
                        self.currentPlayer = togglePlayer(self.currentPlayer)  
        return running



    def getAiInput(self):
        return miniMax(self.gameState,self.SEARCH_DEPTH,self.currentPlayer,self.COUNT_TO_WIN)
    
    def getAiInputCPP(self):
        from ctypes import c_char ,c_int, c_char_p, cdll
        import ctypes
        mini_max = cdll.LoadLibrary('./cpp/mini_max.so')

        out = ""
        for array in self.gameState:
            for c in array:    
                out+= c.value
        #(int search_depth, int count_to_win, int y, int x, char ai_marker, char opponent_marker, char *array_begin)
        mini_max.get_input_tictactoe.argtypes = [c_int ,c_int, c_int, c_int, c_char, c_char, c_char_p]
        mini_max.get_input_tictactoe.restype = c_char_p
        result = mini_max.get_input_tictactoe(self.SEARCH_DEPTH,self.COUNT_TO_WIN,self.GRID_COUNT,self.GRID_COUNT,c_char('o'.encode('utf-8')),c_char('x'.encode('utf-8')),ctypes.c_char_p(out.encode('utf-8')))
        result = result.decode("utf_8")
        print(result)
        result = result.split("-")
        result =  (int(result[0]),int(result[1]))
        return result 
                
        """
        args = ["./cpp/a.out",str(searchDepth),str(self.COUNT_TO_WIN),"o","x",str(self.GRID_COUNT),str(self.GRID_COUNT)]
        for y in range(len(self.gameState)):
            line = ""
            for x in range(len(self.gameState[y])):
                if self.gameState[y][x].value == ' ':
                    line += '.'
                else:
                    line += self.gameState[y][x].value
            args.append(line)
        result = subprocess.run(args, capture_output=True, universal_newlines=True)
        result = result.stdout.strip()
        result = result.split(" ")
        result =  (int(result[0]),int(result[1]))
        return result
        """
        
  

    def startGameLoop(self,gameMode):
        running = True
        while running:
            self.draw()
            if self.moveCount < self.GRID_COUNT * self.GRID_COUNT and self.winner == ' ':
                if self.currentPlayer == 'o' and gameMode == "TTT_SINGLE_PLAYER_P":
                    input = self.getAiInput()
                    print(input)
                    input[1].value = self.currentPlayer
                    self.moveCount += 1
                    self.currentPlayer = togglePlayer(self.currentPlayer)
                if self.currentPlayer == 'o' and gameMode == "TTT_SINGLE_PLAYER_C":
                    input = self.getAiInputCPP()
                    print(input)
                    self.gameState[input[0]][input[1]].value = self.currentPlayer
                    self.moveCount += 1
                    self.currentPlayer = togglePlayer(self.currentPlayer)
                running =self.updateGameState()
                self.winner = checkForWin(self.gameState,True,self.COUNT_TO_WIN)
                continue
            running,mouseClicked = checkEvents()
            if mouseClicked:
                break
        return running

