#ifndef IMGUI_EXTENDED_LINE_GRAPH_HEADER
#define IMGUI_EXTENDED_LINE_GRAPH_HEADER

//This file is not part of imgui library
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"


#include<vector>
namespace ImGuiE
{
	//Constructs graph with passed data which is plotted as lines
	//Draws it at ImGui's cursor's position
	class LineGraph
	{
	public:
		using point = ImVec2;
		//Size of canvas in pixels
		//Range of plotted values
		LineGraph(const ImVec2& size, const ImVec2& XRange, const ImVec2& YRange, size_t capacity = 1'000, const std::string& xLabel = "X", const std::string& yLabel = "Y");
		void operator()();

		void AddPoint(const point& p);
	private:
		struct Line
		{
			std::vector<point> points;
			size_t lastIndex;//When buffer is full, next point will be pushed here. undefined when points.size()!=capacity
			size_t capacity;
		};
		void DrawData();
		void MouseControls();
		//Updates positional variables based on actual postion of graph relative to viewport
		void SetCoords();
		void DrawAxes();
		//Resolution is number of grid squares in each direction
		void DrawGrid(size_t gridRes);
		bool IsInGraph(ImVec2 point);

		Line line;
		ImVec2 size;//Size of canvas in pixels
		ImVec2 XRange, YRange;//Ranges of plotted values
		std::string xLabel,yLabel;
		ImVec2 center;//Center of canvas in pixels
		ImVec2 origin;//Location of (0,0) in pixels
		ImVec2 scale;//Conversion ratio between pixels and data values - data = scale*pixels
		ImVec2 pos;// Top left position of graph
	};
}


#endif