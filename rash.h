#include <readline/readline.h>  // these are here because rash.cc needs it
#include <readline/history.h>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

void initRash();
int binarySearch(std::string s[], int beg, int end, std::string target);
std::vector<std::string> getStrVector(std::string s);
std::vector<char*> getCharVector(std::vector<std::string> v, int* ii);
// std::queue< std::vector<char*> > getExecQueue(std::vector<char*> cv);
std::queue< std::vector<char*> > getExecQueue(std::vector<std::string> sv);
bool cdCheck(const char* cmd, const char* dir);
bool killRash(char* cmd);
// std::vector<std::string> splitString(std::string d, std::string s);
int doExec(char* args[]);
bool doFork(char* args[]);
int expand_and_exec (const char *program, char** options, int rr);
void my_handler(int i);