#ifndef VIEWERS_TEXT_VIEWER_HEADER
#define VIEWERS_TEXT_VIEWER_HEADER

#include "Viewer.h"
#include <iostream>
class TextViewer :public Viewer
{
public:
	void operator()(simData_t& data) override final
	{
		std::cout << "Viewing " << data.size() << " data\n";
	}
private:
};

#endif