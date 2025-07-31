#include "Inquiry.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//CD tree to find the name
User* FindUserName(char* UserName, Contest *c) {
    User x;
    strcpy(x.userName, UserName);
    void *k = Find(&x, &c->CD);
    if(!k) return NULL;
    return (User *)k;
}
//CI tree to find the Id
User * FindUserId(const ll UserId, Contest *c) {
    User x;
    x.UserId = UserId;
    void *k = Find(&x, &c->CI);
    if(!k) return NULL;
    return (User *)k;
}
//PD tree to find the name
Problem * FindProblemName(char* ProblemName, Contest *c) {
    Problem x;
    strcpy(x.problemName, ProblemName);
    void *k = Find(&x, &c->PD);
    if(!k) return NULL;
    return (Problem *)k;
}
//PI tree to find the Id
Problem * FindProblemId(const int ProblemId, Contest *c) {
    Problem x;
    x.problemId = ProblemId;
    void *k = Find(&x, &c->PI);
    if(!k) return NULL;
    return (Problem *)k;
}
//all the submits in the problem
Array FindProblemSubmit(const int ProblemId, Contest *c) {
    SubmitEvent upper;
    SubmitEvent lower;
    upper.problem = ProblemId;
    upper.submitTime = 0;
    lower.problem = ProblemId;
    lower.submitTime = 1e11;
    Array list;
    list.size = findRank(&upper, &c -> SPT) - findRank(&lower, &c -> SPT);
    list.a = (void **)malloc(sizeof(SubmitEvent *)*list.size);
    SubmitEvent *now = (SubmitEvent *)NxtMem(&lower, &c -> SPT);
    int cnt = 0;
    while(now != NULL && c -> SPT.cmp(now, &upper) < 0) {
        list.a[cnt++] = now;
        now = (SubmitEvent *)NxtMem(now, &c -> SPT);
    } 
    list.size = cnt;
    return list;
}
//find all the users' submit
Array FindUserSubmit(const int userId, Contest *c) {
    SubmitEvent upper;
    SubmitEvent lower;
    upper.user = userId; 
    upper.submitTime = 0;
    lower.user = userId;
    lower.submitTime = 1e11;
    Array list;
    list.size = findRank(&upper, &c -> SUT) - findRank(&lower, &c -> SUT);
    list.a = (void **)malloc(sizeof(SubmitEvent *)*list.size);
    SubmitEvent *now = (SubmitEvent *)NxtMem(&lower, &c -> SUT);
    int cnt = 0;
    while(now != NULL && c -> SUT.cmp(now, &upper) < 0) {
        list.a[cnt++] = now;
        now = (SubmitEvent *)NxtMem(now, &c -> SUT);
    } 
    list.size = cnt;
    return list;
}
//find a user's submit of a problem
Array FindProblemUserSubmit(const int UserId, const int ProblemId, Contest *c) {
    SubmitEvent upper;
    SubmitEvent lower;
    upper.problem = ProblemId;
    upper.user = UserId;
    upper.submitTime = 0;
    lower.problem = ProblemId;
    lower.user = UserId;
    lower.submitTime = 1e11;
    Array list;
    list.size = findRank(&upper, &c -> SUP) - findRank(&lower, &c -> SUP);
    list.a = (void **)malloc(sizeof(SubmitEvent *)*list.size);
    SubmitEvent *now;
    now = (SubmitEvent *)NxtMem(&lower, &c -> SUP);
    int cnt = 0;
    while(now != NULL && c->SUP.cmp(now, &upper) < 0) {
        list.a[cnt++] = now;
        now = (SubmitEvent *)NxtMem(now, &c -> SUP);
    }
    list.size = cnt;
    return list;
}
//find all the submit events after a time
Array FindSubmitEventsAfter(int time, Contest *c) {
    SubmitEvent upper;
    SubmitEvent lower;
    upper.submitTime = time - 1;
    lower.submitTime = 1e11;
    Array list;
    list.size = findRank(&upper, &c -> ST) - findRank(&lower, &c -> ST) + 1;
    list.a = (void **)malloc(sizeof(SubmitEvent *)*list.size);
    SubmitEvent *now;
    now = (SubmitEvent *)NxtMem(&lower, &c -> ST);
    int cnt = 0;
    while(now != NULL && c->ST.cmp(now, &upper) < 0) {
        list.a[cnt++] = now;
        now = (SubmitEvent *)NxtMem(now, &c -> ST);
    }
    list.size = cnt;
    return list;
}
//find the problem's Ac submit in submit time order
Array FindProblemAcSubmitOrder(const int ProblemId, Contest *c) {
    SubmitEvent upper;
    SubmitEvent lower;
    upper.problem = ProblemId;
    upper.status = AC;
    upper.submitTime = 0;
    lower.problem = ProblemId;
    upper.status = AC;
    upper.submitTime = 1e11;
    Array list;
    list.size = findRank(&upper, &c->SPAT) - findRank(&lower, &c->SPAT);
    list.a = (void **)malloc(sizeof(SubmitEvent *)*list.size);
    SubmitEvent *now;
    now = (SubmitEvent *)NxtMem(&lower, &c->SPAT);
    int cnt = 0;
    while(now != NULL && c->SPAT.cmp(now, &upper) < 0) {
        list.a[cnt++] = now;
        now = (SubmitEvent *)NxtMem(now, &c->SPAT);
    }
    list.size = cnt;
    return list;
}
//find a problem's AC submits in submit time order
Array FindProblemAcSubmitTime(const int ProblemId, Contest *c) {
    SubmitEvent upper;
    SubmitEvent lower;
    upper.problem = ProblemId;
    upper.status = AC;
    upper.timeUsage = -1;
    lower.problem = ProblemId;
    upper.status = AC;
    upper.timeUsage = NA;
    Array list;
    list.size = findRank(&upper, &c->SPTU) - findRank(&lower, &c->SPTU);
    list.a = (void **)malloc(sizeof(SubmitEvent *)*list.size);
    SubmitEvent *now;
    now = (SubmitEvent *)NxtMem(&lower, &c->SPTU);
    int cnt = 0;
    while(now != NULL && c->SPAT.cmp(now, &upper) < 0) {
        list.a[cnt++] = now;
        now = (SubmitEvent *)NxtMem(now, &c->SPTU);
    }
    list.size = cnt;
    return list;
}
//find all the prblems over the submit of
Array FindProblemOverSubmit(int submits, Contest *c) {
    Problem upper;
    Problem lower;
    upper.Submits = submits - 1;
    lower.Submits = NA;
    Array list;
    list.size = findRank(&upper, &c->PS) - findRank(&lower, &c->PS);
    list.a = (void **)malloc(sizeof(Problem *)*list.size);
    Problem *now;
    now = (Problem *)NxtMem(&lower, &c->PS);
    int cnt = 0;
    while(now != NULL && c->PS.cmp(now, &upper) < 0) {
        list.a[cnt++] = now;
        now = (Problem *)NxtMem(now, &c->PS);
    }
    list.size = cnt;
    return list;
}
//find all the users over the ac of
Array FindUserOverAc(int Acs, Contest *c) {
    User upper;
    User lower;
    upper.Acs = Acs - 1;
    lower.Acs = NA;
    Array list;
    list.size = findRank(&upper, &c->CA) - findRank(&lower, &c->CA) + 1;
    list.a = (void **)malloc(sizeof(User *)*list.size);
    User *now;
    now = (User *)NxtMem(&lower, &c->CA);
    int cnt = 0;
    while(now != NULL && c->CA.cmp(now, &upper) < 0) {
        list.a[cnt++] = now;
        now = (User *)NxtMem(now, &c->CA);
    }
    list.size = cnt;
    return list;
}
