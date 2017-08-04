#ifndef IMGUI_EXTENDED_HEADER
#define IMGUI_EXTENDED_HEADER
//This file is not part of imgui library

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
//All files in imguiExtended folder
#include "imguiExtended/LineGraph.h"

namespace ImGuiE
{
	//Align is in [0,1] where 1,1 means top right corner of last character and 0,0 bottom left of the first one
	//Pos is position of align point ^
	inline void VerticalText(const std::string& text, ImVec2 pos, ImVec2 align)
	{
		ImFont *font = ImGui::GetFont();
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		//Alignment. Rounded to pixels to avoid blurring
		pos.y += (int)align.x*textSize.x;
		pos.x -= (int)(1.0 - align.y)*textSize.y;
		//Reserve buffers
		drawList->PrimReserve(6 * text.length(), 4 * text.length());
		for (auto c : text)
		{
			auto glyph = font->FindGlyph(c);
			if (!glyph) continue;//No texture for it
			//Should use font texture by default
			drawList->PrimQuadUV(pos + ImVec2(glyph->Y0, -glyph->X0), pos + ImVec2(glyph->Y0, -glyph->X1),//bot left, top left
								 pos + ImVec2(glyph->Y1, -glyph->X1), pos + ImVec2(glyph->Y1, -glyph->X0),//top right, bot right
								 ImVec2(glyph->U0, glyph->V0), ImVec2(glyph->U1, glyph->V0),//top left, top right of glyph
								 ImVec2(glyph->U1, glyph->V1), ImVec2(glyph->U0, glyph->V1),//bot right, bot left of glyph
								 ImGui::GetColorU32(ImGuiCol_Text));
			pos.y -= glyph->XAdvance;
		}
	}

#pragma warning( push )
#pragma warning (disable: 4706)//Assignment in if statement
	struct StateButton
	{
		//Return whether the buttons has been clicked this frame
		bool Draw(const std::string& text)
		{
			bool clicked = false;
			ImGui::PushStyleColor(ImGuiCol_Button, selected ? ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_ButtonHovered)) :
								  ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Button)));
			if (clicked = ImGui::Button(text.c_str()))
				selected = !selected;
			hovered = ImGui::IsItemHovered();

			ImGui::PopStyleColor();
			return clicked;
		}
		//Return whether the buttons has been clicked this frame
		bool DrawBlank(const ImVec2& size)
		{
			bool clicked = false;
			ImGui::PushStyleColor(ImGuiCol_Button, selected ? ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_ButtonHovered)) :
								  ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Button)));
			ImGui::PushID(this);
			if (clicked = ImGui::Button("", size))
				selected = !selected;
			hovered = ImGui::IsItemHovered();
			ImGui::PopID();
			ImGui::PopStyleColor();
			return clicked;
		}
		//bool pressed = false;
		bool hovered = false, selected = false;
	};
}

#pragma warning( pop ) 
#endif