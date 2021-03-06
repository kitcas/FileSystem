/**************************************************************
* Class: CSC-415-0# Spring 2020
* Group Name: Last Minute
* Name: Pedro Souto
* Student ID: 918412864
* Name:
* Student ID:
* Name:
* Student ID:
* Name:
* Student ID:
*
* Project: Assignment 3 – File System
* File: inputParser.c
*
* Description: Implementation of all functionalities the user parser  uses.
* **************************************************************/

// Required C libraries
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Group Files
#include "inputParser.h"
#include "fsImplementation.h"

void executeCommand (int argc, char *argv[], uint64_t blockSize) {
    // Find the corresponding command, and call its proper execute function
    if(strcmp(argv[0],"ls") == 0) {
        listDirectories(getVCBCurrentDirectory(blockSize), blockSize);
    }
    else if (strcmp(argv[0],"tree") == 0) {
        listTree(getVCBCurrentDirectory(blockSize), blockSize);
    }
    else if (strcmp(argv[0],"cd") == 0) {
        changeDirectory(argv[1], 0, blockSize);
    }
    else if (strcmp(argv[0],"pwd") == 0) {
        displayCurrentDirectory(blockSize);
    }
    else if (strcmp(argv[0],"mkdir") == 0) {
        createDirectory(argv[1], getVCBCurrentDirectory(blockSize), blockSize);
    }
    else if (strcmp(argv[0],"mkfile") == 0) {
        // Make sure that the file size argument is a number
        for (int i = 0; i < strlen(argv[3]) ; i++) {
            // Check if the ASCII value is between 48 and 57, which corresponds to 0-9
            if (argv[3][i] < 48 || argv[3][i] > 57) {
                printf("Invalid Argument. File Size Must Be a Number.\n\n");
                return;
            }
        }
        createFileDirectory(argv[1], argv[2], atoi(argv[3]), getVCBCurrentDirectory(blockSize), blockSize);
    }
    else if (strcmp(argv[0],"exit") == 0 || strcmp(argv[0],"e") == 0 || strcmp(argv[0],"Exit") == 0 || strcmp(argv[0],"E") == 0) {
        exitFileSystem(blockSize);
    }
    else if (strcmp(argv[0],"commands") == 0 || strcmp(argv[0],"C") == 0 || strcmp(argv[0],"Commands") == 0 || strcmp(argv[0],"C") == 0) {
        printCommands();
    }
}

int userInputIsValid (int argc, char *argv[]) {
    // Array of valid commands
    const char * validCommands[] = {
        "ls",
        "tree",
        "cd",
        "pwd",
        "mkdir",
        "mkfile",
        "rmfile",
        "cpyfile",
        "mvfile",
        "chmod",
        "TODO: Copy from the normal filesystem to this filesystem",
        "TODO: Copy from this filesystem to the normal filesystem",
        "commands",
        "c",
        "Commands",
        "C",
        "exit",
        "e",
        "Exit",
        "E",
    };
    
    // Array listing the number of arguments each of the above commands should have. They are ordered, so think of this as a key value pair
    const int numberOfArgumentsPerCommand[] = {
        0,  // ls
        0,  // tree
        1,  // cd
        0,  // pwd
        1,  // mkdir
        3,  // mkfile
        1,  // rmdile
        2,  // cpyfile
        2,  // mvfile
        2,  // setdata
        99, // TODO: Copy from the normal filesystem to this filesystem
        99, // TODO: Copy from this filesystem to the normal filesystem
        0,  // commands
        0,  // c
        0,  // Commands
        0,  // C
        0,  // exit
        0,  // e
        0,  // Exit
        0,  // E
    };
    
    // Number of possible commands. Comes from the above array
    int numberOfCommands = (sizeof(numberOfArgumentsPerCommand) / sizeof(numberOfArgumentsPerCommand[0]));
    
    // Check that the first argument is a valid command from the list
    for (int i = 0; i < numberOfCommands; i++) {
        // Check to make sure that the command is valid
        if (strcmp(validCommands[i], argv[0]) == 0) {
            // Check to make sure that the command has the correct number of arguments
            if ((argc - 1) == numberOfArgumentsPerCommand[i]) {
                return 1;
            }
            // If the command is valid, but the arguments are not, report that as a failure
            else {
                printf("Invalid Arguments.\n\n");
                return 0;
            }
        }
        // If we checked all valid commands, and none matched, then this is not a valid command
        else if (i == (numberOfCommands - 1)){
            printf("Invalid Command.\n\n");
            return 0;
        }
    }
    
    // Failsafe catch, return 0
    return 0;
}
