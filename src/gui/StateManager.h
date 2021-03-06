/*
 * StateManager.h
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#ifndef STATEMANAGER_H_
#define STATEMANAGER_H_
enum {
	STATE_NONE,
	STATE_GUI,
	STATE_THREAD,
	STATE_TBB,
};

#include "../states/State.h"

class StateManager {
private:
	static State * activeState;

public:
	static void OnEvent(sf::Event * eventHandler);
	static void OnLoop();
	static void OnRender(sf::RenderWindow * window);

public:
	static void SetActiveState(int stateId);
	static State * GetActiveState();
};

#endif /* STATEMANAGER_H_ */
