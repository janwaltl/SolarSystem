#ifndef IMGUI_EXTENDED_LINE_GRAPH_HEADER
#define IMGUI_EXTENDED_LINE_GRAPH_HEADER

//This file is not part of imgui library
#include <External/imgui/imgui.h>


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
		//Range of plotted values - .x=Min,.y=Max
		//Max number of points
		LineGraph(const ImVec2& size, const ImVec2& XRange, const ImVec2& YRange, size_t capacity = 1'000, const std::string& xLabel = "X", const std::string& yLabel = "Y");
		//Same as above, but ranges are computed to span all points
		LineGraph(const ImVec2& size, size_t capacity = 1'000, const std::string& xLabel = "X", const std::string& yLabel = "Y");
		void Draw();
		//Enables/disables autoscaling for each axis
		void AutoscaleX(bool x);
		void AutoscaleY(bool y);
		void Autoscale(bool x, bool y);
		//Computes ranges to encompass all points
		void ShowAll(bool changeX = true, bool changeY = true);
		//Set size of graph canvas in pixels (not acounting for additional padding for axes' labels)
		void SetSize(const ImVec2& newSize);
		void AddPoint(const point& p);
		void Clear();
	private:
		struct Line
		{
			std::vector<point> points;
			size_t nextIndex;//Index in points where will the next point be placed. Equal to points.size() when the size()<capacity
			size_t capacity;
		};
		void DrawData();
		void MouseControls();
		//Updates positional variables based on actual postion of graph relative to viewport and its size
		void SetCoords();
		void DrawAxes();
		//Resolution is number of grid squares in each direction
		void DrawGrid(size_t gridRes);
		bool IsInGraph(ImVec2 point);

		Line line;
		ImVec2 size;//Size of canvas in pixels
		struct
		{
			float xMin, xMax;
			float yMin, yMax;
		}ranges, cachedRanges;
		struct
		{
			bool x, y;
		}autoscale;
		std::string xLabel, yLabel;
		ImVec2 center;//Center of canvas in pixels
		ImVec2 origin;//Location of (0,0) in pixels
		ImVec2 scale;//Conversion ratio between pixels and data values - data = scale*pixels
		ImVec2 pos;// Top left position of graph
	};
}


#endif
