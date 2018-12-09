#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wordexp.h>
#include <sstream>
#include <iterator>
#include <iostream>
#include <math.h>
#include <signal.h>
#include "rash.h"


#define clearScreen() printf("\033[H\033[J")
using namespace std;

string allowed_cmds[] = {"nano", "vi", "vim", "touch", "top", "sleep", "ls", "mv", "cp", "cat", "printenv", "env", "echo"};
// sort(begin(allowed_cmds), end(allowed_cmds));

// Welcome user to my rash, clears screen and gets current user.
void initRash() {
    // clearScreen();
    cout << "Welcome to RASH\n";
    char* username = getenv("USER");
    cout << "User is: " << username << endl << endl;
    // sleep(1);
    
}

int binarySearch(string v[], int beg, int end, string target) {

	int m = floor((beg + end)/2);
    cout << v[m] << "  " << target << endl;
	if(beg > end) {
		return -1;
	}
	else if (v[m] == target) {
        cout << v[m] << " == " << target << endl;
		return m;
	}
	else if (v[m] < target) {
		return binarySearch(v, m+1, end, target);
	}else{
		return binarySearch(v, beg, m-1, target);
	}
}

vector<string> getStrVector(string s) {
    istringstream iss(s);
    vector<string> sVec(istream_iterator<string> {iss}, 
                          istream_iterator<string>());
    return sVec;
}
vector<char*> getCharVector(vector<string> v, int *ii) {
    size_t vSize = v.size();
    vector<char*>  cVec;
    cVec.reserve(vSize);

    for (int i = 0; i < vSize; ++i) {
        if (v[i] == "&") *ii = 1;
        cVec.push_back(const_cast<char*>(v[i].c_str()));
    }
    cVec.push_back(NULL);

    return cVec;
}
queue< vector<char*> > getExecQueue(vector<string> sv) {
    
    queue< vector<char*> > execQ;
    vector<string> topush;

    for (auto item : sv) {
        if ( (item == "&&") && (!topush.empty())) {
            vector<char*> topushc = getCharVector(topush, 0);
            execQ.push(topushc);
            // topush.clear();
        }
        topush.push_back(item);
    }
    return execQ;
}
// check if the command is cd
bool cdCheck(const char* cmd, const char* dir) {
    if (strcmp(cmd, "cd") == 0) {
        int ret = chdir(dir);
        if (ret == -1) cout << "poopy\n";
        else           cout << "changed to: " << dir << "\n";
        return 1;
    }      
    return 0;
}
// Kills rash when "exit" is entered
bool killRash(char* cmd) {
    if (strcmp(cmd, "exit") == 0) {
        cout << "goodbye...\n";
        free(cmd);
        return 1;
    }
    return 0;
}

// This does execv for each item in the $PATH env. 
int doExec(char* args[]) {
    int w = 0;    
    if(strstr(args[0], "/bin")) { 
        // full path included in input
        execv(args[0], args);
    }
    else if(char* env_path = getenv("PATH")) {
        char* each_path = strtok(env_path, ":");
        while (each_path != NULL) {
            char* execute = new char[255];
            strcpy(execute, each_path);
            strcat(execute, "/");
            strcat(execute, args[0]);
            w = execv(execute, args);
            each_path = strtok(NULL, ":");
        }
    }
    return w;
}
// Forking for exec, this calles doExec
bool doFork(char* args[], int r) {
    int status = 0;
    int w = 0;
    pid_t pid = fork();
    if (pid < 0) {          // fail
        cout << "fork pooped\n";
        return 0;
    }
    else if (pid == 0) {    // child
        doExec(&args[0]);
        exit(1);
        return 0;
    }
    else {                  // parent
        if(!r) {
            pid = wait(&status); //dont do this if you are a backgroup process
            return 1;
        }
        return 1;
    } 

}

// word exp stuff
int expand_and_exec(const char *program, char** options, int rr) {
    wordexp_t result;

    // Expand the string for the program to run
    switch (wordexp (program, &result, 0)) {
        case 0:             // Success
            break;
        case WRDE_NOSPACE:  // Perhaps part of the result was allocated
            wordfree (&result);
        default:            // some other error
            return -1;
    }

    // Expand the strings specified for the args
    for (int i = 1; options[i] != NULL; ++i) {
        if (wordexp (options[i], &result, WRDE_APPEND)) {
            wordfree (&result);
            return -1;
        }
    }
    string s(result.we_wordv[0]);
    int h = 0;
    if (s.find("/bin/") != string::npos) s.erase(0, 5);
    for (auto item : allowed_cmds) {
        if (item == s) { h = 1; break;}
    }
    if (cdCheck(result.we_wordv[0], result.we_wordv[1])) return 1;
    if(h) { if (!doFork(result.we_wordv, rr)) return 0; }
    else cout << "'" << result.we_wordv[0] << "' is not an allowed command... loser\n"; 
    return 1;
}
// catch keyboard interrupt
void my_handler(int i) {
    // printf("\nCaught signal %d\n", i);
    cout <<  "\n$ ";
}








// #include <fstream> // write to file, using for time/date in history log
// #include <chrono>
// #include <ctime>

// If I want to add the time to the history file?
//  auto time = chrono::system_clock::now();
//  auto in_time_t = chrono::system_clock::to_time_t(time);
//  ofstream historyfile;
//  historyfile.open("history.txt", ios_base::app);
//  historyfile << ctime(&in_time_t);


// // split a string by delimiter d. returns string vector of each word.
// vector<string> splitString(string d, string s) {
//     size_t pos = 0;
//     vector<string> vect;
//     string token;

//     int start = 0;
//     int end = 0;
//     int count = 1;
//     while ( (end = s.find(d, start)) < s.size() ) {
//         string val = s.substr(start, end - start);
//         vect.push_back(val);
//         start = end + d.size();
//     }
//     if (start < s.size()) {
//         string val = s.substr(start);
//         vect.push_back(val);
//     }

//     return vect;
// }