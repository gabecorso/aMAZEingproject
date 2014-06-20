/* ========================================================================== */
/* File: maze.h
 *
 * Project name: CS50 Amazing Project
 * Component name: maze header
 *
 * Authors: Gabriel Corso, Tyler Crowe, Joanne Zhao 
 * Date Created: Fri May 23, 2014
 *
 * Last Update by: Joanne Zhao - Fri May 23, 2014
 *
 * This file contains the maze defines for the Amazing Project.
 *
 */
/* ========================================================================== */
#ifndef MAZE_H
#define MAZE_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include <stdint.h>                          // uint32_t
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

// ---------------- Constants
/* Additional Server parameters */
#define AM_MIN_AVATAR        2               // min # of avatars for any MazePort

/* Maze Constants */
#define MAX_HEIGHT       100
#define MAX_WIDTH        100

static const key_t SHMKEY = 343434;
static const key_t SHMKEY2 = 434343;

// ---------------- Structures/Types
/* node in a maze */
typedef struct MazeNode
{
  int north;
  int south;
  int east;
  int west;
  int *paths;
  XYPos pos; 
} MazeNode;

/* Maze */
typedef struct Maze
{
  MazeNode **maze[MAX_WIDTH][MAX_HEIGHT];   
} Maze;

#endif // MAZE_H
