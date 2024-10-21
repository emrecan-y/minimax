from ctypes import c_char ,c_int, c_char_p, cdll
import ctypes
mini_max = cdll.LoadLibrary('./mini_max.so')




arr2d3 = [[' ' for x in range(3)] for y in range(3)]
arr2d6 = [['.' for x in range(6)] for y in range(6)]

out = ""
for array in arr2d3:
    for c in array:
        out+= c
#(int search_depth, int count_to_win, int y, int x, char ai_marker, char opponent_marker, char *array_begin)
mini_max.getInput.argtypes = [c_int ,c_int, c_int, c_int, c_char, c_char, c_char_p]
mini_max.getInput(3,3,3,3,c_char('o'.encode('utf-8')),c_char('x'.encode('utf-8')),ctypes.c_char_p(out.encode('utf-8')))

