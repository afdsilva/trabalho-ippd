/*
 * StateManager.cpp
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#include "StateManager.h"
/**
 * States Includes
 */
#include "../states/StateGUI.h"
#include "../states/StateThread.h"
#include "../states/StateTBB.h"

State * StateManager::activeState = 0;

void StateManager::OnEvent(sf::Event * eventHandler) {
	if (activeState) activeState->AppEvent(eventHandler);

}
void StateManager::OnLoop() {
	if (activeState) activeState->OnLoop();
}
void StateManager::OnRender(sf::RenderWindow * window) {
	if (activeState) activeState->OnRender(window);
}

void StateManager::SetActiveState(int stateId) {
	if (activeState) activeState->OnDeactivation();

	/**
	 * States
	 */
	switch(stateId) {
	case STATE_NONE:
		activeState = 0;
		break;
	case STATE_GUI:
		activeState = StateGUI::GetIntance();
		break;
	case STATE_THREAD:
		activeState = StateThread::GetIntance();
		break;
	case STATE_TBB:
		activeState = StateTBB::GetIntance();
		break;
	}
	if (activeState) activeState->OnActivation();
}
State * StateManager::GetActiveState() {
	return activeState;
}

