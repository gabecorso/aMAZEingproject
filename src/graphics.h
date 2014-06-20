/* ========================================================================== */
/* File: graphics.h
 *
 * Project name: CS50 Amazing Project
 * Component name: graphics header
 *
 * Authors: Gabriel Corso, Tyler Crowe, Joanne Zhao 
 * Date Created: Sat May 24, 2014
 *
 * Last Update by: Joanne Zhao - Sat May 24, 2014
 *
 * This file contains the graphics function defines for the Amazing Project.
 *
 */
/* ========================================================================== */
#ifndef GRAPHICS_H
#define GRAPHICS_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include <stdint.h>                          // uint32_t

// ---------------- Constants
#define WALL 64
#define AVATAR 42
#define SPACE 32
// ---------------- Structures/Types

// ---------------- Function Headers

/*
 * printASCII - prints the maze to the console in ASCII characters
 */ 
int printASCII(MazeNode *maze, int mazeWidth, int mazeHeight, int nAvatars, int *trail, XYPos master);

#endif // GRAPHICS_H
