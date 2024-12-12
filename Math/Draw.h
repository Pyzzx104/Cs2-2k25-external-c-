#pragma once
#include "../imgui/imgui.h"
#include <string>




namespace desing 
{

    ImDrawList* GetDrawList() {
        return ImGui::GetForegroundDrawList();
    }

    void DrawFilledRect(int x, int y, int w, int h, ImVec4 fillColor, float thickness)
    {

        ImVec4 transparentColor = ImVec4(fillColor.x, fillColor.y, fillColor.z, fillColor.w * 0.1f);
        GetDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImColor(transparentColor));
        GetDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImColor(fillColor), 0.0f, 0, thickness);
    }

    void Line(float x1, float y1, float x2, float y2, ImVec4 color, float thickness) {
        GetDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImColor(color), thickness);
    }

    void Circle(float x, float y, float radius, ImVec4 color) {
        GetDrawList()->AddCircle(ImVec2(x, y), radius, ImColor(color), 0);
    }

    void DrawHealthBar(float x, float y, int height, ImVec4 color, int sso, float thickness) {

        GetDrawList()->AddLine(ImVec2(x + 10, y), ImVec2(x + 10, y - (height * sso / 100.0f)), ImColor(color), thickness);
    }

    void NameP(float x, float y, ImVec4 color, const char* text)
    {
        GetDrawList()->AddText(ImVec2(x, y), ImColor(color), text);
    }

    void DrawDistance(float playerX, float playerY, float entityX, float entityY, ImVec4 color, float textX, float textY) 
    {
        float distance = std::sqrt(std::pow(entityX - playerX, 2) + std::pow(entityY - playerY, 2));
        distance /= 100.0f; 
        std::stringstream distanceStream;
        distanceStream.precision(1);
        distanceStream << std::fixed << distance; 
        std::string distanceText = distanceStream.str() + " m";
        GetDrawList()->AddText(ImVec2(textX, textY), ImColor(color), distanceText.c_str());
    }
}