
#include "../external cheat/Render/Render.h"
#include "../external cheat/Cheat/Cheat.h"
#include <thread>




 
    int main()
    {
       
        //get client
        std::thread Readthread(&Reader::ThreadLoop, &reader);
        Readthread.detach();

        //funcitions for cheat
        std::thread CheatThread(&Reader::Loopcheat, &reader);
        CheatThread.detach();


        render.ThreadRender();
    }