#ifndef _HEADERS__H
#define _HEADERS__H

#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define SIZE_1024 1024
#define BACKLOG 10

void exit_error(const char*);
void process_args(char**);
void get_console(void);
vector<string> get_tokens(char*, char*);
void* run_server(void*);

extern char* tracker_ip;
extern int tracker_port;

#endif