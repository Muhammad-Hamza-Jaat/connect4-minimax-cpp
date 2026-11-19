#include<iostream>
using namespace std;

void display(char board[6][7])
{
    for (int r = 0; r < 6; r++)
    {
        cout << "| ";
        for (int c = 0; c < 7; c++)
            cout << board[r][c] << " ";
        cout << "|" << endl;
    }
    cout << "+---------------+" << endl;
    cout << "  0 1 2 3 4 5 6  " << endl;
}
bool playable(char board[6][7], int c)
{
    if(c<0 || c>6) return false;

    return (board[0][c]=='.');//if the top row is empty, the col is not full

}

//drop into the lowest EMPTY row of 'col', return row index
int drop(char board[6][7], int col, char piece)
{
    int r;
    for (r=5; r>=0; r--) //go up from lowest row
        if (board[r][col]=='.')
        {
            board[r][col] = piece;
            break;
        }

    return r;
}

//check if the entire board is full(draw)
bool full_board(char board[6][7])
{
    for (int c = 0; c < 7; c++)
        if (board[0][c]=='.') //if even 1 col is non-empty, draw did not occur
            return false;
        
    return true;
}

//check for a win
bool win(char board[6][7], char p)
{
    //horizontal (c+3<7 => c<4)
    for(int r=0; r<6; r++)
        for(int c=0; c<4; c++)
            if(board[r][c]==p && board[r][c+1]==p && board[r][c+2]==p && board[r][c+3]==p)
                return true;
    //vertical (r+3<6 => r<3)
    for(int r=0; r<3; r++)
        for(int c=0; c<7; c++)
            if(board[r][c]==p && board[r+1][c]==p && board[r+2][c]==p && board[r+3][c]==p)
                return true;
    //primary diagnol
    for(int r=0; r<3; r++)
        for(int c=0; c<4; c++)
            if(board[r][c]==p && board[r+1][c+1]==p && board[r+2][c+2]==p && board[r+3][c+3]==p)
                return true;
    //secondary diagnol (c-3>=0 => c>=3)
    for(int r=0; r<3; r++)
        for(int c=3; c<7; c++)
            if(board[r][c]==p && board[r+1][c-1]==p && board[r+2][c-2]==p && board[r+3][c-3]==p)
                return true;

    return false;
}

//Scoring (from the computer’s point of view)
int scoring(char a, char b, char c, char d)
{
    int comp=0, human=0, empty=0;
    if(a=='O') comp++;
    else if(a=='X') human++;
    else empty++;
    
    if(b=='O') comp++;
    else if(b=='X') human++;
    else empty++;

    if(c=='O') comp++;
    else if(c=='X') human++;
    else empty++;

    if(d=='O') comp++;
    else if(d=='X') human++;
    else empty++;

    if(comp==3 && empty==1) return 5;
    if(comp==2 && empty==2) return 2;
    if(human==3 && empty==1) return -4;
    return 0;


}

//if ply3 is reached with no win or draw, look at every group of 4 consecutive cells
int scoringPLY3(char board[6][7])
{
    int score = 0;
    
    for(int r=0; r<6; r++) //horizontal
        for(int c=0; c<4; c++)
            score += scoring(board[r][c], board[r][c+1], board[r][c+2], board[r][c+3]);
    
    for(int r=0; r<3; r++) //vertical
        for(int c=0; c<7; c++)
            score += scoring(board[r][c], board[r+1][c], board[r+2][c], board[r+3][c]);
    
    for(int r=0; r<3; r++) //primary diagonal
        for(int c=0; c<4; c++)
            score += scoring(board[r][c], board[r+1][c+1], board[r+2][c+2], board[r+3][c+3]);
    
    for(int r=0; r<3; r++) //secondary diagonal
        for(int c=3; c<7; c++)
            score += scoring(board[r][c], board[r+1][c-1], board[r+2][c-2], board[r+3][c-3]);

    for(int r=0; r<6; r++)    // +3 for every computer piece in column 3
        if(board[r][3]=='O')
            score += 3;

    return score;
}
int computer_move(char board[6][7])
{
    int order[7] = {3, 2, 4, 1, 5, 0, 6};
    int L = -1000000, U = 1000000;
    int best_score_ply1 = L, best_col = -1;

    for(int i1=0; i1<7; i1++) // PLY1: computer tries each column
    {
        int c1=order[i1];
        if( !playable(board, c1)) continue;

        int s1, r1=drop(board, c1, 'O'); //row index where O was dropped

        if(win(board, 'O')) s1 = 100000+3-1; //ply1 so p=1
        else if(full_board(board)) s1=0; //if draw occurs
        else
        {
            int L1=L, U1=U;
            int best_score_ply2 = 1000000; //human wants lowest (worst) score

            for(int i2=0; i2<7; i2++) //PLY2: human replies
            {
                int c2=order[i2];
                if(!playable(board, c2)) continue;

                int s2, r2=drop(board, c2, 'X');

                if(win(board, 'X')) s2 = -(100000 + 3 - 2); //ply 2
                else if(full_board(board)) s2 = 0;
                else
                {
                    int L2=L1, U2=U1;
                    int best_score_ply3 = -1000000; //computer wants highest score

                    for(int i3=0; i3<7; i3++) //PLY3: computer follow-up
                    {
                        int c3=order[i3];
                        if(!playable(board, c3)) continue;

                        int s3, r3=drop(board, c3, 'O');

                        if(win(board, 'O')) s3 = 100000 + 3 - 3; // +100000 at ply 3
                        else if(full_board(board)) s3 = 0;
                        else s3=scoringPLY3(board);

                        board[r3][c3]='.'; //undo PLY3
                        if(s3>best_score_ply3) best_score_ply3=s3;
                        if(best_score_ply3>L2) L2=best_score_ply3;
                        if(L2>=U2) break; //stop trying the remaining columns at this position
                    }
                    s2=best_score_ply3;
                }
                board[r2][c2]='.'; //undo PLY2
                if(s2<best_score_ply2) best_score_ply2 = s2;
                if(best_score_ply2<U1) U1=best_score_ply2;
                if(L1>=U1) break; //stop trying the remaining columns at this position
            }
            s1=best_score_ply2;
        }
        board[r1][c1]='.'; //undo PLY1

        //replace best column ONLY when new score is strictly greater
        if(s1>best_score_ply1 || best_col==-1)
        {
            best_score_ply1=s1; best_col=c1;
        }
        if(best_score_ply1>L) L=best_score_ply1;
        if(L>=U) break; //stop trying the remaining columns at this position
    }
    return best_col;
}

void q7()
{
    char board[6][7];
    int col, computer;
    for (int r = 0; r < 6; r++) 
        for (int c = 0; c < 7; c++) 
            board[r][c] = '.';
    display(board);
    
    while(true)
    {
        while(true)
        {
            cout<<"Enter column (0-6): "; cin>>col;
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout<<"\tInvalid Input! Try again.\n";
                continue;
            }
            if(!playable(board, col))
            {
            cout<<"\tInvalid or Full column! Try again.\n";
            continue;
            }
            break;
        }
        drop(board, col, 'X');
        display(board);
        if(win(board, 'X'))
        {
            cout<<"\tHuman Wins!\n";
            break;
        }
        if(full_board(board))
        {
            cout<<"\tDRAW!\n";
            break;
        }

        computer = computer_move(board);
        cout<<"Computer plays column: "<<computer<<endl;
        drop(board, computer, 'O');
        display(board);
        if(win(board, 'O'))
        {
            cout<<"\tComputer Wins!\n";
            break;
        }
        if(full_board(board))
        {
            cout<<"\tDRAW!\n";
            break;
        }
    }
}
int main()
{
    q7();
    return 0;
}