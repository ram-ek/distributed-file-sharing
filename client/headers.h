#ifndef _HEADERS__H
#define _HEADERS__H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

#define SIZE_1024 1024
#define MAX_CNCT_TRY 10

void exit_error(string);
void process_args(char**);
void get_console(void);
vector<string> get_tokens(char*, char*);
void connect_tracker(void);

extern int sock_in;
extern map<int, set<pair<string, string> > > tracker_list;

#endif