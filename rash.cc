#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string>
#include <fstream> // write to file, using for time/date in history log
#include <chrono>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <queue>
#include <algorithm> // for remove_if


#define clear() printf("\033[H\033[J")

using namespace std;

void initRash(); // just for fun; got from 
bool cdCheck(const char* cmd, const char* dir);
bool killRash(char* cmd);

// in progress
void splitString(string s, vector<string> v);

// need to implement
void doFork();


int main(int argc, const char * argv[]) {

    initRash();

    if (argc > 1 && string(argv[1]) == "-d") {
        rl_bind_key('\t', rl_insert);
    }

    char* buf;
    const char* command;
    char* args[10];
    vector <string> strVec;
    queue< vector <string> > commandQ;



    // for(int i = 0; i < 10; ++i) {
    //     args[i] = new char[255];
    // }
    // for (auto index : args) {
    //     index = new char[255];
    // }

    using_history();
    read_history("history.txt");
    string thing = "$ ";

    while ((buf = readline(thing.c_str())) != nullptr) {


        if (strlen(buf) > 0) {
            add_history(buf); // for arrow history
            append_history(1, "history.txt");
        }
        else continue;

        if (killRash(buf)) return 0;

        int num = sscanf(buf, "%ms %ms %ms", &args[0], &args[1], &args[2]);
        command = args[0];


        string bufstr(buf);
        splitString(bufstr, strVec);
        // cout << bufstr << endl;
        // vector <const char*> charVec(strVec.size(), nullptr);

        // populate args from strVec
        // for (int i = 0; i < strVec.size(); ++i) {
        //     args[i] = strVec[i].c_str();
        // }

        if (cdCheck(command, args[1])) continue;





        if (bufstr.find("=") != string::npos) { // means we should be setting an env var
            int equalPos = bufstr.find("=");
            string envVar = bufstr.substr(0, equalPos);
            string envSetTo = bufstr.substr(equalPos + 1, bufstr.length());
            if (envVar == "PS1") {
                thing = envSetTo.append(thing);
                continue;
            }
            else setenv(envVar.c_str(), envSetTo.c_str(), 1);
        }
        if (bufstr.find("$") != string::npos) {
            int dollarPos = bufstr.find("$");
            continue;
        }






        // TODO: Methodize fork stuff.
        int status = 0;
        pid_t pid = fork();
        if (pid < 0) { // fail
            cout << "fork pooped\n";
            return 0;
        }
        else if (pid == 0) { // child
            execvp(args[0], args);
            return 0;
        }
        else { // parent
            pid = wait(&status);
        }
        
        
        free(buf);
    }

    return 0;
}

void initRash() {
    cout << "Welcome to RASH\n";
    char* username = getenv("USER");
    cout << "User is: " << username << endl << endl;
    // sleep(1);
    // clear();
}
bool cdCheck(const char* cmd, const char* dir) {
    if (strcmp(cmd, "cd") == 0) {
        int ret = chdir(dir);
        if (ret == -1) cout << "poopy\n";
        else           cout << "changed to: " << dir << "\n";
        return 1;
    }      
    return 0;
}
bool killRash(char* cmd) {
    if (strcmp(cmd, "exit") == 0) {
        cout << "goodbye...\n";
        free(cmd);
        return 1;
    }
    return 0;
}

void splitString(string s, vector<string> v) {
    size_t pos = 0;
    string token;
    
    while ( ((pos = s.find(";")) != string::npos) || ((pos = s.find("&&")) != string::npos) ) {
        token = s.substr(0, pos);
        token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
        v.push_back(token);
        // cout << token << endl;
        // s.erase(0, s.find_first_not_of(' '));
        s.erase(0, pos + 2);
        // cout << s.find_first_not_of(' ') << endl;
        // cout << s.at(s.find_first_not_of(' ')) << endl;

    }
    s.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
    // cout << s << endl;
    v.push_back(s);
}


// If I want to add the time to the history file?
//  auto time = chrono::system_clock::now();
//  auto in_time_t = chrono::system_clock::to_time_t(time);
//  ofstream historyfile;
//  historyfile.open("history.txt", ios_base::app);
//  historyfile << ctime(&in_time_t);
