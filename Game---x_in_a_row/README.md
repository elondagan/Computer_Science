# GAME---x_in_a_row (2020)
Extension of the game "4 in a row", with custom board size and unlimited 'undo' moves 

Settings:
- numbers of rows: 3-9
- number of columns: 3-9
- number of tokens: 3-min(number of rows, number of columns)

In a player turn:
- '-1' : undo move - remove the rival's last token, and let him the option to play again
- 1 - number of rows : choose a column to drop a column 

End of the game:
game is ended when either: 
- a player reach a row/column/diagonal of tokens in the 'number of tokens' input lenght 
- the the board is full  
