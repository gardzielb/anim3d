//
// Created by bartosz on 2/6/21.
//

#pragma once

#include <chrono>
#include <spdlog/spdlog.h>


class Timer
{
private:
	std::chrono::time_point<std::chrono::system_clock> benchmark = std::chrono::high_resolution_clock::now();
	int loopCount = 0;

public:
	void loop()
	{
		loopCount++;
	}

	~Timer()
	{
		std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - benchmark;
		float avgLoopTime = duration.count() / loopCount;
		spdlog::info( "Average loop time: {} s", avgLoopTime );
	}
};



