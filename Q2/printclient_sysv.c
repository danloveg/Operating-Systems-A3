// *****************************************************************************
// printclient_sysv.c
//
// Purpose: Add jobs to be "printed" to the print queue
// Author: Daniel Lovegrove
// Version: Mar 18/2018
// *****************************************************************************

#include "manager.h"
#include <semaphore.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>

void insertIntoBoundedBuffer(PrintRequest * req);


int main(int argc, char *argv[]) {
    int shmid, i, filesize;
    char *shmseg;
    long pid = getpid();
    unsigned int randseed = (unsigned int) pid;
    char filename[FILENAME_SIZE];
    PrintRequest request;

    // Locate segment
    if ((shmid = shmget(KEY, SIZE, 0666)) < 0) {
        perror("shmget failed");
        exit(1);
    }

    // Attach segment
    if ((shmseg = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat failed");
        exit(1);
    }

    // Add 6 print jobs to the queue
    for (i = 0; i < NUM_ITERATIONS; i++) {
        // Filesize between MIN and MAX
        filesize = (rand_r(&randseed) % (FILE_SIZE_MAX + 1 - FILE_SIZE_MIN)) + FILE_SIZE_MIN;

        // Put the filename in the buffer, then duplicate it to a new string
        sprintf(filename, "File-%ld-%d", pid, i);

        // Create the request job
        request.clientID = pid;
        strcpy(request.filename, filename);
        request.fileSize = filesize;

        insertIntoBoundedBuffer(&request);

        // Sleep for up to MAX seconds
        sleep(rand_r(&randseed) % (SLEEP_TIME_MAX + 1));
    }

    // Exit when done
    printf("Client exiting.\n");

    exit(0);
}

// TODO: Complete
void insertIntoBoundedBuffer(PrintRequest * req) {

}
