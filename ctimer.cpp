#include "ctimer.h"

CTimer timer;

CTimer::CTimer()
{
	beginTime = clock();
	endTime = clock();
}

CTimer::~CTimer()
{
}

void CTimer::Update()
{
	beginTime=clock();
	deltaTime = beginTime - endTime;
	endTime = beginTime;
}

float CTimer::GetDt()
{
	float dt = (float)(deltaTime) / CLOCKS_PER_SEC;
	if (dt >= 0.2f)
		dt = 0.2f;
	return dt;
}

float CTimer::GetTime()
{
	return (float)(beginTime) / CLOCKS_PER_SEC;
}