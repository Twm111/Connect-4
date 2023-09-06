#ifndef CONNECT_H
#define CONNECT_H

void grillOutput(void);
void grillInit(void);
int input(int *,int,int);
void position(struct pattern *,int);
//need fixing 1
void grillFiller(struct pattern *,int);
int stateCheck(struct pattern *,int);
int state(struct pattern *,int);
int dirStateCheck(int,int,int,struct pattern *);
bool positionCheck(int,int);
int maximal(int,int);
bool gridFull(void);
int empty(FILE *);
double random(void);
int randNum(int,int);
int ai(void);
int validPosition(struct pattern *);

#endif