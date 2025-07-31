#ifndef INQUIRY_H
#define INQUIRY_H
#include "BasicTypes.h"
//define a type of returned value
typedef struct {
    void **a;
    int size;
}Array;

User * FindUserName(char *UserName, Contest *c);
User * FindUserId(const ll UserId, Contest *c);
Problem * FindProblemName(char *ProblemName, Contest *c);
Problem * FindProblemId(const int ProblemId, Contest *c);
Array FindProblemSubmit(const int ProblemId, Contest *c); //Submits of a problem
Array FindUserSubmit(const int UserId, Contest *c);  //Submits of a user
//submits of user in a problem
Array FindProblemUserSubmit(const int UserId, const int problemId, Contest *c); 
Array FindProblemAcSubmitOrder(const int ProblemId, Contest *c); //Acs of a problem in submit order
Array FindProblemAcSubmitTime(const int ProblemId, Contest *c); //Ac of a problem in submit time usage order
Array FindProblemOverSubmit(int submit, Contest *c); //find problems over the submit of
Array FindUserOverAc(int Acs, Contest *c); //find users over the ac of
Array FindSubmitEventsAfter(int time, Contest *c); //find all the submit events after a time

#endif