#pragma once

class State {
public:
	virtual void calculate() = 0;
	virtual void draw() = 0;
	virtual void events() = 0;
};