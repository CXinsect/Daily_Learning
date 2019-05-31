//#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <string>
//#include <fstream.h>
void printSigset(FILE *of,const char *s,const sigset_t *sigset);
int printSigMask(FILE *of,const char *msg);
int printPendinfSigs(FILE *of,const char *msg);