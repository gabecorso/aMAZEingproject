/* ========================================================================= */
/* File: functions.c - the avatar functionality for the Amazing Project
 *
 * Author: Gabriel Corso, Tyler Crowe, Joanne Zhao
 * Date: May 24, 2014
 *
 * Input: N/A
 *
 * Command line options: None
 *
 * Output: N/A
 *
 * Error Conditions:
 *
 * Special Considerations:
 *
 */
/* ========================================================================= */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

// ---------------- Local includes  e.g., "file.h"
#include "amazing.h"
#include "maze.h"
#include "functions.h"

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/* ========================================================================= */

/*
 * initMaze - initializes the maze with empty node and undiscovered walls
 *
 */ 
MazeNode*** initMaze(int width, int height, int num, int id){
  /* initialize the maze */
  MazeNode ***maze = calloc(width, sizeof(MazeNode));
  int y;
  int x;
  for(y = 0; y < height; y++){
    maze[y] = calloc (height, sizeof(MazeNode));
    for(x = 0; x < width; x++){
      maze[y][x] = calloc(1, sizeof(MazeNode));
      maze[y][x]->north = 1;
      maze[y][x]->south= 1;
      maze[y][x]->east= 1;
      maze[y][x]->west= 1;
      maze[y][x]->pos.x = x;
      maze[y][x]->pos.y = y;
      maze[y][x]->paths = calloc(num, sizeof(int));
      for (int k = 0; k < num; k ++ ){
	maze[y][x]->paths[k] = 0;
      }
    }
  }
  return maze;
}

/*
 * printShmaze - prints info about the shared memory maze
 *
 */ 
int printShmaze(MazeNode *maze, int mazeWidth, int mazeHeight, int nAvatars){
  int i;
  int j;
  for(i = 0; i < mazeHeight; i++){
    for(j = 0; j < mazeWidth; j++){
      printf("ShmazeNode [%02i][%02i]: walls: ",(maze + i*mazeWidth + j)->pos.y, (maze + i*mazeWidth + j)->pos.x);
      if ((maze + i*mazeWidth + j)->west == 0) printf("W");
      else printf(" ");
      if ((maze + i*mazeWidth + j)->north == 0) printf("N");
      else printf(" ");
      if ((maze + i*mazeWidth + j)->south == 0) printf("S");
      else printf(" ");
      if ((maze + i*mazeWidth + j)->east == 0) printf("E");
      else printf(" ");
      printf("  borders: ");
      if((maze + i*mazeWidth + j)->pos.x==0) printf("W");
      else printf(" ");
      if((maze + i*mazeWidth + j)->pos.y==0) printf("N");
      else printf(" ");
      if((maze + i*mazeWidth + j)->pos.y==mazeHeight-1) printf("S");
      else printf(" ");
      if((maze + i*mazeWidth + j)->pos.x==mazeWidth-1) printf("E");
      else printf(" ");

      printf("\n");
    }
  }
  return 0;
}

/*
 * printMaze - prints info about the avatar's maze
 *
 */ 
int printMaze(MazeNode ***maze, int width, int height, int nAvatars, int avatarID){
  int i;
  int j;
  printf("\n");
  for(i = 0; i < height; i++){
    for(j = 0; j < width; j++){
      printf("MazeNode [%02i][%02i]: walls: ", maze[i][j]->pos.y, maze[i][j]->pos.x);
      if (maze[i][j]->west == 0) printf("W");
      else printf(" ");
      if (maze[i][j]->north == 0) printf("N");
      else printf(" ");
      if (maze[i][j]->south == 0) printf("S");
      else printf(" ");
      if (maze[i][j]->east == 0) printf("E");
      else printf(" ");
      printf("  borders: ");
      if(maze[i][j]->pos.x==0) printf("W");
      else printf(" ");
      if(maze[i][j]->pos.y==0) printf("N");
      else printf(" ");
      if(maze[i][j]->pos.y==height-1) printf("S");
      else printf(" ");
      if(maze[i][j]->pos.x==width-1) printf("E");
      else printf(" ");
      printf(" path is %d", maze[i][j]->paths[avatarID]);
      printf("\n");
    }
  }
  return 0;
}

/*
 * updateMaze - updates the avatars maze
 *
 */ 
void updateMaze(MazeNode ***maze, uint32_t direction, XYPos pos, int blocked, int mazeWidth, int mazeHeight, int avatarID){
  /* place walls */
  if(blocked == 0){
    if (direction == M_WEST){
      maze[pos.y][pos.x]->west = 0; // place a west wall
      if (pos.x > 0) {
	 maze[pos.y][pos.x-1]->east = 0; // place an east wall in the adjacent node
      }
    }
    if (direction == M_NORTH){
      maze[pos.y][pos.x]->north = 0; // place a north wall
      if (pos.y> 0) {
	maze[pos.y-1][pos.x]->south = 0;// place a south wall in the adjacent node
      } 
    }
    if (direction == M_SOUTH){
      maze[pos.y][pos.x]->south = 0; 
      if(pos.y<mazeHeight-1){
	maze[pos.y+1][pos.x]->north = 0;
      }
    }
    if (direction == M_EAST){
      maze[pos.y][pos.x]->east = 0;
      if(pos.x<mazeWidth-1){
	maze[pos.y][pos.x+1]->west = 0;
      }
    }
  }
}
/*
 * updateShmaze - updates the shared memory maze
 *
 */ 
void updateShmaze(MazeNode *maze, uint32_t direction, XYPos pos, int blocked, int mazeWidth, int mazeHeight){
  /* place walls */
  if(blocked == 0){
    if (direction == M_WEST){
      (maze + pos.y*mazeWidth + pos.x)->west = 0; // place a west wall
      if (pos.x > 0) {
	(maze + pos.y*mazeWidth + pos.x - 1)->east = 0;  // place an east wall in the adjacent node
      }
    }
    if (direction == M_NORTH){
      (maze + pos.y*mazeWidth + pos.x)->north = 0; // place a north wall
      if (pos.y> 0) {
	(maze + (pos.y-1)*mazeWidth + pos.x)->south = 0;// place a south wall in the adjacent node
      } 
    }
    if (direction == M_SOUTH){
      (maze + pos.y*mazeWidth + pos.x)->south = 0; 
      if(pos.y<mazeHeight-1){
	(maze + (pos.y+1)*mazeWidth + pos.x)->north = 0;
      }
    }
    if (direction == M_EAST){
      (maze + pos.y*mazeWidth + pos.x)->east = 0;
      if(pos.x<mazeWidth-1){
	(maze + pos.y*mazeWidth + pos.x + 1)->west = 0;
      }
    }
  }
}

/*
 * free_maze - frees the avatar's maze
 *
 */ 
void free_maze(MazeNode ***maze, int height, int width, int numAvatars){
  int i;
  int j;
  for(i = 0; i < height; i++){
    for(j = 0; j < width; j++){
      free(maze[i][j]->paths);
      free(maze[i][j]);
    }
    free(maze[i]);
  }
  free(maze);
}

/*
 * getNextMove - calculates the next move for the avatar
 *
 */ 
int getNextMove(MazeNode ***maze, XYPos avatarPos, XYPos masterPos, int avatarID, int mazeHeight, int mazeWidth, MazeNode *shmaze){
  int x = avatarPos.x;
  int y = avatarPos.y;
  int xDiff = x - masterPos.x;
  int yDiff = y - masterPos.y;
  MazeNode *shmazeNode = (shmaze + y*mazeWidth +x);
  // check if we have found the master avatar

  if(xDiff == 0 && yDiff == 0){
    return -1;
  }
  
  if(abs(xDiff) >= abs(yDiff)){
    if(xDiff>0){
      if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_WEST,x,y)==M_WEST) return M_WEST;
      if(yDiff<0){
	if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_SOUTH,x,y)==M_SOUTH) return M_SOUTH;
      }
      if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_NORTH,x,y)==M_NORTH) return M_NORTH;
      if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_SOUTH,x,y)==M_SOUTH) return M_SOUTH;
      if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_EAST,x,y)==M_EAST) return M_EAST;
    }

    else{
      if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_WEST,x,y)==M_WEST) return M_WEST;
      if(yDiff<0){
	if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_SOUTH,x,y)==M_SOUTH) return M_SOUTH;
      }
      if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_NORTH,x,y)==M_NORTH) return M_NORTH;
      if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_SOUTH,x,y)==M_SOUTH) return M_SOUTH;
      if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_WEST,x,y)==M_WEST) return M_WEST;
    }
  }
  if(yDiff<0){
    if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_SOUTH,x,y)==M_SOUTH) return M_SOUTH;
    if(xDiff>0){
      if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_WEST,x,y)==M_WEST) return M_WEST;
    }
    if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_EAST,x,y)==M_EAST) return M_EAST;
    if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_WEST,x,y)==M_WEST) return M_WEST;
    if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_NORTH,x,y)==M_NORTH) return M_NORTH;
  }
  else{
    if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_NORTH,x,y)==M_NORTH) return M_NORTH;
    if(xDiff>0){
      if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_WEST,x,y)==M_WEST) return M_WEST;
    }
    if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_EAST,x,y)==M_EAST) return M_EAST;
    if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_WEST,x,y)==M_WEST) return M_WEST;
    if(tryDirection(shmazeNode, maze, mazeWidth, mazeHeight, avatarID, M_SOUTH,x,y)==M_SOUTH) return M_SOUTH;
  }      

  // at this point, all undiscovered paths/borders have been attempted. It is time to backtrack.
  
  if (shmazeNode->west){
    if (maze[y][x-1]->paths[avatarID] < 2)
      return M_WEST; // in bounds and has a 1, GO WEST!
  }
  
  if (shmazeNode->north){
    if (maze[y-1][x]->paths[avatarID] < 2)
      return M_NORTH; // in bounds and has a 1, GO NORTH!
  }
  
  if (shmazeNode->south){
    if (maze[y+1][x]->paths[avatarID] < 2)
      return M_SOUTH; // in bounds and has a 1, GO SOUTH!
  }
  
  if (shmazeNode->east){
    if (maze[y][x+1]->paths[avatarID] < 2)
      return M_EAST; // in bounds and has a 1, GO EAST!
  }

  // final default: just check if there are walls
  if (shmazeNode->west) 
    return M_WEST; // time to find some walls
  if (shmazeNode->north) 
    return M_NORTH; // time to find some walls
  if (shmazeNode->south) 
    return M_SOUTH; // time to find some walls
  if (shmazeNode->east)
    return M_EAST; // time to find some walls
  return -100; // ALL THINGS FAILED.... impossibru
}

/*
 * inbounds - checks if the coordinates are in bounds
 *
 * NOTE: not for avatar just so we don't look into the
 *       avatar's "memory maze" out of bounds
 *
 */ 
int inbounds(int y, int x, int height, int width){
  if(x < 0 || x >= width){
    return 0;
  }
  if(y < 0 || y >= height){
    return 0;
  }
  return 1;
}
/*
 * isEdge - checks if the corrdinates are on the edge
 *
 */ 
int isEdge(int y, int x, int height, int width){
  if(x-1 < 0 || x+1 >= width){
    return 1;
  }
  if(y-1 < 0 || y+1 >= height){
    return 1;
  }
  return 0;
}

/*
 * stuckMove - done only when stuck in a certain place for an extended period of time
 * 	       it will choose randomly then go through the directions checking if there
 * 	       is a wall and if there isn't go that direction
 */ 
int stuckMove(MazeNode ***maze, XYPos avatarPos, int avatarID, int height, int width){
  MazeNode *current = maze[avatarPos.y][avatarPos.x];
  int order = rand() % 4;
  switch (order){
  case 0:
    if(current->north) return 1;
    if(current->south) return 0;
    if(current->west) return 2;
    if(current->east) return 3;
    break;
  case 1:
    if(current->south) return 1;
    if(current->west) return 0;
    if(current->east) return 2;
    if(current->north) return 3;
    break;
  case 2:
    if(current->east) return 1;
    if(current->west) return 0;
    if(current->north) return 2;
    if(current->south) return 3;
    break;

  case 3:
    if(current->west) return 1;
    if(current->south) return 0;
    if(current->north) return 2;
    if(current->east) return 3;
    break;
  }
  return -1;
}

int tryDirection(MazeNode *shmazeNode, MazeNode ***maze, int width, int height, int avatarID, int direction, int x, int y){
  switch(direction){
  case M_WEST:
    if (shmazeNode->west){
      if( inbounds(y, x-1, height, width)){
	if (maze[y][x-1]->paths[avatarID] < 1)
	  return M_WEST;
      }
      else return M_WEST; // it’s an undiscovered border
    }
    break;
  case M_EAST:
    if (shmazeNode->east){
      if( inbounds(y, x+1, height, width)){
	if (maze[y][x+1]->paths[avatarID] < 1)
	  return M_EAST;
      }
      else return M_EAST; // it’s an undiscovered border
    }
    break;
  case M_SOUTH:
    if (shmazeNode->south){
      if( inbounds(y+1, x, height, width)){
	if (maze[y+1][x]->paths[avatarID] < 1)
	  return M_SOUTH;
      }
      else return M_SOUTH; // it’s an undiscovered border
    }
    break;
  case M_NORTH:
    if (shmazeNode->north){
      if( inbounds(y-1, x, height, width)){
	if (maze[y-1][x]->paths[avatarID] < 1)
	  return M_NORTH;
      }
      else return M_NORTH; // it’s an undiscovered border
    }
    break;
  }
  return -1;
}       



/*
 * move - calculates the move for the avatar to take
 */ 
int move(MazeNode ***maze, XYPos avatarPos, XYPos masterPos, int avatarID, int height, int width){
  int x = avatarPos.x;
  int y = avatarPos.y;
  int xDiff = x - masterPos.x;
  int yDiff = y - masterPos.y;
  MazeNode *avatarNode = maze[y][x];
  // check if we have found the master avatar

  if(xDiff == 0 && yDiff == 0){
    return -1;
  }
  
  if (avatarNode->west){
    if( inbounds(y, x-1, height, width)){
      if (maze[y][x-1]->paths[avatarID] < 1)
	return M_WEST; // in bounds and untraveled, GO WEST!
    }
    else return M_WEST; // it’s an undiscovered border
  }
  
  if (avatarNode->north){ // will pass if no north wall
    if ( inbounds(y-1, x, height, width )){
      if (maze[y-1][x]->paths[avatarID] < 1)
	return M_NORTH; // in bounds and untraveled, GO NORTH!
    }
    else return M_NORTH; // it’s an undiscovered border
  }
  
  if (avatarNode->south){ // will pass if no south wall
    if ( inbounds(y+1, x, height, width )){
      if (maze[y+1][x]->paths[avatarID] < 1)
	return M_SOUTH; // in bounds and untraveled, GO SOUTH!
    }
    else return M_SOUTH; // it’s an undiscovered border
  }
  
  if (avatarNode->east){ // will pass if no east wall
    if ( inbounds(y, x+1, height, width )){
      if (maze[y][x+1]->paths[avatarID] < 1)
	return M_EAST; // in bounds and untraveled, GO EAST!
    }
    else return M_EAST; // it’s an undiscovered border
  }
  
  // at this point, all undiscovered paths/borders have been attempted. It is time to backtrack.
  
  if (avatarNode->west){
    if (maze[y][x-1]->paths[avatarID] < 2)
      return M_WEST; // in bounds and has a 1, GO WEST!
  }
  
  if (avatarNode->north){
    if (maze[y-1][x]->paths[avatarID] < 2)
      return M_NORTH; // in bounds and has a 1, GO NORTH!
  }
  
  if (avatarNode->south){
    if (maze[y+1][x]->paths[avatarID] < 2)
      return M_SOUTH; // in bounds and has a 1, GO SOUTH!
  }
  
  if (avatarNode->east){
    if (maze[y][x+1]->paths[avatarID] < 2)
      return M_EAST; // in bounds and has a 1, GO EAST!
  }

  // final default: just check if there are walls
  if (avatarNode->west){
    return M_WEST; // time to find some walls
  }

  if (avatarNode->north){
    return M_NORTH; // time to find some walls
  }

  if (avatarNode->south){
    return M_SOUTH; // time to find some walls
  }

  if (avatarNode->east){
    return M_EAST; // time to find some walls
  }


return -100;
}
