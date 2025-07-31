#ifndef SYSTEM_H
#define SYSTEM_H
#include "Splay.h"
#define NA 0x7fffffff
typedef long long ll;
enum Status {
    AC = 0, WA, RE, TLE, CE, Others
};
//define the user type
typedef struct {
    char userName[20]; //Users' nickname of the account
    int UserId; //Randomly generated, unique for every user
    int Submits, Acs; 
    int rankAc; //according to Acs
    int ContestScore;
    int totalTime;
}User;
//define the problem type
typedef struct {
    char problemName[20];
    int problemId;
    int Submits; //The number of submits for all users
    int AC, WA, RE, TLE, CE, Others; //Numbers of different status of submits
    int rank;
    int tag;
    int difficulty;
}Problem;
//define the type of Submit Events
typedef struct {
    int user, problem;
    enum Status status;
    int timeUsage, memoryUsage;
    ll submitTime;
    int year, month, day, hour, minute, second;
}SubmitEvent;
//define the type of contest
typedef struct {
    //data storage
    SplayTree PS;  //total submits order of problem
    SplayTree PI;  //Id order of problem
    SplayTree PD;  //Dictionarial order of problem(for finding)
    SplayTree CD; //Dictionarial order of candidates
    SplayTree CI; //Id order of candidates
    SplayTree CA;  //AC order of candidates
    SplayTree SUP; //User-problem order of submitevents
    SplayTree ST; //Time order of submitevents
    SplayTree SUT; //User-Time order of submitevents
    SplayTree SPTU; //problem-timeusage order of submitevents
    SplayTree SPT;  //problem-Time order of submitevents
    SplayTree SPAT; //problem-Ac-SubmitTime order of submitEvents
}Contest;
//Diffent comparison strategies for users
int DicCmpUser(const void * const a, const void* const b); //dictionarial order of user name
int IdCmpUser(const void* const a, const void* const b);
int AcCmpUser(const void* const a, const void* const b);
//Different comparison strategies for problems
int SubmitsCmpProblem(const void * const a, const void* const b);
int DicCmpProblem(const void* const a, const void* const b); //dictionary order of problem name
int IdCmpProblem(const void* const a, const void* const b);
//Different comparison strategies for submit events
int UserProblemCmp(const void* const a, const void* const b);
int TimeFirstCmp(const void* const a, const void* const b);
int UserTimeCmp(const void* const a, const void* const b);
int ProblemTimeUsageCmp(const void* const a, const void* const b);
int ProblemTimeCmp(const void* const a, const void* const b);
int ProblemAcTimeCmp(const void* const a, const void* const b);
//functions for creating a contest
void startContest(Contest *c);

#endif