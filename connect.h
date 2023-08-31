#ifndef CONNECT_H
#define CONNECT_H

void grillOutput(void);
void grillInit(void);
int input(int *);
void position(struct pattern *,int);
//need fixing 1
void grillFiller(struct pattern *,int);
int stateCheck(struct pattern *,int);
int dirStateCheck(int,int,int,struct pattern *);
bool positionCheck(int,int);
int maximal(int,int);
bool gridFull(void);

#endif