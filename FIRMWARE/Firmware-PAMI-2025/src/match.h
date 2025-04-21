// match.h
#ifndef MATCH_H
#define MATCH_H

#include <Arduino.h>
#include "ihm.h"

#define TIME_START_MATCH 85000  //85000
#define TIME_END_MATCH 100000   //100000

#define MATCH_WAIT 0
#define MATCH_BEGIN 1
#define PAMI_RUN 2
#define PAMI_STOP 3

int getMatchState();
void setMatchState(int _state);
void startMatch();
void updateMatchTime();
void pause(long millisecondes);

#endif // MATCH_H
