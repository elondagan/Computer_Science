/*-------------------------------------------------------------------------
  Include files:
--------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>
/*=========================================================================
  Constants and definitions:
==========================================================================*/
#define BOARD_MAX_SIDE 9
#define BOARD_MIN_SIDE 3
#define MIN_TOKENS 3
#define ILLEGAL -3
#define UNDO -1
#define REDO -2

#define ST_PLAYER 1
#define SEC_PLAYER 2

#define RED_SLOT_SYMBOL ('R')
#define YELLOW_SLOT_SYMBOL ('Y')
#define EMPTY_SLOT_SYMBOL (' ')
#define BLANK ('a')
//----------------------- Message Identifiers --------------------------//
#define MSG_GET_BOARD_ROWS   0
#define MSG_GET_BOARD_COLS   1
#define MSG_GET_NUMBER_TOKENS 2
//--------------------------- Board Edges ------------------------------//
#define BOARD_VERT_SEP  '|'
#define BOARD_LEFT_ANG '\\'
#define BOARD_RIGHT_ANG '/'
#define BOARD_BOTTOM  '-'
#define BOARD_BOTTOM_SEP '-'
//----------------------------------------------------------------------//
void print_welcome_message();
void print_read_game_params_message(int param);
void print_chose_color_message();
void print_chose_move_message(int player);
void print_enter_column_message();
void print_full_column_message();
void print_unavailable_undo_redo_message();
void print_board(char board[][BOARD_MAX_SIDE], int board_side[]);
void print_winner(int player_id);

void get_input(char* st_player_color, int* rows, int* columns, int* tokens, char* error);
void set_boards(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int board_side[2], int rows, int columns);
void set_game(char* st_player_color, int* rows, int* columns, int* tokens, char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int board_side[2],
               char* error);
bool its_a_tie(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int rows, int columns, int* player);
bool check_top_bottem_diag(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int rows, int columns, int row_index, int col_index, int tokens);
bool check_bottom_top_diag(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int rows, int columns, int row_index, int col_index, int tokens);
bool check_diag(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int rows, int columns, int row_index, int col_index, int tokens);
bool check_col(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int rows, int row_index, int col_index, int tokens);
bool check_row(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int columns, int row_index, int col_index, int tokens);
int there_is_a_win(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int rows, int columns, int tokens);
bool game_over(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int rows, int columns, int tokens, int* player);
void play_game(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int board_side[], int rows, int columns, int tokens, char st_player_color,
                char* error);
void next_move(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int history_board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int board_side[]
               ,int rows, int columns,
               int *column_input, int player, char st_player_color, int* game_counter, int* undo_seq, char* error);
void check_legal_play(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int rows,
                      int columns, int* column_input, int undo_seq, char* error);
bool undo_is_possible(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int rows, int columns, int column_input);
bool redo_is_possible(int undo_seq, int column_input);
int is_full(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int column);
void do_undo(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int board_side[], int history_board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],
              int* undo_seq, int* game_counter);
void do_redo(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int board_side[], int history_board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],
             int* undo_seq, int* game_counter, int player, char st_player_color);
void set_max_indexes(int mat[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int* row_index, int* col_index);
void fill_board(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int board_side[], int rows, int column_input,
                int* player, char st_player_color);
void insert_token(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int column_input, int rows, int player, char st_player_color);
void update_history(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int history_board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],
                     int column_input, int rows, int* game_counter, int* undo_seq);
void set_value_indexes(int mat[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int* row_index, int* col_index, int value);
int find_mat_max(int mat[BOARD_MAX_SIDE][BOARD_MAX_SIDE]);
int find_min(int x, int y);

void get_color(char* st_player_color);
void get_rows_cols(int* rows, int* columns, char* error);
void get_tokens(int* rows, int* columns, int* tokens, char* error);
void do_move(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int history_board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int board_side[],
             int rows,int *column_input, int player, char st_player_color, int* game_counter,int* undo_seq, bool* undo_or_redo_turn);
int current_player(int player);

int check_input(int* column_input, int rows, int columns,int undo_seq, char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE]);


//getting integer input and make sure the scan succeeds, other wise updating error variable that error occurred
void int_scan_and_check(int* input_variable, char* error)
{
    if(scanf("%d", input_variable)!=1){
        *error = true;
    }
}

//find the max value in given matrix
int find_mat_max(int mat[BOARD_MAX_SIDE][BOARD_MAX_SIDE])
{
    int max=0;
    for(int i=0; i<BOARD_MAX_SIDE; i++){
        for(int j=0; j<BOARD_MAX_SIDE; j++){
            if(mat[i][j]>max){
                max = mat[i][j];
            }
        }
    }
    return max;
}

//returning the min between to given values
int find_min(int x, int y)
{
    if(x<y){
        return x;
    }
    return y;
}

//setting starting condition of the game
void set_game(char* st_player_color, int* rows, int* columns, int* tokens, char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],
              int board_side[2], char* error)
{
    print_welcome_message();
    get_input(st_player_color, rows, columns, tokens, error);
    if(*error==true){   //if getting input fails we returning to the main
        return;
    }
    set_boards(board, board_side, *rows, *columns);
    print_board(board, board_side);
}

void get_input(char* st_player_color, int* rows, int* columns, int* tokens, char* error)
{
    get_color(st_player_color);
    get_rows_cols(rows, columns, error);
    if(*error==true){   //if getting input fails we returning to the main
        return;
    }
    get_tokens(rows, columns, tokens, error);
    if(*error==true){   //if getting input fails we returning to the main
        return;
    }
}

void get_color(char* st_player_color)
{
    while(*st_player_color!=YELLOW_SLOT_SYMBOL && *st_player_color!=RED_SLOT_SYMBOL){
      print_chose_color_message();
      scanf(" %c", st_player_color);
    }
}

void get_rows_cols(int* rows, int* columns, char* error)
{
    while(*rows > BOARD_MAX_SIDE || *rows < BOARD_MIN_SIDE){
        print_read_game_params_message(MSG_GET_BOARD_ROWS);
        int_scan_and_check(rows, error);
        if(*error==true){
            return;
        }
    }
    while(*columns > BOARD_MAX_SIDE || *columns < BOARD_MIN_SIDE || *columns < *rows ){
        print_read_game_params_message(MSG_GET_BOARD_COLS);
        int_scan_and_check(columns, error);
        if(*error==true){
            return;
        }
    }
}

void get_tokens(int* rows, int* columns, int* tokens, char* error)
{
    while(*tokens > find_min(*rows,*columns) || *tokens < MIN_TOKENS ){
        print_read_game_params_message(MSG_GET_NUMBER_TOKENS);
        int_scan_and_check(tokens, error);
        if(*error==true){
            return;
        }
    }
}

void set_boards(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int board_side[2], int rows, int columns)
{
    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++){
            board[i][j]=EMPTY_SLOT_SYMBOL;
        }
    }
    board_side[0] = rows;
    board_side[1] = columns;
}

//checking if the game is over by a win or a draw
bool game_over(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int rows, int columns, int tokens, int* player)
{
    bool some_one_won = there_is_a_win(board, rows, columns, tokens);
    bool tie = its_a_tie(board, rows, columns, player);
    return (some_one_won || tie);
}

bool its_a_tie(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int rows, int columns, int* player)
{
    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++){
            if(board[i][j]==EMPTY_SLOT_SYMBOL) return false;
        }
    }
    *player = ILLEGAL;
    return true;
}

int there_is_a_win(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int rows, int columns, int tokens)
{
    bool found_row=false, found_col=false, found_diag=false;

    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++)
            {
                if(board[i][j]==EMPTY_SLOT_SYMBOL){
                    continue;
                }
                found_row = check_row(board, columns, i, j, tokens);
                found_col = check_col(board, rows, i, j, tokens);
                found_diag = check_diag(board,rows, columns, i, j, tokens);
                if(found_row || found_col ||found_diag){
                    return true;
                }
            }
        }
    return false;
}

//checking for winning row
bool check_row(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int columns, int row_index, int col_index, int tokens)
{
    int right_counter=0, left_counter=0, j=col_index;
    char current = board[row_index][col_index];

    //check right side
    while(++j<columns)
    {
        if(board[row_index][j]!=current){
            break;
        }
        right_counter++;
    }
    //check left side
    j=col_index;
    while(--j >= 0)
    {
        if(board[row_index][j]!=current){
            break;
        }
        left_counter++;
    }
    return(left_counter+right_counter==tokens-1);
}

//checking for winning column
bool check_col(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int rows, int row_index, int col_index, int tokens)
{
    int down_counter=0, up_counter=0, i=row_index;
    char current = board[row_index][col_index];

    //check down side
    while(++i<rows)
    {
        if(board[i][col_index]!=current){
            break;
        }
        down_counter++;
    }
    //check up side
    i=row_index;
    while(--i >= 0)
    {
        if(board[i][col_index]!=current){
            break;
        }
        up_counter++;
    }
    return(down_counter + up_counter == tokens-1);
}

//checking for winning diag
bool check_diag(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int rows, int columns, int row_index, int col_index, int tokens)
{
    return(check_top_bottem_diag(board, rows, columns, row_index, col_index, tokens)
            || check_bottom_top_diag(board, rows, columns, row_index, col_index, tokens));
}

//helping function for "checking diag" - diag option 1
bool check_top_bottem_diag(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int rows, int columns, int row_index, int col_index, int tokens)
{
    int down_counter=0, up_counter=0, i=row_index, j=col_index;
    char current = board[row_index][col_index];

    //check down side
    while(++i < rows && ++j < columns)
    {
        if(board[i][j]!=current){
            break;
        }
        up_counter++;
    }
    //check up side
    i=row_index, j=col_index;
    while(--i >=0  && --j >=0)
    {
        if(board[i][j]!=current){
            break;
        }
        down_counter++;
    }
    return(down_counter + up_counter == tokens-1);
}

//helping function for "checking diag" - diag option 2
bool check_bottom_top_diag(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int rows, int columns, int row_index, int col_index, int tokens)
{
    int down_counter=0, up_counter=0, i=row_index, j=col_index;
    char current = board[row_index][col_index];

    //check up side
    while(--i >= 0 && ++j < columns )
    {
        if(board[i][j]!=current){
            break;
        }
        up_counter++;
    }
    //check down side
    i=row_index, j=col_index;
    while(++i < rows && --j < columns)
    {
        if(board[i][j]!=current){
            break;
        }
        down_counter++;
    }
    return(down_counter + up_counter == tokens-1);
}

//as long the game is not over keep handling each turn
void play_game(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int board_side[], int rows, int columns, int tokens,char st_player_color, char* error)
{
    int player = ST_PLAYER, column_input=ILLEGAL, game_counter =0,undo_seq=0;
    int history_board [BOARD_MAX_SIDE][BOARD_MAX_SIDE] = {{0}};

    while(game_over(board, rows, columns, tokens, &player)==false)
    {
        next_move(board, history_board, board_side, rows, columns, &column_input, player, st_player_color, &game_counter, &undo_seq, error);
        if(*error==true){
            return;
        }
        if(player==ST_PLAYER){
            player++;
        }
        else{
            player=ST_PLAYER;
        }
    }
    print_winner(current_player(player));
}

//helping function to determine who's player turn it is
int current_player(int player)
{
    if(player==ST_PLAYER){
        player=SEC_PLAYER;
    }
    else if(player==SEC_PLAYER){
        player=ST_PLAYER;
    }
    else{
        player=ILLEGAL;
    }
    return player;
}

//playing a single move in the game
void next_move(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int history_board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int board_side[],int rows,
               int columns,int *column_input, int player, char st_player_color, int* game_counter, int* undo_seq, char* error)
{
    bool undo_or_redo_turn = false;
    print_chose_move_message(player);
    print_enter_column_message();
    check_legal_play(board, rows, columns, column_input, *undo_seq, error);
    if(*error==true){
        return;
    }
    do_move(board, history_board, board_side, rows, column_input, player, st_player_color, game_counter, undo_seq, &undo_or_redo_turn);
    *column_input=ILLEGAL;
}

//handling each move according to the player decision
void do_move(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int history_board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int board_side[],
             int rows,int *column_input, int player, char st_player_color, int* game_counter, int* undo_seq, bool* undo_or_redo_turn)
{
    if(*column_input==UNDO)    // if we got here that means undo and redo are possible
    {
        do_undo(board,board_side, history_board, undo_seq, game_counter);
        *undo_or_redo_turn = true;
    }
    if(*column_input==REDO && *undo_or_redo_turn==false)
    {
        do_redo(board,board_side ,history_board, undo_seq,game_counter, player, st_player_color);
        *undo_or_redo_turn = true;
    }
    if(*undo_or_redo_turn==false)
    {
        fill_board(board, board_side, rows, *column_input, &player, st_player_color);
        update_history(board, history_board, *column_input, rows, game_counter, undo_seq);
    }
}

void check_legal_play(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],
                      int rows, int columns, int* column_input,int undo_seq, char* error)
{
    bool played = false;
    while(played==false)
    {
        int_scan_and_check(column_input, error);
        if(*error==true){
            return;
        }
        played=true;

        if(check_input(column_input, rows, columns, undo_seq, board))
        {
            print_enter_column_message();
            played=false;
        }
        if(is_full(board, *column_input) && played == true && *column_input!=UNDO && *column_input!=REDO)
        {
            print_full_column_message();
            played=false;
        }
     }
}

// check if the input is legal whether its undo redo or regular play
int check_input(int* column_input, int rows, int columns,int undo_seq, char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE])
{
    return(*column_input>columns) || ((*column_input<=0 && *column_input!=UNDO) && (*column_input<=0 && *column_input!=REDO))||
           (*column_input==UNDO && undo_is_possible(board, rows, columns, *column_input)==false)||
           (*column_input==REDO && redo_is_possible(undo_seq, *column_input)==false);
}

bool undo_is_possible(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int rows, int columns, int column_input)
{
    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++){
            if(board[i][j]!= EMPTY_SLOT_SYMBOL){
                return true;
            }
        }
    }
    if(column_input==UNDO){
        print_unavailable_undo_redo_message();
    }
    return false;
}

bool redo_is_possible(int undo_seq, int column_input)
{   if(column_input==REDO && undo_seq<=0){
    print_unavailable_undo_redo_message();
    }
    return (undo_seq>0);
}

// check if the column is full
int is_full(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int column)
{
    return(board[0][column-1]!=EMPTY_SLOT_SYMBOL);
}

void do_undo(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int board_side[], int history_board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int* undo_seq
             ,int* game_counter)
{
    *undo_seq = *undo_seq+1;
    *game_counter = *game_counter -1;
    int i=0, j=0;
    set_value_indexes(history_board, &i, &j, find_mat_max(history_board)- *undo_seq+1);
    board[i][j]=EMPTY_SLOT_SYMBOL;
    print_board(board, board_side);

}

void do_redo(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int board_side[], int history_board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int* undo_seq,
             int* game_counter, int player, char st_player_color)//
{

    int i=0, j=0;
    set_value_indexes(history_board, &i, &j, find_mat_max(history_board)- *undo_seq+1);
    if(player==ST_PLAYER){
        board[i][j]= st_player_color;
    }
    else{
        if(st_player_color==RED_SLOT_SYMBOL){
            board[i][j]= YELLOW_SLOT_SYMBOL;
        }
        else{
            board[i][j]= RED_SLOT_SYMBOL;
        }
    }
    print_board(board, board_side);
    *game_counter = *game_counter+1;
    *undo_seq = *undo_seq-1;
}

// setting given indexes addresses to the indexes of the max value in a matrix
void set_max_indexes(int mat[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int* row_index, int* col_index)
{
    int max=mat[0][0];
    for(int i=0; i< BOARD_MAX_SIDE; i++){
        for(int j=0; j < BOARD_MAX_SIDE; j++){
            if(mat[i][j]>max){
                max=mat[i][j];
                *row_index = i;
                *col_index = j;
            }
        }
    }
}

// add the last move to the board
void fill_board(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],int board_side[], int rows, int column_input,
                int* player, char st_player_color)
{
    insert_token(board, column_input, rows, *player, st_player_color);
    print_board(board, board_side);
}

//helping function for "fill board"
void insert_token(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int column_input, int rows, int player, char st_player_color)
{
    char symbol=st_player_color;
    if(player==SEC_PLAYER){
        if(st_player_color==RED_SLOT_SYMBOL){
            symbol=YELLOW_SLOT_SYMBOL;
        }
        else{
            symbol=RED_SLOT_SYMBOL;
        }
    }
    int i=1;
    while(rows-i>=0){
        if(board[rows-i][column_input-1]==EMPTY_SLOT_SYMBOL){
            board[rows-i][column_input-1]= symbol;
            return;
        }
        i++;
    }
}

// updating the history matrix of moves
void update_history(char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int history_board[BOARD_MAX_SIDE][BOARD_MAX_SIDE],
                    int column_input, int rows, int* game_counter,int* undo_seq)
{
    while(*undo_seq>0)
    {
        int i=0, j=0;
        set_value_indexes(history_board, &i, &j, find_mat_max(history_board)-*undo_seq+1);
        history_board[i][j]=0;
        *undo_seq = *undo_seq-1;
    }
    *game_counter = *game_counter+1;
    int i=0;
    while(i<rows)
    {
        if(board[i][column_input-1]!=EMPTY_SLOT_SYMBOL){
            history_board[i][column_input-1] = *game_counter;
            break;
        }
        i++;
    }
}

// setting the given indexes addresses to those that hold the given value in matrix if exists
void set_value_indexes(int mat[BOARD_MAX_SIDE][BOARD_MAX_SIDE], int* row_index, int* col_index, int value)
{

    for(int i=0; i<BOARD_MAX_SIDE; i++){
        for(int j=0; j<BOARD_MAX_SIDE; j++){
            if(mat[i][j]==value){
                *row_index=i;
                *col_index=j;
            }
        }
    }
}

/*-------------------------------------------------------------------------
  The main program.
 -------------------------------------------------------------------------*/
int main()
{
    char st_player_color = BLANK;
    int rows=0, columns=0, tokens=0;
    char board[BOARD_MAX_SIDE][BOARD_MAX_SIDE];
    int board_side[2];
    char error = false;

    set_game(&st_player_color, &rows, &columns, &tokens, board, board_side, &error);
    if(error==true){        //if error variable changed during setting the game than we exit
        return 1;
    }
    play_game(board, board_side, rows, columns, tokens, st_player_color, &error);
    if(error==true){    ////if error variable changed during  game than we return to here when it happens
        return 1;
    }

  return 0;
}


//---------------------- Printing Functions ----------------------------//

// message number 0 = "Welcome to 4-in-a-row game! \n"
void print_welcome_message()
{
    printf("Welcome to 4-in-a-row game! \n");
}


// message number 1 = "Please enter number of rows:"
// message number 2 = "Please enter number of columns:"
// message number 3 = "Please enter number of tokens:"
void print_read_game_params_message(int param)
{
	char const* const possible_params[] = {"rows", "columns", "tokens"};

    printf("Please enter number of %s: ", possible_params[param]);
}

// message number 4 = "Please choose starting color (Y)ellow or (R)ed: "
void print_chose_color_message()
{
    printf("Please choose starting color (Y)ellow or (R)ed: ");
}

// message number 5 = "Your move, player <player>. "
void print_chose_move_message(int player)
{
    printf("Your move, player %d. ", player);
}

// message number 6
void print_enter_column_message()
{
    printf("Please enter column: ");
}

void print_full_column_message()
{
    printf("Column full. ");
}

void print_unavailable_undo_redo_message()
{
    printf("No moves to undo/redo. ");
}

void print_board(char board[][BOARD_MAX_SIDE], int board_side[])
{

    for (int row = 0; row < board_side[0]; ++row)
    {
        printf("\n%c", BOARD_VERT_SEP);
        for (int col = 0; col < board_side[1]; ++col)
            printf("%c%c", board[row][col], BOARD_VERT_SEP);
    }
    printf("\n%c", BOARD_LEFT_ANG);
    for (int ii = 0; ii < board_side[1] - 1; ++ii)
        printf("%c%c", BOARD_BOTTOM, BOARD_BOTTOM_SEP);
    printf("%c%c\n", BOARD_BOTTOM, BOARD_RIGHT_ANG);
}

void print_winner(int player_id)
{
    if (player_id >= 0)
        printf("Player %d won! \n", player_id);
    else
        printf("That's a tie. \n");
}
