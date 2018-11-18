#include <iostream>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <fstream> // write to file, using for time/date in history log
#include <chrono>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define clear() printf("\033[H\033[J")

using namespace std;

void initRash(); // just for fun; got from 
void cdCheck(const char* cmd, char* dir);
bool killRash(char* cmd);

// need to implement
void splitString(string* s);


int main(int argc, const char * argv[]) {

    initRash();

    if (argc > 1 && string(argv[1]) == "-d") {
        rl_bind_key('\t', rl_insert);
    }

    char* buf;
    const char* command;
    char* args[10];


    // for(int i = 0; i < 10; ++i) {
    //     args[i] = new char[255];
    // }

    for (auto index : args) {
        index = new char[255];
    }

    using_history();
    read_history("history.txt");

    while ((buf = readline("$ ")) != nullptr) {

        string str(buf);
        // if (str.length() < 1) continue; 

        if (strlen(buf) > 0) {
            add_history(buf); // for arrow history
            append_history(1, "history.txt");
        }
        else continue;

        if (killRash(buf)) return 0;


        int num = sscanf(buf, "%ms %ms %ms", &args[0], &args[1], &args[2]);
        command = args[0];


        cdCheck(command, args[1]);

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

void cdCheck(const char* cmd, char* dir) {
    if (strcmp(cmd, "cd") == 0) {
        int ret = chdir(dir);
        if (ret == -1) cout << "poopy\n";
        else           cout << "changed to: " << dir << "\n";
    }      
}

bool killRash(char* cmd) {
    if (strcmp(cmd, "exit") == 0) {
        cout << "goodbye...\n";
        free(cmd);
        return 1;
    }
    return 0;
}

// If I want to add the time to the history file?
//  auto time = chrono::system_clock::now();
//  auto in_time_t = chrono::system_clock::to_time_t(time);
//  ofstream historyfile;
//  historyfile.open("history.txt", ios_base::app);
//  historyfile << ctime(&in_time_t);
