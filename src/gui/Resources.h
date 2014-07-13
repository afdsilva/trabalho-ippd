/*
 * Resources.h
 *
 *  Created on: 04/07/2014
 *      Author: andref
 */

#ifndef RESOURCES_H_
#define RESOURCES_H_

#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

enum FONTS {
	ScienceProject = 0,
	SecretLabs,
	Serious1,
	Serious2,
	Serious2b,
	Serious3,
	Serious3b,
	Serious4,
	Serious4b,
	ShadowsAroundUs
};
class ResourceFonts {
public:
	static std::vector<sf::Font *> fontsContainer;
	static sf::Font & getFontResource(FONTS font) {
		return * ResourceFonts::fontsContainer[font];
	}
	static void loadFont(sf::String path, FONTS label) {
		sf::Font * newFont = new sf::Font();
		if (newFont->loadFromFile(path)) {
			ResourceFonts::fontsContainer.resize(ResourceFonts::fontsContainer.size() + 1);
			ResourceFonts::fontsContainer[label] = newFont;
		}
	}
	static void loadFonts() {
		fontsContainer.clear();
		ResourceFonts::loadFont("res/fonts/ScienceProject.ttf", FONTS::ScienceProject);
		ResourceFonts::loadFont("res/fonts/SecretLabs.ttf", FONTS::SecretLabs);
		ResourceFonts::loadFont("res/fonts/Serious1.ttf", FONTS::Serious1);
		ResourceFonts::loadFont("res/fonts/Serious2.ttf", FONTS::Serious2);
		ResourceFonts::loadFont("res/fonts/Serious2b.ttf", FONTS::Serious2b);
		ResourceFonts::loadFont("res/fonts/Serious3.ttf", FONTS::Serious3);
		ResourceFonts::loadFont("res/fonts/Serious3b.ttf", FONTS::Serious3b);
		ResourceFonts::loadFont("res/fonts/Serious4.ttf", FONTS::Serious4);
		ResourceFonts::loadFont("res/fonts/Serious4b.ttf", FONTS::Serious4b);
		ResourceFonts::loadFont("res/fonts/ShadowsAroundUs.ttf", FONTS::ShadowsAroundUs);
	}
};

#endif /* RESOURCES_H_ */
