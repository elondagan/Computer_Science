/*-------------------------------------------------------------------------
  Include files:
--------------------------------------------------------------------------*/
#include <stdio.h>
/*=========================================================================
  Constants and definitions:
==========================================================================*/
#define N 11
#define X 88
#define O 79
#define EVEN 2
#define GAME_OVER 3

/*=========================================================================
 function declaration
 ===========================================================================*/
void print_welcome();
void print_enter_board_size();
void print_board(char board[N][N], int n);
void print_player_trun(int player_index);
void print_error();
void print_winner(int player_index);
void print_tie();
/////////////////////////////////////////////////////
int set_game(int board_size);
int print_state(char board[N][N], int board_size, int moves_count, int loop_index);
int undo_requested(int r, int loop_index);    /*   , int moves_count */
int do_undo(int moves_hisotry[N][N], char board[N][N], int undo_choise, int moves_count);
int check_numbers(char board[N][N], int board_size, int r, int c, int loop_index, int moves_count);
void update_records(char board[N][N], int moves_history[N][N], int moves_count, int r, int c, int m);
int check_rows(char board[N][N], int board_size, int player_index);
int check_coulmns(char board[N][N], int board_size, int player_index);
int check_slants(char charboard[N][N], int board_size, int player_index);
int check_for_win(char board[N][N], int board_size, int player_index);
int check_for_tie(char board[N][N],int board_size, int r);
int check_game_status(char board[N][N], int board_size, int player_index, int r);

/*=============================================================================
 function implementation
 ==============================================================================*/

//print welcome massage
void print_welcome()
{
    printf("*** Welcome to AVOIDANCE TIC-TAC-TOE game ***\n\n");
}

// print massage to enter size
void print_enter_board_size()
{
    printf("Please enter board size (1 to %d):\n", N);
}

// print the board
void print_board(char board[N][N], int n)
{
    printf("\nCurrent board:\n");
    for(int i=0; i < n; i++)
    {
        printf("|");
        for(int j=0; j < n; j++)
        {
            if(board[i][j]==X || board[i][j]==O) printf("%c|", board[i][j]);
            else printf("_|");
        }
        printf("\n");
    }
    printf("\n");
}

// print a request for player with index "player_index" to move
void print_player_trun(int player_index)
{
    printf("\nPlayer ** %d **, enter next move:\n", player_index);
}

// print error message
void print_error()
{
    printf("Illegal move!!!, please try again:\n");
}

// print the winner
void print_winner(int player_index)
{
    printf("Player %d Wins! Hooray!\n", player_index);
}

// print a massage to announce there is a tie
void print_tie()
{
    printf("It's a tie!\n");
}
                                            /* other functions: */
// welcome players and get a board size
int set_game(int board_size)
{
    print_welcome();
    print_enter_board_size();
    if(scanf("%d", &board_size)!=1) return 1;

    return board_size;
}

// print current game state
int print_state(char board[N][N], int board_size, int moves_count, int loop_index)
{
    int m = loop_index;
    moves_count++;
    print_board(board, board_size);
    print_player_trun(m);
    return moves_count;
}

// check for undo request and its validity
int undo_requested(int r, int loop_index)
{
    if(r<0)
    {
        if(r%EVEN == 0)
        {
        print_error(); loop_index=0;
        }
        return loop_index;
    }
    return 0;
}

// deleting the requested number of moves
int do_undo(int moves_hisotry[N][N], char board[N][N], int undo_choise, int moves_count)
{
    for(int i=0; i <= N-1; i++){
        for(int j=0; j <= N-1; j++){
            if(moves_hisotry[i][j]>= moves_count+undo_choise){
                moves_hisotry[i][j] = 0;
                board[i][j] = 0;
            }
        }
    }
    return undo_choise;
}

// receive two numbers and check they validity
int check_numbers(char board[N][N], int board_size, int r, int c, int loop_index,  int moves_count  )
{
    if(r<0 && r%EVEN==0) return 0;
    if(r>=0)
    {
        if(r==0 || r > board_size || c <1 || c > board_size)
            {
                print_error(); return 0;
            }
        if(board[r-1][c-1]==X || board[r-1][c-1] ==O)
            {
                print_error(); return 0;
            }
    }
    if (r < 0 && moves_count <= (-1) * r)
    {
        print_error();
        return 0;
    }
    return loop_index;
}

// update the board and the moves history records
void update_records(char board[N][N], int moves_history[N][N], int moves_count, int r, int c, int m)
{
    if(r>0)
        {
            int player_index = m;
            moves_history[r-1][c-1] = moves_count;

            if(player_index==1) board[r-1][c-1] = X;
            else board[r-1][c-1] = O;
        }
}

// search for winning row
int check_rows(char board[N][N], int board_size, int player_index)
{
    int x_count=0, o_count=0;
    for(int i=0; i < board_size; i++){
        x_count=0, o_count=0;
        for(int k=0; k < board_size; k++){
            if(board[i][k]==X) x_count++;
            if(board[i][k]==O) o_count++;
            if(o_count==board_size || x_count==board_size) {
                    return player_index;
            }
        }
    }
    return 0;
}

// search for winning column
int check_coulmns(char board[N][N], int board_size, int player_index)
{
    int x_count=0, o_count=0;
    for(int i=0; i < board_size; i++){
        x_count=0, o_count=0;
        for(int k=0; k < board_size; k++){
            if(board[k][i]==X) x_count++;
            if(board[k][i]==O) o_count++;
            if(o_count==board_size || x_count==board_size) {
                    return player_index;
            }
        }
    }
    return 0;
}

// search for winning slant
int check_slants(char board[N][N], int board_size, int player_index)
{
        for(int o_count=0, x_count=0, k =0; k < board_size; k++){
            if(board[k][k] == X) x_count++;
            if(board[k][k] == O) o_count++;
            if(x_count == board_size || o_count == board_size){
                return player_index;
            }
        }
        for(int x_count=0, o_count=0, i=board_size-1, k= 0; i >= 0; i--, k++){
            if(board[i][k] == X) x_count++;
            if(board[i][k] == O) o_count++;
            if(x_count == board_size || o_count == board_size){
                return player_index;
            }
        }
        return 0;
}

// search for winning sequence
int check_for_win(char board[N][N], int board_size, int player_index)
{
        int i= player_index;
        if(check_rows(board, board_size, i) || check_coulmns(board, board_size, i) || check_slants(board, board_size, i) )
        {
            print_board(board, board_size);
            if(i==EVEN) print_winner(i-1); else print_winner(i+1); return 1;
        }
        return 0;
}

// check if there is a tie
int check_for_tie(char board[N][N],int board_size, int r)
{
    int count=0;
    for(int i=0; i < board_size; i++){
        for(int j=0; j < board_size; j++){
            if(board[i][j]==X || board[i][j]==O) count++;
            if(count == board_size*board_size && r>=0)
            {
                print_board(board, board_size);
                print_tie();    return 1;
            }
        }
    }
    return 0;
}

// check for win or tie
int check_game_status(char board[N][N], int board_size, int player_index, int r)
{
        if(check_for_win(board, board_size, player_index) || check_for_tie(board, board_size,r) ) return GAME_OVER;
        if (player_index == EVEN) player_index=0; return player_index;
        return player_index;
}

/*-------------------------------------------------------------------------
  The main program.
 -------------------------------------------------------------------------*/
int main()
{
    char board[N][N];
    int moves_history[N][N],board_size=0, moves_count=0, r=0, c=0;

    board_size=set_game(board_size);

    for(int m=1; m<3 ; m++)
    {
        moves_count=print_state(board, board_size, moves_count, m);

        for(int i=1; i<2 ; i++)
        {
            scanf("%d", &r); if (r>=0) scanf("%d", &c);

            i=check_numbers(board, board_size, r, c, i, moves_count);

            if (undo_requested(r, i))
            {
                moves_count += do_undo(moves_history, board, r, moves_count) - 1;
            }
        }
        update_records(board, moves_history, moves_count, r, c, m);

        m=check_game_status(board, board_size, m, r);
    }
    return 0;
}
