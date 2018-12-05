#include <readline/readline.h>  // these are here because rash.cc needs it
#include <readline/history.h>
#include <vector>
#include <string>
#include <queue>

void initRash();
std::vector<std::string> getStrVector(std::string s);
std::vector<char*> getCharVector(std::vector<std::string> v);
// std::queue< std::vector<char*> > getExecQueue(std::vector<char*> cv);
std::queue< std::vector<char*> > getExecQueue(std::vector<std::string> sv);

bool cdCheck(const char* cmd, const char* dir);
bool killRash(char* cmd);
// std::vector<std::string> splitString(std::string d, std::string s);
void doExec(char* args[]);
bool doFork(char* args[]);
int expand_and_exec (const char *program, char** options);
void my_handler(int i);