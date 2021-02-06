/* ========================= INCLUDES ============================== */
#include <stdio.h>
#include <string.h>
#include <math.h>
 
/* ========================= DEFINES ============================== */
#define MAX_ROW 10
#define MAX_COL 4
#define MAX_MOVES 27
#define TRUE (1 == 1)
#define FALSE (1 != 1)
#define INFINITY_TAUNT 50000
#define BEGIN_ALL_ROWS_AND_COLS 	for(row = 0; row < MAX_ROW; row++) \
										{ \
											for(col = 0; col < MAX_COL; col++)
															
															
#define END_ALL_ROWS_AND_COLS	}
#define UPWARD 0
#define DOWNWARD 1
#define MAX_PIECES 9
#define PIECE1_WEIGHT 100
#define PIECE2_WEIGHT 110
#define PIECE3_WEIGHT 110
#define TOTAL_PIECES 9
#define MAX_DEPTH 6
 
//#define DEBUG
//#define DEBUG2
//#define DEBUG3
// #define DEBUG4
 
/* ========================= DATATYPE MODIFIERS ============================== */
typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;
 
/* ========================= ENUMS ============================== */
typedef enum playerId
{
	INVALID_PLAYER,
	PLAYER1,
	PLAYER2,
}playerId_t;
 
typedef enum pieceType
{
	NO_PIECE,
	PIECE1,
	PIECE2,
	PIECE3
}pieceType_t;
 
typedef enum moveSetPiece1
{
	INVALID_MOVE,
	FORWARD,
	LEFT,
	RIGHT,
	BACKWARD
}mvSetPiece1_t;
 
/* ========================= STRUCTURES ============================== */
typedef struct taunt
{
	
	uint16_t board[MAX_ROW][MAX_COL];
	uint16_t totalMoves;
	playerId_t myPlayerId;
}taunt_t;
 
typedef struct outputMove
{
    uint16_t moveFrom[2];
    uint16_t moveTo[2];
}outputMove_t;
typedef struct deletedPieces
{
	uint16_t deletedFrom[2][2];	// 1st PIECE
//	uint16_t deletedFrom2[2];	// 2nd PIECE
	uint16_t deletedPiece[2];		// Max 2 pieces can be deleted in one move
//	uint16_t deletedPiece2;
	uint16_t moveNo;
	uint16_t noOfPiecesDeleted;	// 1 or 2
}deletedPieces_t;
 
/* ========================= PROTOTYPES ============================== */
void readInput(taunt_t *taunt);
int minimax(uint16_t (*board)[MAX_COL], uint16_t depth, int alpha, int beta, uint8_t isMaxPlayer);
uint16_t generateAllMoves(uint16_t (*board)[MAX_COL], outputMove_t *moveList, playerId_t playerId);
uint16_t doMove(uint16_t (*board)[MAX_COL], outputMove_t move, uint16_t moveNo, uint16_t depth, uint8_t isMaxPlayer);
uint16_t undoMove(uint16_t (*board)[MAX_COL], outputMove_t move, uint16_t moveNo, uint16_t depth, uint8_t isMaxPlayer);
int evalHeuVal(uint16_t (*board)[MAX_COL]);
int pattern1Val(uint16_t (*board)[MAX_COL]);
uint8_t isPieceSafe(uint16_t (*board)[MAX_COL], uint16_t row, uint16_t col, uint8_t isMaxPlayer);
int getAttackPoints(uint16_t (*board)[MAX_COL], uint16_t row, uint16_t col);
uint8_t isCloseInMove();
uint8_t checkWin(uint16_t (*board)[MAX_COL], uint8_t isMaxPlayer);
void printMove(outputMove_t myMove);
// #ifdef DEBUG2
void printBoard(uint16_t (*board)[MAX_COL]);
// #endif
 
/* ========================= GLOBAL VARIABLES ============================== */
uint16_t maxDepth = MAX_DEPTH, noOfPiecesDeletedMax = 0, noOfPiecesDeletedMin = 0;
playerId_t myPlayerId = INVALID_PLAYER;
playerId_t oppPlayerId = INVALID_PLAYER;
uint16_t totalMoves = 0;
outputMove_t bestMove;
deletedPieces_t deletedPiecesOfMax[MAX_PIECES][MAX_DEPTH];
deletedPieces_t deletedPiecesOfMin[MAX_PIECES][MAX_DEPTH];
 
/* ============================ ================================= */
 
int main()
{
    taunt_t taunt;
    int score = 0;
 
    readInput(&taunt);
	#ifdef DEBUG2
	printf("============= INPUT BOARD ==============\n");
	printBoard(taunt.board);
	printf("========================================\n");
	fflush(stdout);
	#endif
    myPlayerId = (uint8_t) taunt.myPlayerId;
	totalMoves = taunt.totalMoves;
    switch(taunt.myPlayerId)
    {
        case PLAYER1:
            oppPlayerId = PLAYER2;
            break;
            
        case PLAYER2:
            oppPlayerId = PLAYER1;
            break;
            
        default:
            break;
    }
	memset(&deletedPiecesOfMax, 0, sizeof(deletedPieces_t));
    score = minimax(taunt.board, maxDepth, -INFINITY_TAUNT, INFINITY_TAUNT, TRUE);
	
	#if 0
	if(score == 0 && !isCloseInMove() && arePiecesFar(board))
	{
		getCloseInMove(board);
		printMove(bestMove);
		printf("cl In\n");
	}
	else
	{
	#endif
		printMove(bestMove);
	//}
	 printf("m: %d\n", score);
    return 0;
}
 
#if 0
uint8_t arePiecesFar(uint16_t (*board)[MAX_COL])
{
	uint16_t row = 0, col = 0;
	for(row = 0; row < MAX_ROW; row++)
	{
		for(col = 0; col < MAX_COL; col++)
		{
			if(myPlayerId == PLAYER1)
			{
				if()
			}
			else
			{
				
			}
		}
	}
	END_ALL_ROWS_AND_COLS
}
 
 
uint8_t isCloseInMove()
{
	uint8_t isCloseIn = FALSE;
	if(myPlayerId == PLAYER1)
	{
		if(bestMove.moveTo[0] > bestMove.moveFrom[0])
		{
			isCloseIn = TRUE;
		}
	}
	else
	{
		if(bestMove.moveTo[0] < bestMove.moveFrom[0])
		{
			isCloseIn = TRUE;
		}
	}
	return isCloseIn;
}
#endif
void readInput(taunt_t *taunt)
{
	uint16_t row=0, col=0;
	
	BEGIN_ALL_ROWS_AND_COLS
	{
		scanf("%d", &taunt->board[row][col]);
	}
	END_ALL_ROWS_AND_COLS
	scanf("%d %d", &taunt->totalMoves, &taunt->myPlayerId);
}
 
int minimax(uint16_t (*board)[MAX_COL], uint16_t depth, int alpha, int beta, uint8_t isMaxPlayer)
{
	uint16_t possibleMoves = 0, moveNo = 0, currDepthForBestMove = 0;
	int bestScore, score;
	outputMove_t moveList[MAX_MOVES];
	#ifdef DEBUG
	printf("In function Minimax depth: %d isMaxPlayer: %d\n", depth, isMaxPlayer);
	fflush(stdout);
	#endif
	
	if(checkWin(board, isMaxPlayer))
	{
		#ifdef DEBUG2
		printf("checkWin returned: %d isMaxPlayer: %d\n", checkWin(board, isMaxPlayer), isMaxPlayer);
		#endif
		return INFINITY_TAUNT;
	}
	if(depth == 0 || (totalMoves + (maxDepth - depth)) == 100)
	{
		#ifdef DEBUG4
		printf("max depth reached\n");
		printBoard(board);
		#endif
		return evalHeuVal(board);
	}
	
	if(isMaxPlayer)
	{
		bestScore = alpha;
		score = -INFINITY_TAUNT;
	    possibleMoves = generateAllMoves(board, moveList, myPlayerId);
		#ifdef DEBUG
		printf("This is a MAX player. Possible moves: %d depth: %d\n", possibleMoves, depth);
		fflush(stdout);
		#endif
	    if(possibleMoves != 0)
	    {
	        for(moveNo = 0; moveNo < possibleMoves; moveNo++)
		    {
				#ifdef DEBUG2
				if(depth == 4)
				{
					printf("Max moving moveNo: %d at depth 4\n", moveNo);
				}
				#endif
		        noOfPiecesDeletedMax = doMove(board, moveList[moveNo], moveNo, depth, isMaxPlayer);
				#ifdef DEBUG2
				if(depth == MAX_DEPTH)
				{
					printf("MAX moved moveNo: %d at depth 4, del pieces: %d\n", moveNo, noOfPiecesDeletedMax);
					printMove(moveList[moveNo]);
					printBoard(board);
					fflush(stdout);
				}
				#endif
				#ifdef DEBUG
				printf("Max moved moveNo: %d del pieces: %d\n", moveNo, noOfPiecesDeletedMax);
				#endif
		        score = minimax(board, depth - 1, alpha, beta, FALSE);
				#ifdef DEBUG2
				if(depth == MAX_DEPTH)
				{
					printf("SCORE in MAX: %d\n", score);
				}
				#endif
				noOfPiecesDeletedMax = undoMove(board, moveList[moveNo], moveNo, depth, isMaxPlayer);
				#ifdef DEBUG2
				if(depth == 4)
				{
					printf("MAX UNDO moved moveNo: %d noOfPiecesDeletedMax: %d\n", moveNo, noOfPiecesDeletedMax);
					printBoard(board);
					fflush(stdout);
				}
				#endif
 
		        if (score > bestScore || (score == INFINITY_TAUNT && depth + 1 > currDepthForBestMove) || score == -INFINITY_TAUNT)
		        {
					#ifdef DEBUG2
					if(score == INFINITY_TAUNT)
					{
						printf("Win found at higher level! depth:%d, currDepthForBestMove: %d\n", depth, currDepthForBestMove);
					}
					fflush(stdout);
					#endif
		        	bestScore = score;
					alpha = bestScore;
					#ifdef DEBUG2
					if(depth == MAX_DEPTH)
					{
						printf("BEST SCORE in MAX and depth:%d Sc: %d\n", depth, bestScore);
						fflush(stdout);
					}
					#endif
					currDepthForBestMove = depth;
					#ifdef DEBUG4
					printf("BEST SCORE in MAX: %d depth: %d\n", bestScore, depth);
					#endif
					if(depth == maxDepth)	// At root node if this cond is TRUE. Name "maxDepth" is confusing. So, don't be! :)
					{
					    memcpy(&bestMove, &moveList[moveNo], sizeof(outputMove_t));
					    #ifdef DEBUG4
					    printf("BEST MOVE FOUND. BEST SCORE in MAX: %d depth: %d\n", bestScore, depth);
		                printMove(moveList[moveNo]);
					    printMove(bestMove);
						#endif
						#ifdef DEBUG2
					    printf("BEST MOVE FOUND. BEST SCORE in MAX: %d depth: %d\n", bestScore, depth);
		                printMove(moveList[moveNo]);
					    printMove(bestMove);
						fflush(stdout);
						#endif
					}
		        }
				if(beta <= alpha)
				{
					#ifdef DEBUG2
					printf("Pruning in Max\n");
					#endif
					break;
				}
		    }
	    }
	}
	else
	{	// Min Player
		bestScore = beta;
		score = INFINITY_TAUNT;
		possibleMoves = generateAllMoves(board, moveList, oppPlayerId);
		#ifdef DEBUG
		printf("This is a MIN player. Possible moves: %d depth: %d\n", possibleMoves, depth);
		#endif
	    if(possibleMoves != 0)
	    {
	        for(moveNo = 0; moveNo < possibleMoves; moveNo++)
		    {
				#ifdef DEBUG2
				if(depth == 3)
				{
					printf("Min moving moveNo: %d\n", moveNo);
					fflush(stdout);
				}
				#endif
		        noOfPiecesDeletedMin = doMove(board, moveList[moveNo], moveNo, depth, isMaxPlayer);
				#ifdef DEBUG2
				if(depth == 3)
				{
					printf("MIN moved moveNo: %d del pieces: %d\n", moveNo, noOfPiecesDeletedMin);
					printBoard(board);
					fflush(stdout);
				}
				#endif
				#ifdef DEBUG
				printf("Min moved moveNo: %d\n", moveNo);
				#endif
		        score = minimax(board, depth - 1, alpha, beta, TRUE);
				#ifdef DEBUG3
				printf("SCORE in MIN: %d\n", score);
				#endif
				noOfPiecesDeletedMin = undoMove(board, moveList[moveNo], moveNo, depth, isMaxPlayer);
				#ifdef DEBUG2
				if(depth == 3)
				{
					//printf("MIN UNDO moved moveNo: %d noOfPiecesDeletedMin: %d\n", moveNo, noOfPiecesDeletedMin);
					//printBoard(board);
					//fflush(stdout);
				}
				#endif
				#ifdef DEBUG
				printf("MIN returned: %d curr bestScore: %d\n", score, bestScore);
				fflush(stdout);
				#endif
		        if (score < bestScore)
		        {
		        	bestScore = score;
					beta = bestScore;
					#ifdef DEBUG3
					printf("BEST SCORE in MIN: %d\n", bestScore);
					#endif
					#ifdef DEBUG2
					if(depth == 3)
					{
						printf("BEST SCORE in MIN: %d\n", bestScore);
						fflush(stdout);
					}
					#endif
		        }
				if(beta <= alpha)
				{
					#ifdef DEBUG2
					printf("Pruning in Min\n");
					#endif
					break;
				}
		    }
	    }
	}
	return bestScore;
}
 
uint16_t generateAllMoves(uint16_t (*board)[MAX_COL], outputMove_t *moveList, playerId_t playerId)
{
	uint16_t row = 0, col = 0, movesFound = 0;
	playerId_t pieceBelongsToPl = INVALID_PLAYER;
	pieceType_t pieceType = NO_PIECE;
	int direction;
	
	#ifdef DEBUG
	printf("In fun generateAllMoves playerId: %d\n", playerId);
	#endif
	
	BEGIN_ALL_ROWS_AND_COLS
	{
		pieceBelongsToPl = board[row][col]/100;
		pieceType = (board[row][col]/10) % 10;
		direction = board[row][col]%10;
		/* Find moves of Piece1, Player1 */
		if(pieceBelongsToPl == playerId && pieceType == PIECE1) // Pid Type Dir
		{
			#ifdef DEBUG
			printf("Piece 1 found for player: %d\n", playerId);
			#endif
			if(row < MAX_ROW - 1 && board[row+1][col]/100 != playerId)
			{   // FORWARD
				moveList[movesFound].moveFrom[0] = row;
				moveList[movesFound].moveFrom[1] = col;
				moveList[movesFound].moveTo[0] = row + 1;
				moveList[movesFound].moveTo[1] = col;
				#ifdef DEBUG
				printMove(moveList[movesFound]);
				#endif
				movesFound++;
			}
			if(col < MAX_COL - 1 && board[row][col+1]/100 != playerId)
			{   // LEFT
				moveList[movesFound].moveFrom[0] = row;
				moveList[movesFound].moveFrom[1] = col;
				moveList[movesFound].moveTo[0] = row;
				moveList[movesFound].moveTo[1] = col + 1;
				#ifdef DEBUG
				printMove(moveList[movesFound]);
				#endif
				movesFound++;
			}
			if(col > 0 && board[row][col-1]/100 != playerId)
			{   // RIGHT
				moveList[movesFound].moveFrom[0] = row;
				moveList[movesFound].moveFrom[1] = col;
				moveList[movesFound].moveTo[0] = row;
				moveList[movesFound].moveTo[1] = col - 1;
				#ifdef DEBUG
				printMove(moveList[movesFound]);
				#endif
				movesFound++;
			}
			if(row > 0 && board[row-1][col]/100 != playerId)
			{   // BACKWARD
				moveList[movesFound].moveFrom[0] = row;
				moveList[movesFound].moveFrom[1] = col;
				moveList[movesFound].moveTo[0] = row - 1;
				moveList[movesFound].moveTo[1] = col;
				#ifdef DEBUG
				printMove(moveList[movesFound]);
				#endif
				movesFound++;
			}
 
		}
		else if(pieceBelongsToPl == playerId && pieceType == PIECE2)
		{
			#ifdef DEBUG
			printf("Piece 2 found for player: %d\n", playerId);
			#endif
			if(direction == DOWNWARD)
			{   
				if(row < MAX_ROW - 2 && board[row+2][col]/100 != playerId)
				{   // FORWARD 2 Step
					moveList[movesFound].moveFrom[0] = row;
					moveList[movesFound].moveFrom[1] = col;
					moveList[movesFound].moveTo[0] = row + 2;
					moveList[movesFound].moveTo[1] = col;
					#ifdef DEBUG
					printMove(moveList[movesFound]);
					#endif
					movesFound++;
				}
				if(col < MAX_COL - 1 && board[row][col+1]/100 != playerId)
				{   // LEFT
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
		  		    moveList[movesFound].moveTo[0] = row;
				    moveList[movesFound].moveTo[1] = col + 1;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
				    #endif
				    movesFound++;
				}
				if(col > 0 && board[row][col-1]/100 != playerId)
			    {   // RIGHT
				    moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row;
				    moveList[movesFound].moveTo[1] = col - 1;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
			    }
				if(row == MAX_ROW - 2)
				{   // BOUNCE BACK
				    moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row;
				    moveList[movesFound].moveTo[1] = col;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
					
				}
				
			}
			else if(direction == UPWARD)
			{
				if(row > 1 && board[row-2][col]/100 != playerId)
				{   // FORWARD 2 Step
					moveList[movesFound].moveFrom[0] = row;
					moveList[movesFound].moveFrom[1] = col;
					moveList[movesFound].moveTo[0] = row - 2;
					moveList[movesFound].moveTo[1] = col;
					#ifdef DEBUG
					printMove(moveList[movesFound]);
					#endif
					movesFound++;
				}
				if(col > 0 && board[row][col-1]/100 != playerId)
				{   // LEFT
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
		  		    moveList[movesFound].moveTo[0] = row;
				    moveList[movesFound].moveTo[1] = col - 1;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
				    #endif
				    movesFound++;
				}
				if(col < MAX_COL - 1 && board[row][col+1]/100 != playerId)
			    {   // RIGHT
				    moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row;
				    moveList[movesFound].moveTo[1] = col + 1;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
			    }
				if(row == 1)
				{   // BOUNCE BACK
				    moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row;
				    moveList[movesFound].moveTo[1] = col;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
				}
			}
		}
		else if(pieceBelongsToPl == playerId && pieceType == PIECE3)
		{
			#ifdef DEBUG
			printf("Piece 3 found for player: %d at row: %d col: %d\n", playerId, row, col);
			#endif
			if(direction == DOWNWARD)
			{
				if(col < MAX_COL - 2 && row < MAX_ROW - 2 && board[row+2][col+2]/100 != playerId)
			    {   // DIAG LEFT
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row + 2;
				    moveList[movesFound].moveTo[1] = col + 2;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
				}
				if(col > 1 && row < MAX_ROW - 2 && board[row+2][col-2]/100 != playerId)
			    {   // DIAG RIGHT
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row + 2;
				    moveList[movesFound].moveTo[1] = col - 2;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
				}
				if((col == 1 || col == 2) && row < MAX_ROW - 2 && board[row+2][col]/100 != playerId)
			    {   // BOUNCE RIGHT/LEFT EDGE
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row + 2;
				    moveList[movesFound].moveTo[1] = col;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
				}
				if((col == 0 || col == 1) && row == MAX_ROW - 2 && board[row][col+2]/100 != playerId)
			    {   // BOUNCE DOWNWARD LEFT
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row;
				    moveList[movesFound].moveTo[1] = col + 2;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
				}
				if((col == 2 || col == 3) && row == MAX_ROW - 2 && board[row][col-2]/100 != playerId)
			    {   // BOUNCE DOWNWARD RIGHT
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row;
				    moveList[movesFound].moveTo[1] = col - 2;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
				}
				if(col == 2 && row == MAX_ROW - 2)
			    {   // BOUNCE BACK LEFT CORNER
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row;
				    moveList[movesFound].moveTo[1] = col;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
				}
				if(col == 1 && row == MAX_ROW - 2)
			    {   // BOUNCE BACK RIGHT CORNER
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row;
				    moveList[movesFound].moveTo[1] = col;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
				}
			    
			}
			else if(direction == UPWARD)
			{
			    if(col > 1 && row > 1 && board[row-2][col-2]/100 != playerId)
			    {   // DIAG LEFT
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row - 2;
				    moveList[movesFound].moveTo[1] = col - 2;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
				}
				if(col < MAX_COL - 2 && row > 1 && board[row-2][col+2]/100 != playerId)
			    {   // DIAG RIGHT
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row - 2;
				    moveList[movesFound].moveTo[1] = col + 2;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
				}
				if((col == 1 || col == 2) && row > 1 && board[row-2][col]/100 != playerId)
			    {   // BOUNCE RIGHT/LEFT EDGE
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row - 2;
				    moveList[movesFound].moveTo[1] = col;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
				}
				if((col == 2 || col == 3) && row == 1 && board[row][col-2]/100 != playerId)
			    {   // BOUNCE UPWARD LEFT
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row;
				    moveList[movesFound].moveTo[1] = col - 2;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
				}
				if((col == 0 || col == 1) && row == 1 && board[row][col+2]/100 != playerId)
			    {   // BOUNCE UPWARD RIGHT
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row;
				    moveList[movesFound].moveTo[1] = col + 2;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
				}
				if(col == 1 && row == 1)
			    {   // BOUNCE BACK UPWD LEFT CORNER
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row;
				    moveList[movesFound].moveTo[1] = col;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
				}
				if(col == 2 && row == 1)
			    {   // BOUNCE BACK UPWD RIGHT CORNER
					moveList[movesFound].moveFrom[0] = row;
				    moveList[movesFound].moveFrom[1] = col;
				    moveList[movesFound].moveTo[0] = row;
				    moveList[movesFound].moveTo[1] = col;
				    #ifdef DEBUG
				    printMove(moveList[movesFound]);
			    	#endif
				    movesFound++;
				}
			}
		}
	}
	END_ALL_ROWS_AND_COLS
	
	#ifdef DEBUG
	printf("\n");
	#endif
	return movesFound;
}
 
uint16_t doMove(uint16_t (*board)[MAX_COL], outputMove_t move, uint16_t moveNo, uint16_t depth, uint8_t isMaxPlayer)
{
	pieceType_t pieceType = NO_PIECE;
	playerId_t movingPlayerId = INVALID_PLAYER, oppPlayer = INVALID_PLAYER;
	uint16_t rowFrom = 0, colFrom = 0, rowTo = 0, colTo = 0, delPcCnt = 0, pcDelPerMove = 0;
	int direction;
	deletedPieces_t (*deletedPieces)[MAX_DEPTH];
 
	#ifdef DEBUG
	printf("In function doMove\n");
	#endif
	
	totalMoves++;
	rowFrom = move.moveFrom[0];
	colFrom = move.moveFrom[1];
	rowTo = move.moveTo[0];
	colTo = move.moveTo[1];
	
	movingPlayerId = board[rowFrom][colFrom]/100;
	pieceType = (board[rowFrom][colFrom]/10) % 10;
	direction = board[rowFrom][colFrom]%10;
	
	if(isMaxPlayer)
	{
		deletedPieces = deletedPiecesOfMax;
		delPcCnt = noOfPiecesDeletedMax;
		oppPlayer = oppPlayerId;
	}
	else
	{
		deletedPieces = deletedPiecesOfMin;
		delPcCnt = noOfPiecesDeletedMin;
		//pcDelPerMove = deletedPieces[delPcCnt].noOfPiecesDeleted;
		oppPlayer = myPlayerId;
	}
	
	switch(pieceType)
	{
		case PIECE1:
			#ifdef DEBUG2
			if(depth == 4)
			{
				printf("Moving PIECE1 from row: %d col: %d TO row: %d col: %d depth: %d\n", rowFrom, colFrom, rowTo, colTo, depth);
			}
			#endif
			if(board[rowTo][colTo]/100 == oppPlayer)
			{
				#ifdef DEBUG2
				if(depth == 4)
				{
					printf("DELETED PIECE1 from row: %d col: %d stored at delPcCnt: %d\n", rowFrom, colFrom, delPcCnt);
				}
				#endif
				deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][0] = rowTo;
				deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][1] = colTo;
				deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove] = board[rowTo][colTo];
				deletedPieces[delPcCnt][depth].moveNo = moveNo;
				deletedPieces[delPcCnt][depth].noOfPiecesDeleted++;
				pcDelPerMove++;
			}
			board[rowTo][colTo] = board[rowFrom][colFrom];
			board[rowFrom][colFrom] = 0;
			delPcCnt += pcDelPerMove;
		    break;
			
		case PIECE2:
		    #ifdef DEBUG2
			if(depth == 4)
			{
				printf("Moving PIECE2 from row: %d col: %d TO row: %d col: %d\n", rowFrom, colFrom, rowTo, colTo);
			}
			#endif
			if(abs(rowTo - rowFrom) == 2 || (rowFrom == rowTo && colFrom == colTo))
			{
				if(direction == DOWNWARD && rowFrom < MAX_ROW - 1 && board[rowFrom+1][colFrom]/100 == oppPlayer)
				{
					#ifdef DEBUG
					printf("DOMOVE- pcDelPerMove: %d\n", pcDelPerMove);
					#endif
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][0] = rowFrom + 1;
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][1] = colFrom;
					deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove] = board[rowFrom+1][colFrom];
					deletedPieces[delPcCnt][depth].moveNo = moveNo;
					deletedPieces[delPcCnt][depth].noOfPiecesDeleted++;
					#ifdef DEBUG2
					if(depth == 4)
					{
						printf("DELETED PIECE2 DWND from row: %d col: %d stored at delPcCnt: %d\n", rowFrom+1, colFrom, delPcCnt);
					}
					#endif
					// delPcCnt++;
					pcDelPerMove++;
					board[rowFrom+1][colFrom] = 0;
				}
				else if(direction == UPWARD && rowFrom > 0 && board[rowFrom-1][colFrom]/100 == oppPlayer)
				{
					#ifdef DEBUG
					printf("DOMOVE- pcDelPerMove: %d\n", pcDelPerMove);
					#endif
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][0] = rowFrom - 1;
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][1] = colFrom;
					deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove] = board[rowFrom-1][colFrom];
					deletedPieces[delPcCnt][depth].moveNo = moveNo;
					deletedPieces[delPcCnt][depth].noOfPiecesDeleted++;
					#ifdef DEBUG2
					if(depth == 4)
					{
						printf("DELETED PIECE2 UPWD from row: %d col: %d delPc: %d stored@delPcCnt: %d\n", rowFrom-1, colFrom,
							deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove], delPcCnt);
					}
					#endif
					pcDelPerMove++;
					// delPcCnt++;
					
					board[rowFrom-1][colFrom] = 0;
				}
			}
			if(board[rowTo][colTo]/100 == oppPlayer)
			{	
				#ifdef DEBUG
				printf("DOMOVE- pcDelPerMove: %d\n", pcDelPerMove);
				#endif
				deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][0] = rowTo;
				deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][1] = colTo;
				deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove] = board[rowTo][colTo];
				deletedPieces[delPcCnt][depth].moveNo = moveNo;
				deletedPieces[delPcCnt][depth].noOfPiecesDeleted++;
				#ifdef DEBUG2
				if(depth == 4)
				{
					printf("=========================\n");
					printf("DELETED pc: %d @dest. noOfPiecesDeleted: %d depth: %d\n", deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove], 
								deletedPieces[delPcCnt][depth].noOfPiecesDeleted, depth);
					printf("================+++=========\n");
				}
				#endif
				pcDelPerMove++;
				board[rowTo][colTo] = 0;
			}
			if(rowFrom != rowTo || colFrom != colTo)
			{
			    board[rowTo][colTo] = board[rowFrom][colFrom];
				board[rowFrom][colFrom] = 0;
			}
			else
			{	// PIECE2 BOUNCEed. Change direction.
				#ifdef DEBUG
				printf("PIECE2 BOUNCE\n");
				#endif
				if(direction == DOWNWARD)
				{	// DOWNWARD = 1
					board[rowFrom][colFrom] = board[rowFrom][colFrom] & 0xFFFE;
					#ifdef DEBUG
					printf("Direction changed to UPWARD: new val: %d\n", board[rowFrom][colFrom]);
					#endif
				}
				else
				{
					board[rowFrom][colFrom] = board[rowFrom][colFrom] | 0x1;
					#ifdef DEBUG
					printf("Direction changed to DOWNWARD: new val: %d\n", board[rowFrom][colFrom]);
					#endif
				}
			}
			delPcCnt += pcDelPerMove;
			#ifdef DEBUG
			printf("delPcCnt at end of case: %d, pcDelPerMove: %d noOfPiecesDeletedMin: %d\n", delPcCnt, pcDelPerMove, noOfPiecesDeletedMin);
			#endif
		    break;
			
		case PIECE3:
			#ifdef DEBUG2
			if(depth == 4)
			{
				printf("Moving PIECE3 from row: %d col: %d TO row: %d col: %d\n", rowFrom, colFrom, rowTo, colTo);
			}
			#endif
			if(direction == DOWNWARD)
			{
				if(colTo > colFrom && board[rowFrom+1][colFrom+1]/100 == oppPlayer)
				{	// LEFT DIAG
					#ifdef DEBUG
					printf("DOMOVE LEFT DIAG- pcDelPerMove: %d\n", pcDelPerMove);
					#endif
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][0] = rowFrom + 1;
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][1] = colFrom + 1;
					deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove] = board[rowFrom+1][colFrom+1];
					deletedPieces[delPcCnt][depth].moveNo = moveNo;
					deletedPieces[delPcCnt][depth].noOfPiecesDeleted++;
					#ifdef DEBUG2
					if(depth == 4)
					{
						printf("DELETED PIECE3 DWND LEFT from row: %d col: %d stored at delPcCnt: %d\n", rowFrom+1, colFrom+1, delPcCnt);
					}
					#endif
					// delPcCnt++;
					pcDelPerMove++;
					board[rowFrom+1][colFrom+1] = 0;
				}
				else if(colTo < colFrom && board[rowFrom+1][colFrom-1]/100 == oppPlayer)
				{	// RIGHT DIAG
					#ifdef DEBUG
					printf("DOMOVE RIGHT DIAG- pcDelPerMove: %d\n", pcDelPerMove);
					#endif
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][0] = rowFrom + 1;
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][1] = colFrom - 1;
					deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove] = board[rowFrom+1][colFrom-1];
					deletedPieces[delPcCnt][depth].moveNo = moveNo;
					deletedPieces[delPcCnt][depth].noOfPiecesDeleted++;
					#ifdef DEBUG2
					if(depth == 4)
					{
						printf("DELETED PIECE3 DWND RIGHT from row: %d col: %d stored at delPcCnt: %d\n", rowFrom+1, colFrom-1, delPcCnt);
					}
					#endif
					pcDelPerMove++;
					board[rowFrom+1][colFrom-1] = 0;
				}
				else if((colFrom == 2 && rowFrom == MAX_ROW - 2) && 
							(colFrom == colTo) &&
							(board[rowFrom+1][colFrom+1]/100 == oppPlayer))
			    {	// BOUNCE PIECE3 LEFT CORNER
					#ifdef DEBUG
					printf("DOMOVE- pcDelPerMove: %d\n", pcDelPerMove);
					#endif
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][0] = rowFrom + 1;
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][1] = colFrom + 1;
					deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove] = board[rowFrom+1][colFrom+1];
					deletedPieces[delPcCnt][depth].moveNo = moveNo;
					deletedPieces[delPcCnt][depth].noOfPiecesDeleted++;
					#ifdef DEBUG2
					if(depth == 4)
					{
						printf("DELETED PIECE3 LEFT CORNER from row: %d col: %d stored at delPcCnt: %d\n", rowFrom+1, colFrom+1, delPcCnt);
					}
					#endif
					pcDelPerMove++;
					board[rowFrom+1][colFrom+1] = 0;
				}
				else if((colFrom == 1 && rowFrom == MAX_ROW - 2) && 
							(colFrom == colTo) &&
							(board[rowFrom+1][colFrom-1]/100 == oppPlayer))
			    {	// BOUNCE PIECE3 LEFT CORNER
					#ifdef DEBUG
					printf("DOMOVE- pcDelPerMove: %d\n", pcDelPerMove);
					#endif
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][0] = rowFrom + 1;
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][1] = colFrom - 1;
					deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove] = board[rowFrom+1][colFrom-1];
					deletedPieces[delPcCnt][depth].moveNo = moveNo;
					deletedPieces[delPcCnt][depth].noOfPiecesDeleted++;
					#ifdef DEBUG2
					if(depth == 4)
					{
						printf("DELETED PIECE3 RIGHT CORNER from row: %d col: %d stored at delPcCnt: %d\n", rowFrom+1, colFrom-1, delPcCnt);
					}
					#endif
					pcDelPerMove++;
					board[rowFrom+1][colFrom-1] = 0;
				}
			}
			else if(direction == UPWARD)
			{
				if(colTo < colFrom && board[rowFrom-1][colFrom-1]/100 == oppPlayer)
				{	// LEFT DIAG
					#ifdef DEBUG
					printf("DOMOVE- pcDelPerMove: %d\n", pcDelPerMove);
					#endif
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][0] = rowFrom - 1;
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][1] = colFrom - 1;
					deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove] = board[rowFrom-1][colFrom-1];
					deletedPieces[delPcCnt][depth].moveNo = moveNo;
					deletedPieces[delPcCnt][depth].noOfPiecesDeleted++;
					#ifdef DEBUG2
					if(depth == 4)
					{
						printf("DELETED PIECE3 UPWD LEFT from row: %d col: %d stored at delPcCnt: %d\n", rowFrom-1, colFrom-1, delPcCnt);
					}
					#endif
					pcDelPerMove++;
					board[rowFrom-1][colFrom-1] = 0;
				}
				else if(colTo > colFrom && board[rowFrom-1][colFrom+1]/100 == oppPlayer)
				{	// RIGHT DIAG
					#ifdef DEBUG
					printf("DOMOVE- pcDelPerMove: %d\n", pcDelPerMove);
					#endif
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][0] = rowFrom - 1;
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][1] = colFrom + 1;
					deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove] = board[rowFrom-1][colFrom+1];
					deletedPieces[delPcCnt][depth].moveNo = moveNo;
					deletedPieces[delPcCnt][depth].noOfPiecesDeleted++;
					#ifdef DEBUG2
					if(depth == 4)
					{
						printf("DELETED PIECE3 UPWD RIGHT from row: %d col: %d stored at delPcCnt: %d\n", rowFrom-1, colFrom+1, delPcCnt);
					}
					#endif
					pcDelPerMove++;
					board[rowFrom-1][colFrom+1] = 0;
				}
			}
			if(board[rowTo][colTo]/100 == oppPlayer)
			{	
				#ifdef DEBUG
				printf("DOMOVE- pcDelPerMove: %d\n", pcDelPerMove);
				#endif
				deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][0] = rowTo;
				deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][1] = colTo;
				deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove] = board[rowTo][colTo];
				deletedPieces[delPcCnt][depth].moveNo = moveNo;
				deletedPieces[delPcCnt][depth].noOfPiecesDeleted++;
				#ifdef DEBUG2
				if(depth == 4)
				{
					printf("=========================\n");
					printf("DELETED pc: %d @dest of PIECE3 @ depth: %d noOfPiecesDeleted: %d delPcCnt: %d\n", 
						deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove], depth, deletedPieces[delPcCnt][depth].noOfPiecesDeleted, delPcCnt);
					printf("================---=========\n");
				}
				#endif
				pcDelPerMove++;
				board[rowTo][colTo] = 0;
				
			}
			if(rowFrom != rowTo || colFrom != colTo)
			{
				board[rowTo][colTo] = board[rowFrom][colFrom];
				board[rowFrom][colFrom] = 0;
			}
			if(rowFrom == rowTo)
			{
				#ifdef DEBUG
				printf("PIECE3 BOUNCE\n");
				#endif
				if(direction == DOWNWARD)
				{	// DOWNWARD = 1
					board[rowTo][colTo] = board[rowTo][colTo] & 0xFFFE;
					#ifdef DEBUG
					printf("Direction changed to UPWARD: new val: %d\n", board[rowTo][colTo]);
					#endif
				}
				else
				{
					board[rowTo][colTo] = board[rowTo][colTo] | 0x1;
					#ifdef DEBUG
					printf("Direction changed to DOWNWARD: new val: %d\n", board[rowTo][colTo]);
					#endif
				}
			}
			if(colFrom == colTo)
		    {   
				if(direction == DOWNWARD && rowFrom < MAX_ROW - 2 && colFrom == 2 && board[rowFrom+1][colFrom+1]/100 == oppPlayer)
				{	// BOUNCE LEFT EDGE	// row < 8 so it can never be a corner
					#ifdef DEBUG
					printf("DOMOVE- pcDelPerMove: %d\n", pcDelPerMove);
					#endif
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][0] = rowFrom + 1;
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][1] = colFrom + 1;
					deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove] = board[rowFrom+1][colFrom+1];
					deletedPieces[delPcCnt][depth].moveNo = moveNo;
					deletedPieces[delPcCnt][depth].noOfPiecesDeleted++;
					#ifdef DEBUG2
					if(depth == 4)
					{	
						printf("DELETED PIECE3 DWND LEFT EDGE from row: %d col: %d stored at delPcCnt: %d\n", rowFrom+1, colFrom+1, delPcCnt);
					}
					#endif
					pcDelPerMove++;
					board[rowFrom+1][colFrom+1] = 0;
				}
				else if(direction == DOWNWARD && rowFrom < MAX_ROW - 2 && colFrom == 1 && board[rowFrom+1][colFrom-1]/100 == oppPlayer)
				{	// BOUNCE RIGHT EDGE
					#ifdef DEBUG
					printf("DOMOVE- pcDelPerMove: %d\n", pcDelPerMove);
					#endif
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][0] = rowFrom + 1;
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][1] = colFrom - 1;
					deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove] = board[rowFrom+1][colFrom-1];
					deletedPieces[delPcCnt][depth].moveNo = moveNo;
					deletedPieces[delPcCnt][depth].noOfPiecesDeleted++;
					#ifdef DEBUG2
					if(depth == 4)
					{
						printf("DELETED PIECE3 DWND RIGHT EDGE from row: %d col: %d stored at delPcCnt: %d\n", rowFrom+1, colFrom-1, delPcCnt);
					}
					#endif
					pcDelPerMove++;
					board[rowFrom+1][colFrom-1] = 0;
				}
				else if(direction == UPWARD && rowFrom > 1 && colFrom == 1 && board[rowFrom-1][colFrom-1]/100 == oppPlayer)
				{	// BOUNCE LEFT EDGE	// row > 2 so it can never be a corner
					#ifdef DEBUG
					printf("DOMOVE- pcDelPerMove: %d\n", pcDelPerMove);
					#endif
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][0] = rowFrom - 1;
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][1] = colFrom - 1;
					deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove] = board[rowFrom-1][colFrom-1];
					deletedPieces[delPcCnt][depth].moveNo = moveNo;
					deletedPieces[delPcCnt][depth].noOfPiecesDeleted++;
					#ifdef DEBUG2
					if(depth == 4)
					{
						printf("DELETED PIECE3 UPWD LEFT EDGE from row: %d col: %d stored at delPcCnt: %d\n", rowFrom-1, colFrom-1, delPcCnt);
					}
					#endif
					pcDelPerMove++;
					board[rowFrom-1][colFrom-1] = 0;
				}
				else if(direction == UPWARD && rowFrom > 1 && colFrom == 2 && board[rowFrom-1][colFrom+1]/100 == oppPlayer)
				{	// BOUNCE RIGHT EDGE
					#ifdef DEBUG
					printf("DOMOVE- pcDelPerMove: %d\n", pcDelPerMove);
					#endif
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][0] = rowFrom - 1;
					deletedPieces[delPcCnt][depth].deletedFrom[pcDelPerMove][1] = colFrom + 1;
					deletedPieces[delPcCnt][depth].deletedPiece[pcDelPerMove] = board[rowFrom-1][colFrom+1];
					deletedPieces[delPcCnt][depth].moveNo = moveNo;
					deletedPieces[delPcCnt][depth].noOfPiecesDeleted++;
					#ifdef DEBUG2
					if(depth == 4)
					{
						printf("DELETED PIECE3 UPWD RIGHT EDGE from row: %d col: %d stored at delPcCnt: %d\n", rowFrom-1, colFrom+1, delPcCnt);
					}
					#endif
					pcDelPerMove++;
					board[rowFrom-1][colFrom+1] = 0;
				}
			}
			delPcCnt += pcDelPerMove;
		    break;
			
		default:
		    #ifdef DEBUG2
		    printf("INVALID PIECE: %d received for Move:\n", pieceType);
			printMove(move);
			#endif
		    break;
	}
	#ifdef DEBUG
	printf("delPcCnt returned from doMove: %d, pcDelPerMove: %d\n", delPcCnt);
	#endif
	return delPcCnt;
}
 
uint16_t undoMove(uint16_t (*board)[MAX_COL], outputMove_t move, uint16_t moveNo, uint16_t depth, uint8_t isMaxPlayer)
{
	pieceType_t pieceType = NO_PIECE;
	uint16_t rowFrom = 0, colFrom = 0, rowTo = 0, colTo = 0, delPcCnt = 0, pcDelPerMove = 0, delRow = 0, delCol = 0;
	uint8_t pieceIte = 0, pieceIteMove = 0, isPieceFound = FALSE;
	int direction;
	deletedPieces_t (*deletedPieces)[MAX_DEPTH];
	
	#ifdef DEBUG
	printf("In function UNDO Move\n");
	//printf("Board before UNDO\n");
	//printBoard(board);
	#endif
	
	totalMoves--;
	if(isMaxPlayer)
	{
		deletedPieces = deletedPiecesOfMax;
		delPcCnt = noOfPiecesDeletedMax;
		#ifdef DEBUG
		printf("delPcCnt in Max UNDO: %d\n", delPcCnt);
		fflush(stdout);
		#endif
	}
	else
	{
		deletedPieces = deletedPiecesOfMin;
		delPcCnt = noOfPiecesDeletedMin;
		#ifdef DEBUG
		printf("delPcCnt in Min UNDO: %d\n", delPcCnt);
		fflush(stdout);
		#endif
	}
	
	rowTo = move.moveFrom[0];
	colTo = move.moveFrom[1];
	rowFrom = move.moveTo[0];
	colFrom = move.moveTo[1];
	
	pieceType = (board[rowFrom][colFrom]/10) % 10;
	direction = board[rowFrom][colFrom]%10;
	
	switch(pieceType)
	{
		case PIECE1:
			#ifdef DEBUG2
			if(depth == 4)
			{
				printf("UNDO Moving PIECE1 from row: %d col: %d TO row: %d col: %d\n", rowFrom, colFrom, rowTo, colTo);
			}
			#endif
			board[rowTo][colTo] = board[rowFrom][colFrom];
			board[rowFrom][colFrom] = 0;
			
			for(pieceIte = 0; pieceIte < delPcCnt && !isPieceFound; pieceIte++)
			{
				if(deletedPieces[pieceIte][depth].moveNo == moveNo && deletedPieces[pieceIte][depth].noOfPiecesDeleted > 0)
				{
					isPieceFound = TRUE;
					delRow = deletedPieces[pieceIte][depth].deletedFrom[0][0];
		    		delCol = deletedPieces[pieceIte][depth].deletedFrom[0][1];
					#ifdef DEBUG2
					if(depth == 4)
					{
						printf("UNDO delRow: %d delCol: %d delPc: %d\n", delRow, delCol, deletedPieces[pieceIte][depth].deletedPiece[0]);
					}
					#endif
					board[delRow][delCol] = deletedPieces[pieceIte][depth].deletedPiece[0];
					delPcCnt--;
					memset(&deletedPieces[delPcCnt][depth], 0, sizeof(deletedPieces_t));
				}
			}
		    break;
			
		case PIECE2:
			#ifdef DEBUG2
			if(depth == 4)
			{
				printf("UNDO Moving PIECE2 from row: %d col: %d TO row: %d col: %d\n", rowFrom, colFrom, rowTo, colTo);
				fflush(stdout);
			}
			#endif
			if(rowFrom != rowTo || colFrom != colTo)
			{
				// #ifdef DEBUG2
				// printf("BEFORE UNDO Move PIECE2 @ from: %d @To: %d\n", board[rowFrom][colFrom], board[rowTo][colTo]);
				// #endif
			    board[rowTo][colTo] = board[rowFrom][colFrom];
				board[rowFrom][colFrom] = 0;
				// #ifdef DEBUG2
				// printf("AFTER UNDO Move PIECE2 @ from: %d @To: %d\n", board[rowFrom][colFrom], board[rowTo][colTo]);
				// #endif
			}
			else
			{
				if(direction == DOWNWARD)
				{	// DOWNWARD = 1
					board[rowFrom][colFrom] = board[rowFrom][colFrom] & 0xFFFE;
					#ifdef DEBUG
					printf("UNDO Direction changed to UPWARD: new val: %d\n", board[rowFrom][colFrom]);
					#endif
				}
				else
				{
					board[rowFrom][colFrom] = board[rowFrom][colFrom] | 0x1;
					#ifdef DEBUG
					printf("UNDO Direction changed to DOWNWARD: new val: %d\n", board[rowFrom][colFrom]);
					#endif
				}
			}
			for(pieceIte = 0; pieceIte < delPcCnt && !isPieceFound; pieceIte++)
			{
				if(deletedPieces[pieceIte][depth].moveNo == moveNo && deletedPieces[pieceIte][depth].noOfPiecesDeleted > 0)
				{	// Piece/s deleted for this move
					#ifdef DEBUG
					printf("UNDO delete for moveNo: %d pieceIte: %d\n", moveNo, pieceIte);
					#endif
					isPieceFound = TRUE;
					pcDelPerMove = deletedPieces[pieceIte][depth].noOfPiecesDeleted;
					#ifdef DEBUG
					printf("UNDO pcDelPerMove: %d\n", pcDelPerMove);
					fflush(stdout);
					#endif
					for(pieceIteMove = 0; pieceIteMove < pcDelPerMove; pieceIteMove++)
					{
						delRow = deletedPieces[pieceIte][depth].deletedFrom[pieceIteMove][0];
						delCol = deletedPieces[pieceIte][depth].deletedFrom[pieceIteMove][1];
						#ifdef DEBUG
						printf("UNDO delRow: %d delCol: %d delPc: %d fetch from del pieceIteMove: %d\n", delRow, delCol, 
								deletedPieces[pieceIte][depth].deletedPiece[pieceIteMove], pieceIteMove);
						fflush(stdout);
						#endif
						board[delRow][delCol] = deletedPieces[pieceIte][depth].deletedPiece[pieceIteMove];
						delPcCnt--;
					}
					memset(&deletedPieces[delPcCnt][depth], 0, sizeof(deletedPieces_t));					
				}
			}
			
		    break;
		
		case PIECE3:
			#ifdef DEBUG2
			if(depth == 4)
			{
				printf("UNDO Moving PIECE3 from row: %d col: %d TO row: %d col: %d\n", rowFrom, colFrom, rowTo, colTo);
			}
			#endif
			if(rowFrom != rowTo || colFrom != colTo)
			{
				board[rowTo][colTo] = board[rowFrom][colFrom];
			    board[rowFrom][colFrom] = 0;
			}
			if(rowFrom == rowTo)
			{
				#ifdef DEBUG
				printf("PIECE3 BOUNCE\n");
				#endif
				if(direction == DOWNWARD)
				{	// DOWNWARD = 1
					board[rowTo][colTo] = board[rowTo][colTo] & 0xFFFE;
					#ifdef DEBUG
					printf("UNDO Direction changed to UPWARD: new val: %d\n", board[rowTo][colTo]);
					#endif
				}
				else
				{
					board[rowTo][colTo] = board[rowTo][colTo] | 0x1;
					#ifdef DEBUG
					printf("UNDO Direction changed to DOWNWARD: new val: %d\n", board[rowTo][colTo]);
					#endif
				}
			}
			for(pieceIte = 0; pieceIte < delPcCnt && !isPieceFound; pieceIte++)
			{
				if(deletedPieces[pieceIte][depth].moveNo == moveNo && deletedPieces[pieceIte][depth].noOfPiecesDeleted > 0)
				{
					isPieceFound = TRUE;
					pcDelPerMove = deletedPieces[pieceIte][depth].noOfPiecesDeleted;
					#ifdef DEBUG
					printf("UNDO delete for moveNo: %d of depth: %d pieceIte: %d pcDelPerMove: %d\n", moveNo, depth, pieceIte, pcDelPerMove);
					#endif
					for(pieceIteMove = 0; pieceIteMove < pcDelPerMove; pieceIteMove++)
					{
						delRow = deletedPieces[pieceIte][depth].deletedFrom[pieceIteMove][0];
						delCol = deletedPieces[pieceIte][depth].deletedFrom[pieceIteMove][1];
						#ifdef DEBUG
						printf("UNDO delRow: %d delCol: %d delPc: %d fetch from del pieceIteMove: %d\n", delRow, delCol, 
								deletedPieces[pieceIte][depth].deletedPiece[pieceIteMove], pieceIteMove);
						#endif
						board[delRow][delCol] = deletedPieces[pieceIte][depth].deletedPiece[pieceIteMove];
						delPcCnt--;
						memset(&deletedPieces[delPcCnt][depth], 0, sizeof(deletedPieces_t));					
					}
				}
			}
		    break;
			
		default:
		    #ifdef DEBUG2
		    printf("INVALID PIECE: %d received for Move:\n", pieceType);
			printMove(move);
			#endif
		    break;
	}
	return delPcCnt;
}
int evalHeuVal(uint16_t (*board)[MAX_COL])
{
	uint16_t row = 0, col = 0;
	int heuVal = 0;
	pieceType_t pieceType = NO_PIECE;
	uint16_t myPiece1Count = 0, myPiece2Count = 0, myPiece3Count = 0, oppPiece1Count = 0, oppPiece2Count = 0, oppPiece3Count = 0;
						
	BEGIN_ALL_ROWS_AND_COLS
	{
		pieceType = (board[row][col]/10) % 10;
		if(board[row][col]/100 == myPlayerId)
		{ 	// If my PIECE, add 100
			switch(pieceType)
			{
				case PIECE1:
				    myPiece1Count++;
					// Need to do this only if the resulting position is safe!
					if(isPieceSafe(board, row, col, TRUE))
					{
						heuVal += (getAttackPoints(board, row, col));
						if(row == 4 || row == 5)
						{
							heuVal +=1;
						}
						if(myPlayerId == PLAYER2 && row == 6)
						{
							heuVal +=1;
						}
						else if(myPlayerId == PLAYER2 && row == 5)
						{
							heuVal +=2;
						}
						#ifdef DEBUG2
						printf("piece1 safe @ row: %d, col: %d heuVal after attack points: %d\n", row, col, heuVal);
						#endif
					}
					break;
				case PIECE2:
					myPiece2Count++;
					if(isPieceSafe(board, row, col, TRUE))
					{
						heuVal += (getAttackPoints(board, row, col));
						if(row == 4 || row == 5)
						{
							heuVal +=1;
						}
						#ifdef DEBUG2
						printf("piece2 safe @ row: %d, col: %d heuVal after attack points: %d\n", row, col, heuVal);
						#endif
					}
					break;
				case PIECE3:
					myPiece3Count++;
					if(isPieceSafe(board, row, col, TRUE))
					{
						heuVal += (getAttackPoints(board, row, col));
						if(row == 4 || row == 5)
						{
							heuVal +=1;
						}
						#ifdef DEBUG2
						printf("piece3 safe @ row: %d, col: %d heuVal after attack points: %d\n", row, col, heuVal);
						#endif
					}
					break;
				default:
					break;
			}
		}
		else if(board[row][col]/100 == oppPlayerId)
		{	// If opp PIECE, sub 100
			switch(pieceType)
			{
				case PIECE1:
				    oppPiece1Count++;
					break;
				case PIECE2:
					oppPiece2Count++;
					break;
				case PIECE3:
					oppPiece3Count++;
					break;
				default:
					break;
			}
		}
	}
	END_ALL_ROWS_AND_COLS
	
	heuVal = heuVal + ((PIECE1_WEIGHT * myPiece1Count) + (PIECE2_WEIGHT * myPiece2Count) + (PIECE3_WEIGHT * myPiece3Count)) - 
					  ((PIECE1_WEIGHT * oppPiece1Count) + (PIECE2_WEIGHT * oppPiece2Count) + (PIECE3_WEIGHT * oppPiece3Count));
	//heuVal += (TOTAL_PIECES - (oppPiece1Count + oppPiece2Count + oppPiece3Count));
	//heuVal += totalMoves;
#if 0	
	if(isCloseInMove())
	{
		heuVal = heuVal + 5;
	}
#endif
	
	//heuVal = heuVal + pattern1Val(board);
	#ifdef DEBUG2
	printf("heuVal: %d pattern1Val: %d\n", heuVal, pattern1Val(board));
	#endif
	return heuVal;
}
 
uint8_t isPieceSafe(uint16_t (*board)[MAX_COL], uint16_t row, uint16_t col, uint8_t isMaxPlayer)
{
	uint16_t isSafe = TRUE;
	pieceType_t pieceType = NO_PIECE;
	int direction;
	playerId_t oppPlayer = INVALID_PLAYER;
	
	if(isMaxPlayer)
	{
		oppPlayer = oppPlayerId;
	}
	else
	{
		oppPlayer = myPlayerId;
	}
	// Check above cell.
	if(row > 0 && board[row-1][col]/100 == oppPlayer)
	{
		pieceType = (board[row-1][col]/10) % 10;
		direction = board[row-1][col]%10;
		switch(pieceType)
		{
			case PIECE1:
				isSafe = FALSE;
				break;
			case PIECE2:
				if(row < MAX_ROW - 1 && direction == DOWNWARD && (board[row+1][col]/100 != oppPlayer))
				{
					isSafe = FALSE;
				}
				break;
			default:
				break;
		}
	}
	// Check below cell.
	if(row < MAX_ROW - 1 && board[row+1][col]/100 == oppPlayer)
	{	
		pieceType = (board[row+1][col]/10) % 10;
		direction = board[row+1][col]%10;
		switch(pieceType)
		{
			case PIECE1:	// Opp PIECE1 below.
				isSafe = FALSE;
			    break;
			case PIECE2:	// Opp PIECE2 below.
				if(row > 0 && direction == UPWARD && (board[row-1][col]/100 != oppPlayer))
				{
					isSafe = FALSE;
				}
			    break;
			default:
				break;
		}
	}
	// Check LEFT cell (P1 pov).
	if(col < MAX_COL - 1 && board[row][col+1]/100 == oppPlayerId)
	{
		pieceType = (board[row][col+1]/10) % 10;
		direction = board[row][col+1]%10;
		switch(pieceType)
		{
			case PIECE1:	// Opp PIECE1 on LEFT.
				isSafe = FALSE;
				break;
			case PIECE2:	// Opp PIECE1 on RIGHT.
				isSafe = FALSE;
				break;
			default:
				break;
		}
	}
	// Check RIGHT cell (P1 pov).
	if(col > 0 && board[row][col-1]/100 == oppPlayer)
	{
		pieceType = (board[row][col-1]/10) % 10;
		direction = board[row][col-1]%10;
		switch(pieceType)
		{
			case PIECE1:	// Opp PIECE1 on RIGHT.
				isSafe = FALSE;
				break;
			case PIECE2:
				isSafe = FALSE;
				break;
			default:
				break;
		}
	}
	// Check 2 cells above (P1 pov).
	if(row > 1 && board[row-2][col]/100 == oppPlayer)
	{
		pieceType = (board[row-2][col]/10) % 10;
		direction = board[row-2][col]%10;
		switch(pieceType)
		{
			case PIECE2:	// Opp PIECE2 two cells above.
				if(direction == DOWNWARD)
				{
					isSafe = FALSE;
				}
				break;
			case PIECE3:
				if((col == 1 || col == 3) && direction == DOWNWARD)
				{
					isSafe = FALSE;
				}
				break;
			default:
				break;
		}
	}
	// Check 2 cells below (P1 pov).
	if(row < MAX_ROW - 2 && board[row+2][col]/100 == oppPlayer)
	{
		pieceType = (board[row+2][col]/10) % 10;
		direction = board[row+2][col]%10;
		switch(pieceType)
		{
			case PIECE2:	// Opp PIECE2 two cells above.
				if(direction == UPWARD)
				{
					isSafe = FALSE;
				}
				break;
			case PIECE3:
				if((col == 1 || col == 3) && direction == DOWNWARD)
				{
					isSafe = FALSE;
				}
				break;
			default:
				break;
		}
	}
	// Check 2 cells LEFT (P1 pov).
	if(col < MAX_COL - 2 && board[row][col+2]/100 == oppPlayer)
	{
		pieceType = (board[row][col+2]/10) % 10;
		direction = board[row][col+2]%10;
		switch(pieceType)
		{
			case PIECE3:	// Opp PIECE3 two cells LEFT.
				if(direction == UPWARD && row == 1)
				{
					isSafe = FALSE;
				}
				else if(direction == DOWNWARD && row == 8)
				{
					isSafe = FALSE;
				}
				break;
			default:
				break;
		}
	}
	// Check 2 cells RIGHT (P1 pov).
	if(col > 1 && board[row][col-2]/100 == oppPlayer)
	{
		pieceType = (board[row][col-2]/10) % 10;
		direction = board[row][col-2]%10;
		switch(pieceType)
		{
			case PIECE3:	// Opp PIECE3 two cells LEFT.
				if(direction == UPWARD && row == 1)
				{
					isSafe = FALSE;
				}
				else if(direction == DOWNWARD && row == 8)
				{
					isSafe = FALSE;
				}
				break;
			default:
				break;
		}
	}
	// Check attack from DWND LEFT DIAG (P1 pov).
	if(col < MAX_COL - 2 && row < MAX_ROW - 2 && board[row+2][col+2]/100 == oppPlayer)
	{
		pieceType = (board[row+2][col+2]/10) % 10;
		direction = board[row+2][col+2]%10;
		switch(pieceType)
		{
			case PIECE3:	// Opp PIECE3 @  row+2, col+2
				if(direction == UPWARD)
				{
					isSafe = FALSE;
				}
				break;
			default:
				break;
		}
	}
	// Check attack from DWND RIGHT DIAG (P1 pov).
	if(col > 1 && row < MAX_ROW - 2 && board[row+2][col-2]/100 == oppPlayer)
	{
		pieceType = (board[row+2][col-2]/10) % 10;
		direction = board[row+2][col-2]%10;
		switch(pieceType)
		{
			case PIECE3:	// Opp PIECE3 @  row+2, col-2
				if(direction == UPWARD)
				{
					isSafe = FALSE;
				}
				break;
			default:
				break;
		}
	}
	// Check attack from UPWD LEFT DIAG (P1 pov).
	if(col > 1 && row < MAX_ROW - 2 && board[row-2][col+2]/100 == oppPlayer)
	{
		pieceType = (board[row-2][col+2]/10) % 10;
		direction = board[row-2][col+2]%10;
		switch(pieceType)
		{
			case PIECE3:	// Opp PIECE3 @  row-2, col+2
				if(direction == DOWNWARD)
				{
					isSafe = FALSE;
				}
				break;
			default:
				break;
		}
	}
	// Check attack from UPWD RIGHT DIAG (P1 pov).
	if(col > 1 && row > 1 && board[row-2][col-2]/100 == oppPlayer)
	{
		pieceType = (board[row-2][col-2]/10) % 10;
		direction = board[row-2][col-2]%10;
		switch(pieceType)
		{
			case PIECE3:	// Opp PIECE3 @  row-2, col-2
				if(direction == DOWNWARD)
				{
					isSafe = FALSE;
				}
				break;
			default:
				break;
		}
	}
	return isSafe;
}
int getAttackPoints(uint16_t (*board)[MAX_COL], uint16_t row, uint16_t col)
{
	int heuVal = 0, direction;
	pieceType_t pieceType = NO_PIECE;
	
	pieceType = (board[row][col]/10) % 10;
	direction = board[row][col]%10;
	
	switch(pieceType)
	{
		case PIECE1:
			if(row < MAX_ROW - 1 && board[row+1][col]/100 == oppPlayerId)
			{	// FORWARD
				if((board[row+1][col]/10) % 10 == PIECE1)
				{	
					heuVal = 8;
				}
				else
				{
					heuVal =10;
				}
			}
			if(row > 0 && board[row-1][col]/100 == oppPlayerId)
			{	//BACKWARD
				if((board[row-1][col]/10) % 10 == PIECE1)
				{	
					heuVal = 8;
				}
				else
				{
					heuVal =10;
				}
			}
			if(col > 0 && board[row][col-1]/100 == oppPlayerId)
			{	// LEFT
				if((board[row][col-1]/10) % 10 == PIECE1)
				{	
					heuVal = 8;
				}
				else
				{
					heuVal =10;
				}
			}
			if(col < MAX_COL - 1 && board[row][col-1]/100 == oppPlayerId)
			{	// RIGHT
				if((board[row][col+1]/10) % 10 == PIECE1)
				{	
					heuVal = 8;
				}
				else
				{
					heuVal = 10;
				}
			}
			break;
		case PIECE2:
			if(direction == DOWNWARD)
			{	
				if(row < MAX_ROW - 2 && board[row+2][col]/100 == oppPlayerId)
				{ 	// FORWARD
					if((board[row+2][col]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}	
				}
				if(col < MAX_COL - 1 && board[row][col+1]/100 == oppPlayerId)
				{	// LEFT
					if((board[row][col+1]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}
				}
				if(col > 0 && board[row][col-1]/100 == oppPlayerId)
				{	// RIGHT
					if((board[row][col-1]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}
				}
				if(row < MAX_ROW - 1 && board[row+1][col]/100 == oppPlayerId)
				{	// BOUNCE BACK
					if((board[row+1][col]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}	
				}
			}
			else
			{	// DOWNWARD
				if(row > 1 && board[row-2][col]/100 == oppPlayerId)
				{	// FORWARD
					if((board[row-2][col]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}
				}
				if(col > 0 && board[row][col-1]/100 == oppPlayerId)
				{	// LEFT
					if((board[row][col-1]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}
				}
				if(col < MAX_COL - 1 && board[row][col+1]/100 == oppPlayerId)
				{	// RIGHT
					if((board[row][col+1]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}
				}
				if(row > 0 && board[row-2][col]/100 == oppPlayerId)
				{	// BOUNCE BACK
					if((board[row-2][col]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}	
				}
			}
			break;
		case PIECE3:
			if(direction == DOWNWARD)
			{	
				if(row < MAX_ROW - 2 && col < MAX_COL -2 && board[row+2][col+2]/100 == oppPlayerId)
				{	// LEFT DIAG
					if((board[row+2][col+2]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}	
				}
				if(row > 1 && col > 1 && board[row-2][col-2]/100 == oppPlayerId)
				{	// RIGHT DIAG
					if((board[row-2][col-2]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}	
				}
				if((col == 1 || col == 2) && row < MAX_ROW - 2 && board[row+2][col]/100 == oppPlayerId)
			    {	// RIGHT/LEFT EDGE
					if((board[row+2][col]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}
				}
				if((col == 0 || col == 1) && row == MAX_ROW - 2 && board[row][col+2]/100 == oppPlayerId)
			    {   // BOUNCE DOWNWARD LEFT
					if((board[row][col+2]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}
				}
				if((col == 2 || col == 3) && row == MAX_ROW - 2 && board[row][col-2]/100 == oppPlayerId)
			    {   // BOUNCE DOWNWARD RIGHT
					if((board[row][col-2]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}
				}
				if(col == 2 && row == MAX_ROW - 2)
			    {   // BOUNCE BACK LEFT CORNER
					if((board[row+1][col+1]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}
				}
				if(col == 1 && row == MAX_ROW - 2)
			    {   // BOUNCE BACK RIGHT CORNER
					if((board[row+1][col-1]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}
				}
			}
			else
			{	// UPWARD
				if(row > 1 && col > 1 && board[row-2][col-2]/100 == oppPlayerId)
				{	// LEFT DIAG
					if((board[row-2][col-2]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}	
				}
				if(row > 1 && col < MAX_COL - 2 && board[row-2][col+2]/100 == oppPlayerId)
				{	// RIGHT DIAG
					if((board[row-2][col+2]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}	
				}
				if((col == 1 || col == 2) && row > 1 && board[row-2][col]/100 == oppPlayerId)
			    {	// RIGHT/LEFT EDGE
					if((board[row-2][col]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}
				}
				if((col == 2 || col == 3) && row == 1 && board[row][col-2]/100 == oppPlayerId)
			    {   // BOUNCE UPWARD LEFT
					if((board[row][col-2]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}
				}
				if((col == 0 || col == 1) && row == 1 && board[row][col+2]/100 == oppPlayerId)
			    {   // BOUNCE UPWARD RIGHT
					if((board[row][col+2]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}
				}
				if(col == 1 && row == 1)
			    {   // BOUNCE BACK LEFT CORNER
					if((board[row-1][col-1]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}
				}
				if(col == 2 && row == 1)
			    {   // BOUNCE BACK RIGHT CORNER
					if((board[row-1][col+1]/10) % 10 == PIECE1)
					{	
						heuVal = 8;
					}
					else
					{
						heuVal =10;
					}
				}
			}
			break;
		default:
			#ifdef DEBUG2
			printf("heuVal in att DEFAULT!!: %d\n", heuVal);
			#endif
			break;
	}
	#ifdef DEBUG
	printf("heuVal in getAttackPoints: %d\n", heuVal);
	#endif
	return heuVal;
}
int pattern1Val(uint16_t (*board)[MAX_COL])
{
	int heuVal = 0, direction;
	pieceType_t pieceType = NO_PIECE;
	playerId_t playerId = INVALID_PLAYER;
	
	pieceType = (board[2][1]/10)%10;
	direction = board[2][1]%10;
	playerId = board[2][1]/100;
	if(playerId == myPlayerId && pieceType == PIECE3 && direction == DOWNWARD)
	{
		heuVal += 5;
	}
	
	pieceType = (board[2][2]/10)%10;
	direction = board[2][2]%10;
	playerId = board[2][2]/100;
	if(playerId == myPlayerId && pieceType == PIECE3 && direction == DOWNWARD)
	{
		heuVal += 5;
	}
	
	pieceType = (board[3][1]/10)%10;
	direction = board[3][1]%10;
	playerId = board[3][1]/100;
	if(playerId == myPlayerId && pieceType == PIECE2 && direction == DOWNWARD)
	{
		heuVal += 5;
	}
	
	pieceType = (board[3][2]/10)%10;
	direction = board[3][2]%10;
	playerId = board[3][2]/100;
	if(playerId == myPlayerId && pieceType == PIECE2 && direction == DOWNWARD)
	{
		heuVal += 5;
	}
	return heuVal;
#if 0
	BEGIN_ALL_ROWS_AND_COLS
	{
		pieceType = (board[row][col]/10) % 10;
		direction = board[row][col]%10;
		if(board[row][col]/100 == myPlayerId && myPlayerId == 1)
		{
			if(pieceType == PIECE3 && row < 7)
			{
				if((direction == DOWNWARD) && 
				   (board[row+1][col]/10)%10 == PIECE2)
				{
					if(col)
					heuVal += 5;
				}
				else
				{
					
				}
				
			}
		}
		else if(board[row][col]/100 == myPlayerId && myPlayerId == 2)
		{
			
		}
	}
	END_ALL_ROWS_AND_COLS
#endif
}
 
uint8_t checkWin(uint16_t (*board)[MAX_COL], uint8_t isMaxPlayer)
{
	uint16_t row = 0, col = 0;
	uint8_t isWon = TRUE;
	
	for(row = 0; row < MAX_ROW && isWon; row++)
	{
		for(col = 0; col < MAX_COL; col++)
		{
			if(board[row][col] / 100 == oppPlayerId)
			{
				isWon = FALSE;
			}
		}
	}
	return isWon;
}
void printMove(outputMove_t myMove)
{
	printf("%d %d\n", myMove.moveFrom[0], myMove.moveFrom[1]);
    printf("%d %d\n", myMove.moveTo[0], myMove.moveTo[1]);
	#ifdef DEBUG
	printf("\n");
	#endif
}
 
// #ifdef DEBUG2
void printBoard(uint16_t (*board)[MAX_COL])
{
	uint16_t row = 0, col = 0;
	
	BEGIN_ALL_ROWS_AND_COLS
	{
		printf("%d\t", board[row][col]);
	}
	printf("\n");
	END_ALL_ROWS_AND_COLS
}
// #endif
