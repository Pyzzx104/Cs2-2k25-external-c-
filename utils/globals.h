#pragma once
#include <cstddef>
#include <Windows.h>
#include <string>
#include "../imgui/imgui.h"




struct globals
{
	// System(Base
	bool Run = true;
	bool ShowMenu = true;
	RECT GameRect{};
	bool Crosshair = false;
	//aim
	bool aimbot = false;
	bool showFov = false;
	float Aimfov = 30.f;
	float Aimsmoothing = 0.f;
	float fovSpedd;
	//visuals
	bool visuals = false;
	int screenfov = 0;
	bool e_box = false;
	
	bool skeleton = false;
	bool distance = false;
	bool lines = false;
	bool name = false;
	bool healthBar = false;
	
	bool noflash = false;
	bool visibleCheck;
	bool AimRCS;
	
	ImVec4 healthBarColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Verde
	ImVec4 linesColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);     // Branco
	ImVec4 skeletonColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);   // Verde
	ImVec4 boxColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);       // preto igual o vt
	ImVec4 nameColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);      // Branco     

	//padrao
	float lineThickness = 1;    // Espessura das linhas
	float boxThickness = 1;     // Espessura das caixas
	float healthBarThickness = 3; // Espessura da barra de saúde
	float skeletonThickness = 1;
};
inline globals g;



namespace offsets
{
	//acess
	
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1855CE8;
	constexpr std::ptrdiff_t dwEntityList = 0x19F2488;
	constexpr std::ptrdiff_t m_iTeamNum = 0x3E3;
	constexpr std::ptrdiff_t m_vOldOrigin = 0x1324; //vector3
	constexpr std::ptrdiff_t m_vecViewOffset = 0xCB0;
	constexpr std::ptrdiff_t m_lifeState = 0x348;
	constexpr std::ptrdiff_t m_entitySpottedState = 0x11A8;
	constexpr std::ptrdiff_t m_bSpotted = 0x8;
	constexpr std::ptrdiff_t m_bSpottedByMask = 0xC;
	constexpr std::ptrdiff_t dwViewRender = 0x1A54D60;
	constexpr std::ptrdiff_t m_modelState = 0x170;
	constexpr std::ptrdiff_t dwViewAngles = 0x1A5E650;
	constexpr std::ptrdiff_t dwViewMatrix = 0x1A54550;
	constexpr std::ptrdiff_t m_pGameSceneNode = 0x328;
	constexpr std::ptrdiff_t m_hPlayerPawn = 0x80C;
	constexpr std::ptrdiff_t m_iHealth = 0x344;
	constexpr std::ptrdiff_t m_aimPunchAngle = 0x1574;
	constexpr std::ptrdiff_t m_hPawn = 0x62C;
	constexpr std::ptrdiff_t m_pCameraServices = 0x11E0;
	constexpr std::ptrdiff_t m_iFOV = 0x210;
	constexpr std::ptrdiff_t m_bIsScoped = 0x23D0;
	constexpr std::ptrdiff_t m_iszPlayerName = 0x660;
	constexpr std::ptrdiff_t m_sSanitizedPlayerName = 0x770;
	constexpr std::ptrdiff_t m_bUseScreenAspectRatio = 0x585; // bool
};

