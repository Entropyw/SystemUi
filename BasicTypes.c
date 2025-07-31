#include "BasicTypes.h"
#include <string.h>
//compare in dictionary order
int DicCmpUser(const void* const a, const void* const b) {
    User *x = (User *)a;
    User *y = (User *)b;
    char *namea = x -> userName;
    char *nameb = y -> userName;
    int lena = strlen(namea);
    int lenb = strlen(nameb);
    int i = 0;
    while(namea[i] == nameb[i] && i < lena && i < lenb) i++;
    if(i == lena || i == lenb) {
        if(lena < lenb) return -1;
        else if(lena > lenb) return 1;
        return 0;
    }
    if(namea[i] < nameb[i]) return -1;
    else return 1;
}
int IdCmpUser(const void* const a, const void* const b) {
    //compare in Id dictionary Order
    User *x = (User *)a;
    User *y = (User *)b;
    int Ida = x->UserId;
    int Idb = y->UserId;
    if(Ida < Idb) return -1;
    else if(Ida == Idb) return 0;
    return 1;
    //ID is unique for each user
}
//Ac number comparison
int AcCmpUser(const void* const a, const void* const b) {
    User *x = (User *)a;
    User *y = (User *)b;
    if(x -> Acs == y -> Acs) {
        if(x -> totalTime == y -> totalTime) {
            if(x -> UserId < y -> UserId) return -1;
            else if(x -> UserId == y -> UserId) return 0;
            else return 1;
        }
        if(x -> totalTime < y -> totalTime) return -1;
        else if(x -> totalTime == y -> totalTime) return 0;
        return 1;
    }else if(x -> Acs > y -> Acs) return -1;
    else return 1;
}
//Submits comparison for problems
int SubmitsCmpProblem(const void* const a, const void* const b) {
    Problem *x = (Problem *)a;
    Problem *y = (Problem *)b;
    if(x -> Submits == y -> Submits) {
        if(x -> problemId > y -> problemId) return 1;
        else if(x -> problemId == y -> problemId) return 0;
        else return -1;
    }
    if(x -> Submits > y -> Submits) return -1;
    else return 1;
}
//Dictionary comparison for problems
int DicCmpProblem(const void* const a, const void* const b) {
    Problem *x = (Problem *)a;
    Problem *y = (Problem *)b;
    char *namea = x -> problemName;
    char *nameb = y -> problemName;
    int lena = strlen(namea);
    int lenb = strlen(nameb);
    int i = 0;
    while(namea[i] == nameb[i] && i < lena && i < lenb) i++;
    if(i == lena || i == lenb) {
        if(lena < lenb) return -1;
        else if(lena > lenb) return 1;
        return 0;
    }
    if(namea[i] < nameb[i]) return -1;
    else return 1;
}
//Id comparisons for problems
int IdCmpProblem(const void* const a, const void* const b) {
    Problem *x = (Problem *)a;
    Problem *y = (Problem *)b;
    if(x -> problemId == y -> problemId) return 0;
    else if(x -> problemId > y -> problemId) return 1;
    else return -1;
}
//Submit event comparison strategies
int UserProblemCmp(const void* const a, const void* const b) {
    SubmitEvent *x = (SubmitEvent *)a;
    SubmitEvent *y = (SubmitEvent *)b;
    if(x -> user == y -> user) {
        if(x -> problem == y -> problem) {
            if(x -> submitTime > y -> submitTime) return -1;
            else if(x -> submitTime == y -> submitTime) return 0;
            else if(x -> submitTime < y -> submitTime) return 1;
        }else{
            if(x -> problem > y -> problem) return 1;
            else if(x -> problem < y -> problem) return -1;
        }
    }else if(x -> user < y -> user) return -1;
    return 1;
}
//Comparison stategies for submitevents to split certain submits
int TimeFirstCmp(const void* const a, const void* const b) {
    SubmitEvent *x = (SubmitEvent *)a;
    SubmitEvent *y = (SubmitEvent *)b;
    if(x -> submitTime < y -> submitTime) return 1;
    else if(x -> submitTime == y -> submitTime) return 0;
    return -1;
}
int UserTimeCmp(const void* const a, const void* const b) {
    SubmitEvent *x = (SubmitEvent *)a;
    SubmitEvent *y = (SubmitEvent *)b;
    if(x -> user == y -> user) {
        if(x -> submitTime < y -> submitTime) return 1;
        else if(x -> submitTime == y -> submitTime) return 0;
        else return -1;   
    }
    if(x -> user < y -> user) return -1;
    return 1;
}
int ProblemTimeCmp(const void* const a, const void* const b) {
    SubmitEvent *x = (SubmitEvent *)a;
    SubmitEvent *y = (SubmitEvent *)b;
    if(x -> problem == y -> problem) {
        if(x -> submitTime < y -> submitTime) return 1;
        else if(x -> submitTime == y -> submitTime) return 0;
        else return -1;
    }
    if(x -> problem > y -> problem) return 1;
    return -1; 
}
int ProblemTimeUsageCmp(const void* const a, const void* const b) {
    SubmitEvent *x = (SubmitEvent *)a;
    SubmitEvent *y = (SubmitEvent *)b;
    if(x -> problem == y -> problem) {
        if(x -> status == y -> status) {
            if(x -> timeUsage == y -> timeUsage) {
                if(x -> submitTime < y -> submitTime) return 1;
                else if(x -> submitTime == y -> submitTime) return 0;
                return -1;
            }
            if(x -> timeUsage < y -> timeUsage) return -1;
            return 1;
        }
        if(x -> status < y -> status) return -1;
        return 1;
    }
    if(x -> problem < y -> problem) return -1;
    return 1;
}
int ProblemAcTimeCmp(const void* const a, const void* const b) {
    SubmitEvent *x = (SubmitEvent *)a;
    SubmitEvent *y = (SubmitEvent *)b;
    if(x -> problem == y -> problem) {
        if(x -> status == y -> status) {
            if(x -> submitTime == y -> submitTime) return 0;
            if(x -> submitTime < y -> submitTime) return 1;
            return -1;
        }
        if(x -> status < y -> status) return -1;
        return 1;
    }
    if(x -> problem < y -> problem) return -1;
    return 1;
}
//start a contest
void startContest(Contest *c) {
    build(&c->CD, DicCmpUser);
    build(&c->CI, IdCmpUser);
    build(&c->CA, AcCmpUser);
    build(&c->PD, DicCmpProblem);
    build(&c->PI, IdCmpProblem);
    build(&c->PS, SubmitsCmpProblem);
    build(&c->SPT, ProblemTimeCmp);
    build(&c->ST, TimeFirstCmp);
    build(&c->SUP, UserProblemCmp);
    build(&c->SUT, UserTimeCmp);
}