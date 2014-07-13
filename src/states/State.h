/*
 * State.h
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#ifndef STATE_H_
#define STATE_H_

#include "State.h"
#include "../gui/Eventos.h"
#include <SFML/Graphics.hpp>

class State : public Eventos {
public:
	State();
public:
	virtual void OnActivation() = 0;
	virtual void OnDeactivation() = 0;
	virtual void OnLoop() = 0;
	virtual void OnRender(sf::RenderWindow * window) = 0;
};

#endif /* STATE_H_ */
