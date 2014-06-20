# AMStartup.makefile
CC = gcc
CFLAGS = -ggdb -Wall -pedantic -std=c11 -D_SVID_SOURCE
SRC_DIR = src

#UTILDIR=../util/
#UTILFLAG=-ltseutil
#UTILLIB=$(UTILDIR)libtseutil.a
#UTILC=$(UTILDIR)file.c $(UTILDIR)hashtable.c $(UTILDIR)list.c $(UTILDIR)querylist.c $(UTILDIR)utils.c
#UTILH=$(UTILC:.c=.h)

all: AMStartup avatar

AMStartup: $(SRC_DIR)/AMStartup.c $(SRC_DIR)/amazing.h $(SRC_DIR)/maze.h $(SRC_DIR)/functions.h
	$(CC) $(CFLAGS) -o AMStartup $(SRC_DIR)/AMStartup.c $(SRC_DIR)/functions.c

avatar: $(SRC_DIR)/avatar.c $(SRC_DIR)/amazing.h $(SRC_DIR)/maze.h $(SRC_DIR)/functions.c $(SRC_DIR)/functions.h $(SRC_DIR)/graphics.h $(SRC_DIR)/graphics.c
	$(CC) $(CFLAGS) -o avatar $(SRC_DIR)/avatar.c $(SRC_DIR)/functions.c $(SRC_DIR)/graphics.c


.SILENT:

clean:
	rm -f $(SRC_DIR)/*~
	rm -f $(SRC_DIR)/*#
	rm -f core.*
	rm -f *~
	rm -f AMStartup	
	rm -f avatar
