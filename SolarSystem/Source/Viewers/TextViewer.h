#ifndef VIEWERS_TEXT_VIEWER_HEADER
#define VIEWERS_TEXT_VIEWER_HEADER

#include "Viewer.h"
#include <iostream>
class TextViewer :public Viewer
{
public:
	void operator()(elements_t& elements) override final
	{
		std::cout << "Viewing " << elements.size() << " elements\n";
	}
private:
};

#endif