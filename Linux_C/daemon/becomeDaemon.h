#ifndef __DAEMON_H__
#define __DAEMON_H__
const int  NoChdir = 01;
const int NoCloseFiles = 02;
const int NoReopenStdFds = 04;
const int NoUmask = 010;
const int MaxClose = 8192;
int becomeDaemon (int flags); 

#endif


