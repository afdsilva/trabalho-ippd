#include "Quadtree.h"
#include <iostream>
#include <sstream>

using namespace std;

Quadtree::Quadtree(float _x, float _y, float _width, float _height, int _level, int _maxLevel) :
	x		(_x),
	y		(_y),
	width	(_width),
	height	(_height),
	level	(_level),
	maxLevel(_maxLevel)
{
	shape.setPosition(x, y);
	shape.setSize(sf::Vector2f(width, height));
	shape.setFillColor(sf::Color(0, 0, 0, 0));
	shape.setOutlineThickness(1.0f);
	shape.setOutlineColor(sf::Color(64, 128, 255));
	text.setPosition(x, y + level * 16);
	text.setCharacterSize(12);

	if (level == maxLevel) {
		return;
	}
	parent = NULL;
	NW = new Quadtree(x, y, width / 2.0f, height / 2.0f, level+1, maxLevel);
	NE = new Quadtree(x + width / 2.0f, y, width / 2.0f, height / 2.0f, level+1, maxLevel);
	SW = new Quadtree(x, y + height / 2.0f, width / 2.0f, height / 2.0f, level+1, maxLevel);
	SE = new Quadtree(x + width / 2.0f, y + height / 2.0f, width / 2.0f, height / 2.0f, level+1, maxLevel);
}

Quadtree::~Quadtree()
{
	if (level == maxLevel)
		return;

	delete NW;
	delete NE;
	delete SW;
	delete SE;
}

void Quadtree::AddObject(Triangle *object) {
	if (level == maxLevel) {
		objects.push_back(object);
		return;
	}
	if (contains(NW, object)) {
		NW->AddObject(object); return;
	} else if (contains(NE, object)) {
		NE->AddObject(object); return;
	} else if (contains(SW, object)) {
		SW->AddObject(object); return;
	} else if (contains(SE, object)) {
		SE->AddObject(object); return;
	}
	if (contains(this, object)) {
		objects.push_back(object);
	}
}

vector<Triangle*> Quadtree::GetObjects() {
	if (level == maxLevel) {
		return objects;
	}
	vector<Triangle *> returnObjects, childReturnObjects;
	childReturnObjects = SE->GetObjects();
	returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
	childReturnObjects = NE->GetObjects();
	returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
	childReturnObjects = SW->GetObjects();
	returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
	childReturnObjects = NW->GetObjects();
	returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());

	return returnObjects;
}
vector<Triangle*> Quadtree::GetObjectsAt(float _x, float _y) {
	if (level == maxLevel) {
		return objects;
	}
	
	vector<Triangle *> returnObjects, childReturnObjects;
	if (!objects.empty()) {
		returnObjects = objects;
	}
	bool neb, seb, swb, nwb;
	neb = seb = swb = nwb = false;
	if (_x > x + width / 2.0f && _x < x + width) {
		if (_y > y + height / 2.0f && _y < y + height) {
			childReturnObjects = SE->GetObjectsAt(_x, _y);
			seb = (childReturnObjects.empty()? true : false);
			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
			if (!returnObjects.empty())
				return returnObjects;
		} else if (_y > y && _y <= y + height / 2.0f) {
			childReturnObjects = NE->GetObjectsAt(_x, _y);
			neb = (childReturnObjects.empty()? true : false);
			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
			if (!returnObjects.empty())
				return returnObjects;
		}
	} else if (_x > x && _x <= x + width / 2.0f) {
		if (_y > y + height / 2.0f && _y < y + height) {
			childReturnObjects = SW->GetObjectsAt(_x, _y);
			swb = (childReturnObjects.empty()? true : false);
			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
			if (!returnObjects.empty())
				return returnObjects;
		} else if (_y > y && _y <= y + height / 2.0f) {
			childReturnObjects = NW->GetObjectsAt(_x, _y);
			nwb = (childReturnObjects.empty()? true : false);
			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
			if (!returnObjects.empty())
				return returnObjects;
		}
	}
	if (returnObjects.empty()) {
		if (!seb) {
			childReturnObjects = SE->GetObjectsAt(_x, _y);
			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
		}
		if (!neb) {
			childReturnObjects = NE->GetObjectsAt(_x, _y);
			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
		}
		if (!swb) {
			childReturnObjects = SW->GetObjectsAt(_x, _y);
			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
		}
		if (!nwb) {
			childReturnObjects = NW->GetObjectsAt(_x, _y);
			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
		}
	}

	return returnObjects;
}

void Quadtree::Clear() {
	if (level == maxLevel) {
		objects.clear();
		return;
	} else {
		NW->Clear();
		NE->Clear();
		SW->Clear();
		SE->Clear();
	}
	if (!objects.empty()) {
		objects.clear();
	}
}

int Quadtree::GetQuadrantSize(float _x, float _y) {
	int retorno = 0;
	if (level == maxLevel) {
		return objects.size();
	} else {
		retorno += NW->GetTreeSize();
		retorno += NE->GetTreeSize();
		retorno += SW->GetTreeSize();
		retorno += SE->GetTreeSize();

		if (_x > x + width / 2.0f && _x < x + width) {
			if (_y > y + height / 2.0f && _y < y + height) {
				//SE
				retorno += SE->GetQuadrantSize(_x,_y);
			} else if (_y > y && _y <= y + height / 2.0f) {
				//NE
				retorno += NE->GetQuadrantSize(_x,_y);
			}
		} else if (_x > x && _x <= x + width / 2.0f) {
			if (_y > y + height / 2.0f && _y < y + height) {
				//SW
				retorno += SW->GetQuadrantSize(_x,_y);
			} else if (_y > y && _y <= y + height / 2.0f) {
				//NW
				retorno += NW->GetQuadrantSize(_x,_y);
			}
		}
	}
	return retorno;
}
int Quadtree::GetTreeSize() {
	int retorno = 0;
	if (level == maxLevel) {
		return objects.size();
	} else {
		retorno += NW->GetTreeSize();
		retorno += NE->GetTreeSize();
		retorno += SW->GetTreeSize();
		retorno += SE->GetTreeSize();
	}
	return retorno;
}
bool Quadtree::Inserted(Triangle * object) {
	bool retorno = false;
	try {
		if (maxLevel == level) {
			std::for_each(objects.begin(), objects.end(), [&] (Triangle * t) {
				if (t == object)
					retorno = true;
			});
			return retorno;
		}
		if (object->getCentroid().x > x + width / 2.0f && object->getCentroid().x < x + width) {
			if (object->getCentroid().y > y + height / 2.0f && object->getCentroid().y < y + height) {
				return SE->Inserted(object);
			} else if (object->getCentroid().y > y && object->getCentroid().y <= y + height / 2.0f) {
				return NE->Inserted(object);
			}
		} else if (object->getCentroid().x > x && object->getCentroid().x <= x + width / 2.0f) {
			if (object->getCentroid().y > y + height / 2.0f && object->getCentroid().y < y + height) {
				return SW->Inserted(object);
			} else if (object->getCentroid().y > y && object->getCentroid().y <= y + height / 2.0f) {
				return NW->Inserted(object);
			}
		}

	} catch (int e) {
	}
	return retorno;
}
void Quadtree::SetFont(const sf::Font &font) {
	text.setFont(font);
	if (level != maxLevel) {
		NW->SetFont(font);
		NE->SetFont(font);
		SW->SetFont(font);
		SE->SetFont(font);
	}
}

void Quadtree::Draw(sf::RenderTarget &canvas) {
	stringstream ss;
	ss << objects.size();
	string numObjectsStr = ss.str();
	text.setString(numObjectsStr);
	canvas.draw(shape);
	canvas.draw(text);
	if (level != maxLevel) {
		NW->Draw(canvas);
		NE->Draw(canvas);
		SW->Draw(canvas);
		SE->Draw(canvas);
	}
}

bool Quadtree::contains(Quadtree *child, Triangle *object) {
	return	 !(object->getCentroid().x < child->x ||
				object->getCentroid().y < child->y ||
				object->getCentroid().x > child->x + child->width  ||
				object->getCentroid().y > child->y + child->height);
}
