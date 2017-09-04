#include "LineGraph.h"
#include "Source/Math/Math.h"
#include "../imguiExtended.h"
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

namespace ImGuiE
{
	LineGraph::LineGraph(const ImVec2& size, const ImVec2& XRange, const ImVec2& YRange, size_t capacity, const std::string& xLabel, const std::string& yLabel) :
		size(size), xLabel(xLabel), yLabel(yLabel)
	{
		assert(XRange.x < XRange.y);
		assert(YRange.x < YRange.y);
		ranges.xMin = XRange.x;
		ranges.xMax = XRange.y;
		ranges.yMin = YRange.x;
		ranges.yMax = YRange.y;

		assert(capacity > 0);
		line.capacity = capacity;
		line.nextIndex = 0;
		autoscale = {false,false};
	}

	LineGraph::LineGraph(const ImVec2 & size, size_t capacity, const std::string & xLabel, const std::string & yLabel) :
		size(size), xLabel(xLabel), yLabel(yLabel)
	{
		autoscale = {true,true};
		assert(capacity > 0);
		line.capacity = capacity;
		line.nextIndex = 0;
		SetCoords();//To set ranges
	}

	void LineGraph::Draw()
	{
		///OPTIMIZATION does not need to be called every frame
		ShowAll(autoscale.x, autoscale.y);
		SetCoords();
		MouseControls();

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		//Draw canvas
		drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_FrameBg));
		//Adjust cursor pos by canvas and axis labels
		ImGui::Dummy(size + ImVec2(ImGui::GetFontSize() + 50, ImGui::GetFontSize() * 2));

		DrawAxes();
		DrawGrid(10);
		DrawData();
	}

	void LineGraph::AutoscaleX(bool x)
	{
		Autoscale(x, autoscale.y);
	}

	void LineGraph::AutoscaleY(bool y)
	{
		Autoscale(autoscale.x, y);
	}

	void LineGraph::Autoscale(bool x, bool y)
	{
		autoscale = {x,y};
	}


	void LineGraph::ShowAll(bool changeX, bool changeY)
	{
		if (!changeX && !changeY)
			return;
		if (line.points.size() > 0)
		{
			float xMin, xMax, yMin, yMax;
			xMin = yMin = std::numeric_limits<float>::max();
			xMax = yMax = std::numeric_limits<float>::min();
			for (const auto&p : line.points)
			{
				xMin = std::min(xMin, p.x);
				xMax = std::max(xMax, p.x);
				yMin = std::min(yMin, p.y);
				yMax = std::max(yMax, p.y);
			}

			if (xMax == xMin)//Fine because there were no arithemic ops, just assingments
			{
				xMax += 0.5f;
				xMin -= 0.5f;
			}
			if (yMax == yMin)//Fine because there were no arithemic ops, just assingments
			{
				yMax += 0.5f;
				yMin -= 0.5f;
			}
			//Little bit expand the borders
			xMin *= xMin > 0 ? 0.9f : 1.1f;
			yMin *= yMin > 0 ? 0.9f : 1.1f;
			xMax *= xMax > 0 ? 1.1f : 0.9f;
			yMax *= yMax > 0 ? 1.1f : 0.9f;
			if (changeX)
			{
				ranges.xMin = xMin;
				ranges.xMax = xMax;
			}
			if (changeY)
			{
				ranges.yMin = yMin;
				ranges.yMax = yMax;
			}
		}
		else
		{
			if (changeX)
			{
				ranges.xMin = -1.0f;
				ranges.xMax = +1.0f;
			}
			if (changeY)
			{
				ranges.yMin = -1.0f;
				ranges.yMax = +1.0f;
			}
		}
	}

	void LineGraph::SetSize(const ImVec2 & newSize)
	{
		this->size = newSize;
	}

	void LineGraph::DrawData()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImGui::PushClipRect(pos, pos + size, true);
		for (auto p : line.points)
		{
			p.x /= scale.x;
			p.y /= -1 * scale.y;
			p += origin;
			drawList->PathLineTo(p);
		}
		drawList->PathStroke(ImGui::GetColorU32(ImGuiCol_PlotHistogram), false);
		ImGui::PopClipRect();
	}

	void LineGraph::MouseControls()
	{
		if (ImGui::IsMouseHoveringRect(pos, pos + size))//Show coords at mouse position
		{
			auto io = ImGui::GetIO();
			if (ImGui::IsMouseDragging(1, 5))
			{
				ImVec2 drag = ImVec2(ImGui::GetMouseDragDelta(1))*scale;
				ranges.xMin = cachedRanges.xMin - drag.x; ranges.xMax = cachedRanges.xMax - drag.x;
				ranges.yMin = cachedRanges.yMin + drag.y; ranges.yMax = cachedRanges.yMax + drag.y;//Y is up
			}
			else if (abs(io.MouseWheel) > solar::epsilon<float>)
			{
				const static float scrollSpeed = 0.05f;
				auto xSize = ranges.xMax - ranges.xMin;
				auto xCenter = (ranges.xMin + ranges.xMax) / 2.0f;
				ranges.xMin = xCenter - xSize*(0.5f + io.MouseWheel*scrollSpeed);
				ranges.xMax = xCenter + xSize*(0.5f + io.MouseWheel*scrollSpeed);

				auto ySize = ranges.yMax - ranges.yMin;
				auto yCenter = (ranges.yMin + ranges.yMax) / 2.0f;
				ranges.yMin = yCenter - ySize*(0.5f + io.MouseWheel*scrollSpeed);
				ranges.yMax = yCenter + ySize*(0.5f + io.MouseWheel*scrollSpeed);
			}
			else
			{
				cachedRanges = ranges;
				ImGui::BeginTooltip();
				auto vals = (ImVec2(ImGui::GetMousePos()) - origin)*scale;
				ImGui::Text("(x,y)=(%+.2f,%+.2f)", vals.x, -vals.y);//Y is up
				ImGui::Text("Right click to move around.\nScroll to zoom.");
				ImGui::EndTooltip();
			}
		}

	}

	void LineGraph::AddPoint(const point & p)
	{
		assert(line.points.size() <= line.capacity);

		if (line.points.size() < line.capacity)
			line.points.push_back(p);
		else
			line.points[line.nextIndex] = p;
		//Adjusts ranges, point was placed at nextIndex
		if (autoscale.x)
		{
			ranges.xMin = std::min(ranges.xMin, line.points[line.nextIndex].x);
			ranges.xMax = std::max(ranges.xMax, line.points[line.nextIndex].x);
		}
		if (autoscale.y)
		{
			ranges.yMin = std::min(ranges.yMin, line.points[line.nextIndex].y);
			ranges.yMax = std::max(ranges.yMax, line.points[line.nextIndex].y);
		}
		++line.nextIndex;
		line.nextIndex %= line.capacity;
	}

	void LineGraph::Clear()
	{
		line.points.clear();
		line.nextIndex = 0;
	}

	void LineGraph::SetCoords()
	{
		pos = ImGui::GetCursorScreenPos();//Top-left corner

		pos.x += ImGui::GetFontSize();//Leave space for Y axis label on the left
		pos.y += ImGui::GetFontSize();//Leave space for X axis values at the top
		//Conversion ratio between pixels and data values
		scale = ImVec2(ranges.xMax - ranges.xMin, ranges.yMax - ranges.yMin) / size;
		center = ImVec2(pos.x + size.x / 2, pos.y + size.y / 2);
		//Calculate relative position of origin based on ranges, Y is up
		auto relOrigin = (ImVec2(ranges.xMin + ranges.xMax, ranges.yMin + ranges.yMax) / 2.0f) / ImVec2(ranges.xMax - ranges.xMin, ranges.yMin - ranges.yMax);
		origin = center - relOrigin*size;
	}

	void LineGraph::DrawAxes()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		if (IsInGraph(origin))//Origin
			drawList->AddText(ImVec2(origin.x, origin.y), ImGui::GetColorU32(ImGuiCol_Text), "0,0");
		if (IsInGraph(ImVec2(center.x, origin.y)))//X axis
			drawList->AddLine(ImVec2(pos.x, origin.y), ImVec2(pos.x + size.x, origin.y), ImGui::GetColorU32(ImGuiCol_Border));
		if (IsInGraph(ImVec2(origin.x, center.y)))//Y axis
			drawList->AddLine(ImVec2(origin.x, pos.y), ImVec2(origin.x, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Border));

		drawList->AddText(ImVec2(pos.x, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text), xLabel.c_str());
		ImGuiE::VerticalText(yLabel.c_str(), ImVec2(pos.x, pos.y + size.y), ImVec2(0.5f, 0.0f));
	}

	void LineGraph::DrawGrid(size_t gridRes)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		float fontSize = ImGui::GetFontSize();

		const ImVec2 offset = size / float(gridRes);
		//Number of grid lines from on the left/top side od Y/X axis
		ImVec2 count = ImVec2((origin - pos) / offset);
		count.x = trunc(count.x);
		count.y = trunc(count.y);
		//Left-most/Top-most position of Y/X grid line
		ImVec2 linePos = origin - offset*ImVec2(count);
		//Whether we are in canvas
		bool hLine = linePos.y <= pos.y + size.y && linePos.y >= pos.y, vLine = linePos.x <= pos.x + size.x && linePos.x >= pos.x;
		while (hLine || vLine)
		{
			if (vLine)
				drawList->AddLine(ImVec2(linePos.x, pos.y), ImVec2(linePos.x, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Border, 0.3f));
			if (hLine)
				drawList->AddLine(ImVec2(pos.x, linePos.y), ImVec2(pos.x + size.x, linePos.y), ImGui::GetColorU32(ImGuiCol_Border, 0.3f));
			//Draw text only for every other line  - for clarity
			if (hLine && int(count.x) % 4 == 0)
			{
				std::stringstream sVal;
				sVal << std::fixed << std::setprecision(1) << (linePos.x - origin.x)*scale.x;
				drawList->AddText(ImVec2(linePos.x - 15, pos.y - fontSize*1.5f), ImGui::GetColorU32(ImGuiCol_Text), sVal.str().c_str());
			}
			if (vLine &&  int(count.y) % 4 == 0)
			{
				const ImVec2 val(count.x / (2.0f*gridRes)*size.x*scale.x, count.x / (2.0f*gridRes)*size.y*scale.y);
				std::stringstream sVal;
				sVal << std::fixed << std::setprecision(1) << (origin.y - linePos.y)*scale.y;
				drawList->AddText(ImVec2(pos.x + size.x + fontSize / 2.0f, linePos.y - fontSize / 2.0f), ImGui::GetColorU32(ImGuiCol_Text), sVal.str().c_str());
			}

			linePos += offset;
			count.x++, count.y++;

			hLine = linePos.y <= pos.y + size.y && linePos.y >= pos.y;
			vLine = linePos.x <= pos.x + size.x && linePos.x >= pos.x;
		}
	}

	bool LineGraph::IsInGraph(ImVec2 point)
	{
		if (point.x<pos.x || point.x >pos.x + size.x)
			return false;
		else if (point.y<pos.y || point.y > pos.y + size.y)
			return false;
		else
			return true;
	}

}