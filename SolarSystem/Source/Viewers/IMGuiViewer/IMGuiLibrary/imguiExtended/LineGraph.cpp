#include "LineGraph.h"
#include "Source/Math/Math.h"
#include "../imguiExtended.h"
#include <vector>
#include <sstream>
#include <iomanip>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui_internal.h"

namespace ImGuiE
{
	LineGraph::LineGraph(const ImVec2& size, const ImVec2& XRange, const ImVec2& YRange, size_t capacity, const std::string& xLabel, const std::string& yLabel) :
		size(size), XRange(XRange), YRange(YRange), xLabel(xLabel), yLabel(yLabel)
	{
		assert(capacity > 0);
		line.capacity = capacity;
		line.lastIndex = 0;
	}

	void LineGraph::Draw()
	{
		SetCoords();

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		//Draw canvas
		drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_FrameBg));
		//Adjust cursor pos by canvas and axis labels
		ImGui::Dummy(size + ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize()));

		MouseControls();
		DrawAxes();
		DrawGrid(10);
		DrawData();
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
			static ImVec2 cXRange, cYRange;
			auto io = ImGui::GetIO();

			if (ImGui::IsMouseDragging(1, 5))
			{
				ImVec2 drag = ImVec2(ImGui::GetMouseDragDelta(1))*scale;
				XRange.x = cXRange.x - drag.x; XRange.y = cXRange.y - drag.x;
				YRange.x = cYRange.x + drag.y; YRange.y = cYRange.y + drag.y;//Y is up
			}
			else if (abs(io.MouseWheel) > solar::epsilon<float>)
			{
				cXRange += XRange*io.MouseWheel*0.05f;
				cYRange += YRange*io.MouseWheel*0.05f;
				const float minRange = 0.1f;
				//TO not skew aspect ratio between axes when hitting the bound
				const auto ratio = (cXRange.y - cXRange.x) / (cYRange.y - cYRange.x);
				if (cXRange.y - cXRange.x > minRange*ratio)
					XRange = cXRange;
				if (cYRange.y - cYRange.x > minRange)
					YRange = cYRange;
			}
			else
			{
				cXRange = XRange;
				cYRange = YRange;
				ImGui::BeginTooltip();
				auto vals = (ImVec2(ImGui::GetMousePos()) - origin)*scale;
				ImGui::Text("(x,y)=(%+.2f,%+.2f)\nRight click to move around.\nScroll to zoom.", vals.x, -vals.y);//Y is up
				ImGui::EndTooltip();
			}
		}

	}

	void LineGraph::AddPoint(const point & p)
	{
		if (line.points.size() < line.capacity)
			line.points.push_back(p);
		else
		{
			assert(line.points.size() == line.capacity);
			line.points[line.lastIndex++] = p;
			line.lastIndex %= line.capacity;
		}
	}

	void LineGraph::SetCoords()
	{
		pos = ImGui::GetCursorScreenPos();//Top-left corner

		pos.x += ImGui::GetFontSize();//Leave space for Y axis label on left
		pos.y += ImGui::GetFontSize();//Leave space for X axis values at top
									  //Conversion ratio between pixels and data values
		scale = ImVec2(XRange.y - XRange.x, YRange.y - YRange.x) / size;
		center = ImVec2(pos.x + size.x / 2, pos.y + size.y / 2);
		//Calculate relative position of origin based on ranges
		auto relOrigin = (ImVec2(XRange.x + XRange.y, YRange.y + YRange.x) / 2.0f) / ImVec2(XRange.y - XRange.x, YRange.x - YRange.y);
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

		drawList->AddText(ImVec2(center.x, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text), xLabel.c_str());
		ImGuiE::VerticalText(yLabel.c_str(), ImVec2(pos.x, center.y), ImVec2(0.5f, 0.0f));
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