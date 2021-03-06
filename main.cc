#include <stdlib.h>
#include <cstdlib>
#include <signal.h>
#include <iostream>
#include "rash.h"

using namespace std;

int main(int argc, const char * argv[]) {

    initRash();

    if (argc > 1 && string(argv[1]) == "-d") {
        rl_bind_key('\t', rl_insert);
    }

    char* buf;
    string thing = "$ ";
    using_history();
    read_history("history.txt");

    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    while ((buf = readline(thing.c_str())) != nullptr) {
    // while ((buf = readline(getenv("PS1"))) != nullptr) {


        queue< vector <char*> > commandQueue;

        if (strlen(buf) > 0) {
            add_history(buf); // for arrow history
            append_history(1, "history.txt");
        }
        else continue;

        if (killRash(buf)) return 0;
        
        string bufstr(buf);
        int r = 0;
        vector<string> strVec = getStrVector(bufstr);
        vector<char*>  args   = getCharVector(strVec, &r);

        // commandQueue = getExecQueue(strVec);


        
        // while (!commandQueue.empty()) {
        //     vector<char*> something = commandQueue.front();
        //     for (auto item : something) {
        //         cout << item << endl;
        //     }
        //     commandQueue.pop();
        // }

        if (bufstr.find("=") != string::npos) { 
            // means we should be setting an env var

            int equalPos = bufstr.find("=");
            string envVar = bufstr.substr(0, equalPos);
            string envSetTo = bufstr.substr(equalPos + 1, 
                                            bufstr.length());
            if (envVar == "PS1") {
                thing = envSetTo.append(thing);
                continue;
            }
            else setenv(envVar.c_str(), envSetTo.c_str(), 1);
            // else putenv((char*)envVar.c_str());
        }
        else if (!expand_and_exec(args[0], &args[0], r)) return 0;

        free(buf);
    }

    return 0;
}