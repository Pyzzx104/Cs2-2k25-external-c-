#include "../Cheat/Cheat.h"
#include "../Math/Vector.h"
#include "../Math/bone.hpp"
#include "../Math/Draw.h"




extern int ScreenCenterX = GetSystemMetrics(SM_CXSCREEN) / 2;
extern int ScreenCenterY = GetSystemMetrics(SM_CYSCREEN) / 2;

Vector3 RCSScale = { 0.5f,0.5f };
int RCSBullet = 1;

void Reader::ThreadLoop()
{

	// get client
	if (!reader.client)
	{
			std::this_thread::sleep_for(std::chrono::milliseconds(15));

			reader.client = mem.GetBase("client.dll");
			std::cout << "client.dll " << std::hex << reader.client << std::endl;


	}
}

void Reader::Loopcheat()
{

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(6));
		cheat.Aimbot();
		cheat.screenfov();

	}
}



void Cheat::Aimbot()
{
	//resolution actual
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	float screenWidth = desktop.right; // width
	float screenHeight = desktop.bottom;// heigth

	// Verifique se a proporção está em uso
	bool useAspectRatio = mem.Read<bool>(reader.client + offsets::m_bUseScreenAspectRatio);
	float aspectRatioX = useAspectRatio ? screenWidth / 1920.0f : 1.0f;
	float aspectRatioY = useAspectRatio ? screenHeight / 1080.0f : 1.0f;

	// Acesso para aimbot
	uintptr_t enityList = mem.Read<uintptr_t>(reader.client + offsets::dwEntityList);
	uintptr_t listEntry = mem.Read<uintptr_t>(enityList + 0x10);
	uintptr_t PawnAddress = mem.Read<uintptr_t>(reader.client + offsets::dwLocalPlayerPawn);
	uint8_t localteam = mem.Read<uint8_t>(PawnAddress + offsets::m_iTeamNum);
	Vector3 localorigin = mem.Read<Vector3>(PawnAddress + offsets::m_vOldOrigin);
	Vector3 localView = mem.Read<Vector3>(PawnAddress + offsets::m_vecViewOffset);
	view_matrix_t viewMatrix = mem.Read<view_matrix_t>(reader.client + offsets::dwViewMatrix);
	Vector3 localviewangles = mem.Read<Vector3>(reader.client + offsets::dwViewAngles);
	Vector3 localEyePos = localorigin + localView;

	// Variáveis de mouse
	float TargetX = 0.f;
	float TargetY = 0.f;
	Vector3 bestTarget;
	double distance = 1000;



	for (int i = 0; i < 64; i++)
	{

		if (listEntry == 0)
			continue;

		uintptr_t currentConroller = mem.Read<uintptr_t>(listEntry + i * 0x78);
		if (currentConroller == 0)
			continue;

		int pawnHandle = mem.Read<int>(currentConroller + offsets::m_hPlayerPawn);
		if (pawnHandle == 0)
			continue;

		uintptr_t listEntry2 = mem.Read<uintptr_t>(enityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10);

		uintptr_t currentPawn = mem.Read<uintptr_t>(listEntry2 + 0x78 * (pawnHandle & 0x1FF));
		if (currentPawn == 0)
			continue;

		uint32_t health = mem.Read<uint32_t>(currentPawn + offsets::m_iHealth);
		uint8_t team = mem.Read<uint8_t>(currentPawn + offsets::m_iTeamNum);
		int lifeState = mem.Read<int>(currentPawn + offsets::m_lifeState);


		if (lifeState != 256)
			continue;

		if (team == localteam)
			continue;



		//visible check
		

		Vector3 origin = mem.Read<Vector3>(currentPawn + offsets::m_vOldOrigin);
		Vector3 view = mem.Read<Vector3>(currentPawn + offsets::m_vecViewOffset);
		uintptr_t sceneNode = mem.Read<uintptr_t>(currentPawn + offsets::m_pGameSceneNode);
		uintptr_t boneMatrix = mem.Read<uintptr_t>(sceneNode + offsets::m_modelState + 0x80);
		Vector3 entityHead = mem.Read<Vector3>(boneMatrix + 6 * 32);
		Vector3 ScreenPos = entityHead.WTS(viewMatrix);

		double currentDistance = sqrt(pow(ScreenPos.x - screenWidth / 2, 2) + pow(ScreenPos.y - screenHeight / 2, 2));
		if (currentDistance < distance)
		{
			distance = currentDistance;
			bestTarget = ScreenPos;
		}
	}

	Vector3 ScreenPos = bestTarget;
	if (distance > g.Aimfov) return;

	// Ajustar a centralização com base na proporção
	float ScreenCenterX = screenWidth / 2;
	float ScreenCenterY = screenHeight / 2;

	if (ScreenPos.x != ScreenCenterX)
	{
		TargetX = (ScreenPos.x - ScreenCenterX) * aspectRatioX;
		TargetX /= (g.Aimsmoothing != 0.0f) ? g.Aimsmoothing : 1.0f;
	}

	if (ScreenPos.y != ScreenCenterY)
	{
		TargetY = (ScreenPos.y - ScreenCenterY) * aspectRatioY;
		TargetY /= (g.Aimsmoothing != 0.0f) ? g.Aimsmoothing : 1.0f;
	}

	if (GetAsyncKeyState(VK_XBUTTON2) && g.aimbot)
	{
		mouse_event(MOUSEEVENTF_MOVE, static_cast<int>(TargetX), static_cast<int>(TargetY), NULL, NULL);
	}
}

void Cheat::RenderESP()
{

	uintptr_t enityList = mem.Read<uintptr_t>(reader.client + offsets::dwEntityList); // ler off da entidade
	
	uintptr_t PawnAddress = mem.Read<uintptr_t>(reader.client + offsets::dwLocalPlayerPawn); // local player
	uint8_t localteam = mem.Read<uint8_t>(PawnAddress + offsets::m_iTeamNum); // meu time
	Vector3 localOrigin = mem.Read<Vector3>(PawnAddress + offsets::m_vOldOrigin); //posição antiga 
	Vector3 localView = mem.Read<Vector3>(PawnAddress + offsets::m_vecViewOffset); // oque ele ta olhando
	view_matrix_t view_matrix = mem.Read<view_matrix_t>(reader.client + offsets::dwViewMatrix); // minha tela
	

	for (int i = 0; i < 64; i++)
	{
		uintptr_t listEntry = mem.Read<uintptr_t>(enityList + 0x10); // list entry para usar no percorrimento
		if (listEntry == 0)
			continue;

		uintptr_t currentConroller = mem.Read<uintptr_t>(listEntry + i * 0x78);
		if (currentConroller == 0)
			continue;

		int pawnHandle = mem.Read<int>(currentConroller + offsets::m_hPlayerPawn);
		if (pawnHandle == 0)
			continue;

		uintptr_t listEntry2 = mem.Read<uintptr_t>(enityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10);

		uintptr_t currentPawn = mem.Read<uintptr_t>(listEntry2 + 0x78 * (pawnHandle & 0x1FF));
		if (currentPawn == 0)
			continue;

		uint32_t health = mem.Read<uint32_t>(currentPawn + offsets::m_iHealth);
		uint8_t team = mem.Read<uint8_t>(currentPawn + offsets::m_iTeamNum);
		int lifeState = mem.Read<int>(currentPawn + offsets::m_lifeState);


		if (lifeState != 256)
			continue;

		if (team == localteam)
			continue;


	

		if (!mem.InForeground()) // Não renderizar em alt tab
			continue;

		//bones
		uintptr_t gamescene = mem.Read<uintptr_t>(currentPawn + offsets::m_pGameSceneNode);
		uintptr_t bonearray = mem.Read<uintptr_t>(gamescene + offsets::m_modelState + 0x80);

		//positions
		Vector3 origin = mem.Read<Vector3>(currentPawn + offsets::m_vOldOrigin); // origin da entidade
		Vector3 head = mem.Read<Vector3>(bonearray + static_cast<unsigned long long>(bones::head) * 32); // cabeça da entidade
		Vector3 feet = mem.Read<Vector3>(bonearray + static_cast<unsigned long long>(bones::right_feet) * 32); // cabeça da entidade



		//head1.z = head.z + 10.f;
		
		Vector3 screenFeetPos = origin.WTS(view_matrix);
		Vector3 screenHeadPos = head.WTS(view_matrix);
		Vector3 feetpos = feet.WTS(view_matrix);

		



		float headHeigth = (feetpos.y - screenHeadPos.y) / 8;
		float height = screenFeetPos.y - screenHeadPos.y + 15.f;
		float width = height / 1.4f;



		if (g.e_box)
			desing::DrawFilledRect(
				screenFeetPos.x - width / 2, //definir o centro
				screenHeadPos.y - headHeigth, // subir a altura do box
				width,
				height,
				g.boxColor,
				g.boxThickness
			);

		

		if (g.healthBar) {
			
			
			desing::DrawHealthBar(
				screenHeadPos.x - height, 
				screenFeetPos.y,
				height,
				g.healthBarColor,
				health,
				g.healthBarThickness
			);
		};

		if(g.distance)
		desing::DrawDistance(
			localOrigin.x,    
			localOrigin.y,
			origin.x,  
			origin.y,
			g.linesColor,
			screenFeetPos.x,   
			screenFeetPos.y
		);

		char pName[128] = {};
		if(g.name)
		if (mem.ReadString(PawnAddress, pName, sizeof(pName))) // endereço do controlador não da entidade
		{
			const char* PlayerName = pName;
			float esquerda = 15.f; //centro

			desing::NameP(
				screenHeadPos.x - esquerda,
				screenHeadPos.y - 18.f,
				g.nameColor,
				" "
			);
		}


		Vector3 screenCenter = { screenWidth / 2.0f, 0.0f }; // centro superior
		Vector3 screenCenterinf = { screenWidth / 2.0f, screenHeight - 1.0f, 0.0f };
		


		if (g.lines)
			desing::Line(
				screenCenter.x,
				screenCenter.y,
				screenHeadPos.x,
				screenHeadPos.y,
				g.linesColor,
				g.lineThickness
			);

		for (int i = 0; i < sizeof(boneConnections) / sizeof(boneConnections[0]); i++)
		{
			int bone1 = boneConnections[i].bone1;
			int bone2 = boneConnections[i].bone2;

			Vector3 VectorBone1 = mem.Read<Vector3>(bonearray + bone1 * 32);
			Vector3 VectorBone2 = mem.Read<Vector3>(bonearray + bone2 * 32);

			Vector3 b1 = VectorBone1.WTS(view_matrix);
			Vector3 b2 = VectorBone2.WTS(view_matrix);

			if (g.skeleton)
			{
				desing::Line(b1.x, b1.y, b2.x, b2.y, g.skeletonColor, g.skeletonThickness);

				desing::Circle(
					screenHeadPos.x,
					screenHeadPos.y,
					headHeigth - 3,
					g.skeletonColor
				);
			}


		}



	}

}


void Cheat::screenfov()
{

	std::uint16_t desiredFov = (uint16_t)g.screenfov;
	std::uintptr_t localPlayer = mem.Read<std::uintptr_t>(reader.client + offsets::dwLocalPlayerPawn);

	std::uintptr_t CameraServices = mem.Read<std::uintptr_t>(localPlayer + offsets::m_pCameraServices);

	std::uint16_t currentFov = mem.Read<std::uint16_t>(CameraServices + offsets::m_iFOV);

	bool isScoped = mem.Read<bool>(localPlayer + offsets::m_bIsScoped);

	if (!isScoped && currentFov != desiredFov)
	{
		mem.Write<uint64_t>(CameraServices + offsets::m_iFOV, desiredFov); //detecd? edited: no
	}

}


