This is the source code which I used in an online Tournament named "Battle of Bots #9" on hackerearth.com
I secured 91.98 Points and 8th rank.

Here are the rules:

Taunt is a two player board game which is played on a  grid of cells and is played on opposite sides of the game-board. Each player has an allocated color, Orange ( First Player ) or Green ( Second Player ) being conventional. Each player has nine piece in total. The players move their pieces towards to his / her opponent’s area by moving their pieces strategically. Each piece has a different moving feature and is one of the 3 types of pieces.

Piece 1: It can move to horizontally or vertically adjacent cell, if the cell doesn't contain a piece of same color.


Piece 2: It can move to horizontally adjacent cell or can move two steps forward, if the cell doesn't contain a piece of same color (except the piece itself).

This type of piece can move to its own position if its in the second last row of the grid and going downward or if its in the second row of the grid and going upward.


Piece 3: It can move two step diagonally in the forward direction, if the cell doesn't contain a piece of same color (except the piece itself).

This type of piece can move to its own position if its in the second last row of the grid and going downward or if its in the second row of the grid and going upward.


Players take turns involving moves of pieces as mentioned above and can captures opponent's piece by jumping on or over opponent's pieces.

Note: Forward direction for first player is downward and for second player is upward.

If a piece (except piece 1) is moving downward and touches the last row, its direction will change i.e. now it will move upward. Similarly, once if a piece (except piece 1) is moving upward and touches the first row, its direction will change i.e. now it will move downward.

Rules:

Player can only move according to the moves mentioned above.
A player may not move an opponent's piece.
A player can captures opponent's piece by jumping on or over opponent pieces.
The game will end after  moves (  moves for each player ) or when any of the players don't have any move left. At the end of the game the player with majority of pieces will win.

We will play it on an  grid. The top left of the grid is  and the bottom right is .
