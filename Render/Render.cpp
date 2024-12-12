#include "../Render/Render.h"
#include "../memory/memory.h"
#include "../utils/globals.h"
#include "../Render/Overlay.h"
#include <algorithm> 
//imguis

#include <thread>
#include "../Cheat/Cheat.h"
#include "../imgui/imgui.h"


//render menu and esp
void Render::ThreadRender()
{

    overlay.shouldRun = true;
    overlay.RenderMenu = false;

    overlay.CreateOverlay();
    overlay.CreateDevice();
    overlay.CreateImGui();

    printf("[>>] Hit insert to show the menu in this overlay!\n");

    overlay.SetForeground(GetConsoleWindow());

    while (overlay.shouldRun)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        overlay.StartRender();

        cheat.RenderESP();
        render.RenderDbg();

        if (overlay.RenderMenu) 
            overlay.Render();
        
       

        // if you want to render here, you could move the imgui includes to your .hpp file instead of the .cpp file!
        overlay.EndRender();
    }

    overlay.DestroyImGui();
    overlay.DestroyDevice();
    overlay.DestroyOverlay();
    

}

//fov circle


template <typename T>
T clamp(T value, T min, T max) {
	return (value < min) ? min : (value > max) ? max : value;
}

void Render::RenderDbg()
{
    if (g.showFov)
    {
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        float distance = 10000000.f;
        // Obtém o viewport principal
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 viewportPos = viewport->Pos;
        ImVec2 viewportSize = viewport->Size;

        // Calcula o centro da tela manualmente
        ImVec2 center;
        center.x = viewportPos.x + (viewportSize.x * 0.5f);
        center.y = viewportPos.y + (viewportSize.y * 0.5f);

        // Obtém o tempo atual
        float time = ImGui::GetTime(); // Tempo em segundos desde o início do aplicativo

        // Calcula a cor com base no tempo
        float red = 0.5f + 0.5f * std::sin(time);   // Varia de 0.0 a 1.0
        float green = 0.5f + 0.5f * std::sin(time + 2.0f); // Varia de 0.0 a 1.0, mas deslocado no tempo
        float blue = 0.5f + 0.5f * std::sin(time + 4.0f); // Varia de 0.0 a 1.0, deslocado no tempo

        // Garante que a cor esteja no intervalo de 0.0 a 1.0
        red = std::clamp(red, 0.0f, 1.0f);
        green = std::clamp(green, 0.0f, 1.0f);
        blue = std::clamp(blue, 0.0f, 1.0f);

        // Define a cor atual
        ImU32 color = ImColor(red, green, blue, 1.0f); // Totalmente opaco

        // Desenha um círculo pequeno preto preenchido no centro
        drawList->AddCircleFilled(center, 3.0f, ImColor(0.0f, 0.0f, 0.0f, 1.0f));

        // Desenha um círculo branco menor preenchido no centro
        drawList->AddCircleFilled(center, 2.0f, ImColor(1.0f, 1.0f, 1.0f, 1.0f));

        // Desenha o círculo do FOV com a cor que muda
        drawList->AddCircle(center, g.Aimfov , color, 0, 1.f); // O 0 define a suavização padrão e 1.0f a espessura da borda
        if (distance < g.Aimfov)
            return;
    }
}
