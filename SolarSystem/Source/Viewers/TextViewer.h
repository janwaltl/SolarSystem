#ifndef VIEWERS_TEXT_VIEWER_HEADER
#define VIEWERS_TEXT_VIEWER_HEADER

#include "Viewer.h"
#include <iostream>
class TextViewer :public Viewer
{
public:
	void operator()(simData_t& data) override final
	{
		assert(data.size() == 2);
		auto& earth = data[0];
		auto& sun = data[1];
		/*
		std::cout << "1. Earth: position(" << earth.pos.X() << ", " << earth.pos.Y() << ")\n";
		std::cout << "          velocity(" << earth.vel.X() << ", " << earth.vel.Y() << ")\n";

		std::cout << "2. Sun: position(" << sun.pos.X() << ", " << sun.pos.Y() << ")\n";
		std::cout << "        velocity(" << sun.vel.X() << ", " << sun.vel.Y() << ")\n";*/
	}
private:
};

#endif