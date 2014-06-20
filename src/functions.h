/* ========================================================================== */
/* File: functions.h
 *
 * Project name: CS50 Amazing Project
 * Component name: functions header
 *
 * Authors: Gabriel Corso, Tyler Crowe, Joanne Zhao 
 * Date Created: Sat May 24, 2014
 *
 * Last Update by: Joanne Zhao - Sat May 24, 2014
 *
 * This file contains the avatar function defines for the Amazing Project.
 *
 */
/* ========================================================================== */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include <stdint.h>                          // uint32_t

// ---------------- Constants

// ---------------- Structures/Types

// ---------------- Function Headers

/*
 * initializeMaze - initializes the individual maze
 * @width - maze width
 * @height - maze height
 * @num - number of avatars
 * @id - avatarid
 *
 */
MazeNode ***initMaze(int width, int height, int num, int id);

/*
 * printShmaze - prints the shared maze's info
 * @maze - the maze to print from
 * @mazeWidth - the width of the maze
 * @mazeHeight the height of the maze
 * @nAvatars - the number of avatars
 *
 * prints the shared memory maze information, including all discovered walls at each grid cell
 *
 */ 
int printShmaze(MazeNode *maze, int mazeWidth, int mazeHeight, int nAvatars);

/*
 * printMaze - prints the avatars maze's info
 *
 * @maze - the maze to print from
 * @width - the width of the maze
 * @height the height of the maze
 * @nAvatars - the number of avatars
 * @avatarID - the avatar ID 
 *
 * prints the individual maze information, including all discovered walls at each grid cell  
 */  
int printMaze(MazeNode ***maze, int width, int height, int nAvatars, int avatarID);

/*
 * updateMaze - updates the avatar's maze after results from his move come back
 *
 */ 
void updateMaze(MazeNode ***maze, uint32_t direction, XYPos pos, int blocked, int mazeWidth, int mazeHeight, int avatarID);

/*
 * updateShmaze - updates the shared maze with wall information
 *
 */ 
void updateShmaze(MazeNode *maze, uint32_t direction, XYPos pos, int blocked, int mazeWidth, int mazeHeight);

/*
 * getNextMove - calculates the next move using shared memory maze
 *
 */ 
int getNextMove(MazeNode ***maze, XYPos avatarPos, XYPos masterPos, int avatarID, int height, int width, MazeNode *shmaze);

/*
 * move - calculate the next move using just the avatar's instance of the maze
 *
 */ 
int move(MazeNode ***maze, XYPos avatarPos, XYPos masterPos, int avatarID, int height, int width);

/*
 * stuckMove - done only when stuck in a certain place for an extended period of time 
 *             it will choose randomly then go through the directions checking if there
 *             is a wall and if there isn't go that direction
 */
int stuckMove(MazeNode ***maze, XYPos avatarPos, int avatarID, int height, int width);

/*
 * isEdge - checks if the coordinates are on the edge
 *
 */ 
int isEdge(int y, int x, int height, int width);

/*
 * inbounds - checks if the coordinates are inbounds
 *
 * Note: not for the avatar to determine their move
 *       just to check if within bounds so we dont try 
 *       to reference something outside the maze
 *
 */ 
int inbounds(int y, int x, int height, int width);

/*
 * free_maze - frees the memory allocated for the maze
 *
 */  
void free_maze(MazeNode ***maze, int height, int width, int numAvatars);

/*
 * tryDirection - checks if we can move in that direction using shared memory
 *                helper function for getNextMove
 */ 
int tryDirection(MazeNode *shmazeNode, MazeNode ***maze, int mazeWidth, int mazeHeight, int avatarID, int direction, int x, int y);

#endif // FUNCTIONS_H
