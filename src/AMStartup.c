/* ========================================================================= */
/* File: AMStartup.c - startup program for the Amazing Project
 *
 * Author: Gabriel Corso, Tyler Crowe, Joanne Zhao
 * Date: May 18, 2014
 *
 * Input:
 * ./AMStartup [NUMBER_AVATARS] [DIFFICULTY] [HOSTNAME]
 *
 * [NUMBER_AVATARS]
 * Number of avatars to place in the maze.
 * Requirements: Must be an integer greater than or equal to 2, less than or 
 * equal to 10.
 *
 * [DIFFICULTY]
 * The difficulty of the maze, determines the max number of moves and the size 
 * of the maze
 * Requirements: Must be an integer between 0 and 9 inclusive.
 *
 * [HOSTNAME]
 * The hostname of the maze server 
 * Requirement: Must be able to establish a connection
 *
 * Command line options: None
 *
 * Output: Alert messages displaying messages sent to and from server.
 * A log file titled with the username, number of avatars, difficulty, 
 * including the mazeport, date and time is generated
 *
 * Error Conditions: any invalid arguments, unable to connect to server
 *
 * Special Considerations:
 *
 */
/* ========================================================================= */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <inttypes.h>
#include <iso646.h>

// ---------------- Local includes  e.g., "file.h"
#include "amazing.h"
#include "maze.h"
#include "functions.h"

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables
char *progname;                // program name, i.e. AMStartup
int nAvatars;                  // number of avatars
int difficulty;                // difficulty of the maze
char *hostName;                // the host name
char *IPAddress;               // the IP address
char *fileName;                // file name for the log file
uint32_t mazePort;             // the maze port
uint32_t mazeWidth;            // width of the maze
uint32_t mazeHeight;           // height of the maze

// ---------------- Private prototypes

/* ========================================================================= */

int main(int argc, char **argv) 
{
  progname = argv[0];
  
  /* Check the arguments */
  if (argc !=4) {
    printf("Usage: AMStartup [nAvatars] [Difficulty] [Hostname]");
    exit(1);
  }

  /* get the number of avatars */
  char *endptr;
  nAvatars = strtod(argv[1], &endptr);
  if (strcmp("", endptr) || !strcmp("",argv[1]) || nAvatars > AM_MAX_AVATAR || nAvatars < AM_MIN_AVATAR ){
    printf("[nAvatars] must be an integer between 2 and 10 inclusive");
    exit(1);
  }

  /* get the difficulty */  
  difficulty = strtod(argv[2], &endptr);
  if (strcmp("", endptr) || !strcmp("",argv[2]) || difficulty > AM_MAX_DIFFICULTY || difficulty < 0 ){
    printf("[difficulty] must be an integer between 0 and 9 inclusive");
    exit(1);
  }

  /* get the host name */
  hostName = (char *)calloc(1, strlen(argv[3])+1);
  snprintf(hostName, strlen(argv[3])+1, "%s", argv[3]);

  /* get the IP address from the host name */
  struct hostent *host;
  if (( host = gethostbyname(hostName)) == NULL){
    printf("Invalid [hostname]");
    free(fileName);
    exit (1);
  }

  struct in_addr **this_in_addr = (struct in_addr **) host->h_addr_list;

  IPAddress= inet_ntoa(*this_in_addr[0]);
 
  printf("%d avatars on level %d\n", nAvatars, difficulty);
 
  /* set up the log file */
  fileName = calloc(strlen(argv[3])+40, sizeof(char));
  snprintf(fileName, strlen(argv[3])+40, "\"AMAZING_\"$USER\"_%d_%d.log\"", nAvatars, difficulty);  
  
  /* creation of the socket */
  
  int sockfd;
  struct sockaddr_in servaddr;

  //Create a socket for the client
  //If sockfd<0 there was an error in the creation of the socket
  if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
    perror("Problem in creating the socket");
    free(fileName);
    free(hostName);
    exit(2);
  }
  
  char *temp;
  uint16_t port = strtoimax(AM_SERVER_PORT, &temp, 10);
  
  //Creation of the socket
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr= inet_addr(IPAddress);
  servaddr.sin_port = htons(port); //convert to big-endian order
  
  //Connection of the client to the socket 
  if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
    perror("Problem connecting to the server");
    free(fileName);
    free(hostName);
    exit(3);
  }
  
  AM_Message initSignal;
  AM_Message initReceivedSignal;  
  
  initSignal.type = htonl(AM_INIT);
  initSignal.init.nAvatars = htonl(nAvatars);
  initSignal.init.Difficulty = htonl(difficulty);

  send(sockfd, &initSignal, sizeof(initSignal), 0);
  
  if (recv(sockfd, &initReceivedSignal, sizeof(initReceivedSignal), 0) == 0){
    //error: server terminated prematurely
    perror("The server terminated prematurely"); 
    free(fileName);
    free(hostName);
    exit(4);
  }
  if(IS_AM_ERROR(initReceivedSignal.type)){
    perror("An error message was returned");
    free(fileName);
    free(hostName);
    exit(5);
  }
  
  printf("Messsage received from the server: "); 
  if(ntohl(initReceivedSignal.type) == AM_INIT_OK){
    printf("AM_INIT_OK\n");
  }
  
  mazePort = ntohl(initReceivedSignal.init_ok.MazePort);
  mazeWidth = ntohl(initReceivedSignal.init_ok.MazeWidth);
  mazeHeight = ntohl(initReceivedSignal.init_ok.MazeHeight);
  
  printf("Mazeport number is: %d\n", mazePort);
  printf("MazeWidth is: %d\n", mazeWidth);
  printf("MazeHeight is: %d\n", mazeHeight);
  
  
  /* make a system call to write the first line of the file */
  char *firstLine = calloc(strlen(fileName) + 40, sizeof(char));
  snprintf(firstLine, strlen(fileName) + 40, "echo \"$USER, %d, `date`\n\" > %s", mazePort, fileName);
  system(firstLine);
  free(firstLine);

  int i;
  pid_t pid;
  
  for(i = 0; i < nAvatars; i++){    
    if(pid == 0){
      continue;
    }
    // start a process for each avatar                                                                     
    pid = fork();
    
    if(pid == -1){
      perror("fork failed");
      free(fileName);
      free(hostName);
      exit(1);
    }
    if(pid == 0){
      printf("Creating avatar %d in child process with pid: %d\n", i, getpid());
      char *executeLine = calloc(strlen(fileName) + 200, sizeof(char));
      snprintf(executeLine, strlen(fileName) + 200, "./avatar %d %d %d %s %d %s %d %d", i, nAvatars, difficulty, IPAddress, mazePort, fileName, mazeWidth, mazeHeight);
      system(executeLine);
      free(executeLine);
    }
    
    else {
    }
  }
  int status = 0;
  pid_t wpid;
  while((wpid = wait(&status)) > 0 ){
  }
  
  free(fileName);
  free(hostName);
  exit(0);
}
