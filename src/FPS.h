/*
 * FPS.h
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#ifndef FPS_H_
#define FPS_H_

#include <SFML/System.hpp>

class FPS {
public:
	static FPS FPSControl;
	int maxFrame;
private:
	int oldTime;
	int lastTime;

	float speedFactor;

	int numFrames;
	int frames;

	sf::Clock clock;

public:
	FPS();
	void OnLoop();

public:
	int GetFPS();
	float GetSpeedFactor();
};

#endif /* FPS_H_ */
