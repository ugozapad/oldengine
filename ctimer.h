#ifndef CTIMER_H
#define CTIMER_H

#include <time.h>

class CTimer
{
public:
	CTimer();
	~CTimer();

	void Update();

	float GetDt();
	float GetTime();

public:
	clock_t beginTime;
	clock_t endTime;
	clock_t deltaTime;
};

extern CTimer timer;

#endif
