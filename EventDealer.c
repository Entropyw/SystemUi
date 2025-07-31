#include "EventDealer.h"
//add or remove a problem or user
void AddUser(User *user, Contest *c) {
    insert(user, &c -> CA);
    insert(user, &c -> CD);
    insert(user, &c -> CI);
}
void RemoveUser(User *user, Contest *c) {
    Remove(user, &c -> CA);
    Remove(user, &c -> CD);
    Remove(user, &c -> CI);
}
void AddProblem(Problem *problem, Contest *c) {
    insert(problem, &c -> PD);
    insert(problem, &c -> PI);
    insert(problem, &c -> PS);
}
void RemoveProblem(Problem *problem, Contest *c) {
    Remove(problem, &c -> PD);
    Remove(problem, &c -> PI);
    Remove(problem, &c -> PS);
}
//add submitEvent
void AddSubmitEvent(SubmitEvent *event, Contest *c) {
    insert(event, &c -> SPT);
    insert(event, &c -> SUP);
    insert(event, &c -> SUT);
    insert(event, &c -> ST);
}
void RemoveSubmitEvent(SubmitEvent *event, Contest *c) {
    Remove(event, &c -> SPT);
    Remove(event, &c -> SUP);
    Remove(event, &c -> SUT);
    Remove(event, &c -> ST);
}
//dynamicly update user ac
void UpdateUserAc(User *user, int newAc, Contest *c) {
    RemoveUser(user, c);
    user->Acs = newAc;
    AddUser(user, c);
}
//dynamicly update problem submit
void UpdateProblemSubmit(Problem *problem, int newSubmit, Contest *c) {
    RemoveProblem(problem, c);
    problem -> Submits = newSubmit;
    AddProblem(problem, c);
}