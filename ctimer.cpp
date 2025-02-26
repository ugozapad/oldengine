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
	return (float)(deltaTime) / CLOCKS_PER_SEC;
}

float CTimer::GetTime()
{
	return (float)(beginTime) / CLOCKS_PER_SEC;
}