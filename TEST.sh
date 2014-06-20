#!/bin/bash
# Script name: TEST.sh
#
# Script for testing the arguments given to AMStartup
#

f=test_maze.log

# beginning time stamp
start=`date`
starttime=`date +%H:%M:%S`
startdate=`date +%m/%d/%Y`

echo "Amazing Test Log" > $f
echo -e Started at $starttime on $startdate".\n" >> $f

echo -e "make" >> $f
make

# test arguments
echo -e "AMStartup 1 4 pierce.cs.dartmouth.edu whoops" >> $f
AMStartup 1 4 pierce.cs.dartmouth.edu whoops >> $f

echo -e "\n\nAMStartup 1 4 pierce.cs.dartmouth.edu" >> $f
AMStartup 1 4 pierce.cs.dartmouth.edu >> $f

echo -e "\n\nAMStartup 1 10 pierce.cs.dartmouth.edu" >> $f
AMStartup 1 10 pierce.cs.dartmouth.edu >> $f

echo -e"\n\nAMStartup 1 10 ierce.cs.dartmouth.edu" >> $f
AMStartup 1 10 ierce.cs.dartmouth.edu >> $f

echo -e "\n\nAMStartup 2 0 pierce.cs.dartmouth.edu" >> $f
AMStartup 2 0 pierce.cs.dartmouth.edu

echo -e "\n"make clean >> $f
make clean >> $f

# end time stamp
stop=`date`
stoptime=`date +%H:%M:%S`
stopdate=`date +%m/%d/%Y`

echo -e "\n"Finished at $stoptime on $stopdate".\n" >> $f
