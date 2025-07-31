#ifndef EVENT_H
#define EVENT_H
#include "Inquiry.h"
//add or remove user or problem
void AddUser(User * user, Contest *c);
void RemoveUser(User * user, Contest *c);
void AddProblem(Problem * problem, Contest *c);
void RemoveProblem(Problem *problem, Contest *c);
void AddSubmitEvent(SubmitEvent *event, Contest *c);
void RemoveSubmitEvent(SubmitEvent *event, Contest *c);
//dynamicly update users 
void UpdateUserName(User *user, char *newname, Contest *c);
void UpdateUserAc(User *user, int newAc, Contest *c);
//dynamicly update problems
void UpdateProblemSubmit(Problem *problem, int newSubmit, Contest *c);
void UpdateProblemName(Problem *problem, char *newname, Contest *c);
//deal with Events
#endif