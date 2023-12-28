#include <stdio.h>

static char oImage[2][5] = {{' ','_','_','_',' '}, {'|','_','_','_','|'}};
static char xImage[2][5] = {{' ','\\','/',' ',' '},{' ','/','\\',' ',' '}};
static char board[9] = {0,0,0,0,0,0,0,0,0};
void DrawBoard(char *board);
char getSquare(char *input);
char isQuit(char *input);
char Result(char *node);
char StatEval(char *node);
char Minimax(char *node,char maximizingPlayer,char Depth);
void printNodeVal(char* node)
{
	printf("{");
	char str[19];
	for(char i = 0;i < 9;i++)
	{
		switch(node[i])
		{
			case 0: str[i*2] = '_';break;
			case 1: str[i*2] = 'O';break;
			case 2: str[i*2] = 'X';break;
		}
		str[i*2+1] = ',';
	}
	str[17] = '}';
	str[18] = '\0';
	printf(str);
	printf("\n");
}
int main(int argc, char *argv[])
{
	while(1)
	{
		DrawBoard(board);
		char move[5];
		printf("Your turn to move\n");
		scanf("%5s",move);
		if(isQuit(move))
			break;
		char movSqr = getSquare(move);
		if(board[movSqr])
		{
			printf("Invalid move. Try again\n");
			continue;
		}
		board[movSqr] = 1;
		char Terminal = Result(board);
		if(Terminal)
		{
			printf("Game over.\n");
			switch(Terminal)
			{
				case 1:printf("O wins.\n"); break;
				case 2:printf("X wins.\n"); break;
				case -1:printf("It is a draw.\n"); break;
			}
			DrawBoard(board);
			printf("Board cleared.\n");
			for(char i = 0;i < 9;i++)
			{
				board[i] = 0;
			}
			continue;
		}
		char minEvalI = -1;
		char evals[9] = {0,0,0,0,0,0,0,0,0};
		for(char i = 0;i < 9;i++)
		{
			if(board[i])
				continue;
			
			if(minEvalI == -1)
				minEvalI = i;
			char copy[9];
			memcpy(copy,board,9);
			copy[i] = 2;
			char eval = Minimax(copy,1, 0);
			evals[i] = eval;
			if(eval < evals[minEvalI])
				minEvalI = i;
		}
		printf("\n");
		board[minEvalI] = 2;
		Terminal = Result(board);
		if(Terminal)
		{
			printf("Game over.\n");
			switch(Terminal)
			{
				case 1:printf("O wins.\n"); break;
				case 2:printf("X wins.\n"); break;
				case -1:printf("It is a draw.\n"); break;
			}
			DrawBoard(board);
			printf("Board cleared.\n");
			for(char i = 0;i < 9;i++)
			{
				board[i] = 0;
			}
			continue;
		}
	}
	return 0;
}
char Result(char* node)
{
	char winnings[8][3] = {{0,4,8},{0,1,2},{0,3,6},{1,4,7},{2,4,6},{2,5,8},{3,4,5},{6,7,8}};
	char moveLeft = 0;
	for(char i = 0;i < 9;i++)
		if(!node[i])
		{
			moveLeft = 1;
			break;
		}
	for(char i = 0;i < 8;i++)
	{
		if(!node[winnings[i][0]])
		{
			continue;
		}
		if((node[winnings[i][0]] == node[winnings[i][1]]) && (node[winnings[i][1]] == node[winnings[i][2]]))
		{
			return node[winnings[i][0]];
		}
	}
	return moveLeft ? 0 : -1;
}
char StatEval(char* node)
{
	char result = 0;
	char PartialMatch[16][2] = {{0,1},{0,2},{0,3},{0,6},{0,4},{0,8},{1,4},{1,7},{2,5},{2,8},{2,4},{2,6},{3,4},{3,5},{6,7},{6,8}};
	for(char i = 0;i < 16;i++)
	{
		if(!node[PartialMatch[i][0]])
			continue;
		if(node[PartialMatch[i][0]] == node[PartialMatch[i][1]])
			result += (node[PartialMatch[i][0]] == 1) ? 1 : -1;
	}
	return result;
}
char Minimax(char* node,char maximizingPlayer, char depth)
{
	char Terminal = Result(node);
	depth++;
	if(Terminal)
	{
		switch(Terminal)
		{
			case 1:{return 1;}
			case 2:{return -1;}
			case -1:{return 0;}
		}
	}
	char bestEval = maximizingPlayer ? -128 : 127;
	for(char i = 0;i < 9;i++)
	{
		if(node[i])
			continue;
		char copy[9];
		memcpy(copy, node, 9 * sizeof(char));
		copy[i] = maximizingPlayer ? 1 : 2;
		char CurrentEval = Minimax(copy, !maximizingPlayer, depth);
		if(maximizingPlayer)
			bestEval = CurrentEval > bestEval ? CurrentEval : bestEval;
		else
			bestEval = CurrentEval < bestEval ? CurrentEval : bestEval;
	}
	
	return bestEval;
}
char isQuit(char *input)
{
	char *iterate = input;
	char found = 0;
	while(*iterate == ' ')
		iterate++;
	while(*iterate != '\0')
	{
		if((*iterate == 'q') || (*iterate == 'Q'))
			if(found)
				return 0;
			else
			{
				found = 1;
				iterate++;
				continue;
			}
		if(*iterate == ' ');
		{
			iterate++;
			continue;
		}
		return 0;
	}
	return found;

}
char getSquare(char *input)
{
	char *iterate = input;
	char position = 0;
	while(*iterate != '\0')
	{
		if(*iterate == ' ')
		{
			iterate++;
			continue;
		}
		switch(*iterate)
		{
			case 'A':
			case 'a':
				break;
			case 'C':
			case 'c':
				position++;
			case 'B':
			case 'b':
				position++;
			case '1':
				break;
			case '3':
				position += 3;
			case '2':
				position += 3;
				break;
			default:
				return -1;
		}
		iterate++;
	}
	if(position > 8)
		return -1;
	return position;
}
void DrawBoard(char* board)
{
	char Draw[9][19] =  {"     |     |     \n",
			     "     |     |     \n",
			     "_____|_____|_____\n",
			     "     |     |     \n",
			     "     |     |     \n",
			     "_____|_____|_____\n",
			     "     |     |     \n",
			     "     |     |     \n",
			     "     |     |     \n"};
	for(char i = 0;i < 9;i++)
	{
		if(!board[i])
			continue;
		char column = i % 3 * 6;
		char row = i - i%3;
		if(board[i] - 1)
			for(char j = 0;j < 5;j++)
				for(char k = 0;k < 2;k++)
					Draw[row + k][column + j] = xImage[k][j];
		else
			for(char j = 0;j < 5;j++)
				for(char k = 0;k < 2;k++)
					Draw[row + k][column + j] = oImage[k][j];
	}
	printf("   A     B     C  \n");
	for(char i = 0;i < 9;i++)
	{
		char numStr[2];
		numStr[0] = 49 + i / 3;
		numStr[1] = '\0';
		printf(((i - 1) % 3 == 0) ? numStr : " ");
		printf(Draw[i]);
	}
}
