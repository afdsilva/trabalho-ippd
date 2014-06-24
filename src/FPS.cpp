/*
 * FPS.cpp
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#include "FPS.h"

FPS FPS::FPSControl;

FPS::FPS() {
	this->maxFrame = 60;
	this->oldTime = 0;
	this->lastTime = 0;

	this->speedFactor = 0;

	frames = 0;
	numFrames = 0;
}

void FPS::OnLoop() {
	sf::Time elapsed = clock.getElapsedTime();
	if (oldTime + 1000 < elapsed.asMilliseconds()) {
		oldTime = elapsed.asMilliseconds();
		numFrames = frames;
		frames = 0;
	}
	speedFactor = ((elapsed.asMilliseconds() - lastTime) / 1000.0f) * 32.0f;
	frames++;
	lastTime = elapsed.asMilliseconds();
}

int FPS::GetFPS() {
	return numFrames;
}

float FPS::GetSpeedFactor() {
	return speedFactor;
}
