/* ========================================================================= */
/* File: graphics.c - the graphisc functionality for the Amazing Project
 *
 * Author: Gabriel Corso, Tyler Crowe, Joanne Zhao
 * Date: May 26, 2014
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
#include <sys/types.h>
#include <string.h>

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
 * printASCII - prints the maze to the console in ASCII characters
 *
 */ 
int printASCII(MazeNode *maze, int mazeWidth, int mazeHeight, int nAvatars, int *trail, XYPos master){

  printf("**************** align ****************\n\n\n");

  int i;
  int j;
  int k;

  //print every node in the map, 3 rows and 3 columns per node	
  for(i = 0; i < mazeHeight; i++){
    //the north row
    for(j = 0; j < mazeWidth; j++){
      //print the 2 columns per node
      printf("+");
      //print the center column
      if ((maze + i*mazeWidth + j)->north == 0) {
	    printf("-");
      }
      else  printf(" ");
    }
    printf("+");
    
    //format line
    printf("\n");
    
    //mid row
    for(j = 0; j < mazeWidth; j++){
      //print the 2 columns per node
      for (k = 0; k < 2; k++){
	
        //print the west column
        if (k == 0) {
	  if ((maze + i*mazeWidth + j)->west == 0) {
	    printf("|");
	  }
	  
	  else printf(" ");
	}
	int crossed = 0;
	int p;
	int path;
        for(p = 0; p < nAvatars; p++){
          if(*(trail + i*mazeWidth*nAvatars + j*nAvatars + p) > 0){
		  path = p;
                  crossed = 1;
          }
        }
        //print the center column
        if (k == 1) {
		if(master.y == i && master.x == j){
			printf("M");
		}
		else if(crossed){
			printf("%d", path);
		} 
		else{
	  		printf(" ");
		}
	}
      }
      //print the last east column
      if (j == mazeWidth-1) {
	if ((maze + i*mazeWidth + j)->east == 0) {
	  printf("|");
	}
	else printf(" ");
      }
      			
    }
    
    //format
    printf("\n");
    if(i == mazeHeight-1){
    	//south row	
    	for(j = 0; j < mazeWidth; j++){
    	  //print the west column
		printf("+");
    	    //print the center column
		  if ((maze + i*mazeWidth + j)->south == 0) {
		    printf("-");
		  }
		  else printf(" ");
 	   }
	  // print the last east column
  	  printf("+");

    	//format line                                                                                                                                                                       
    	printf("\n");
    }
  }



  return 0;
}
