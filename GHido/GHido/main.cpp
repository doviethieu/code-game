// GHido.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DefineGame.h"
#include "GameMain.h"

int main(int argc, char* argv[])
{
    bool bInit = GameMain::GetInstance()->Init();
    bool bInitData = GameMain::GetInstance()->InitData();

    if (bInit && bInitData)
    {
        GameMain::GetInstance()->LoopGame();
    }

    GameMain::GetInstance()->Close();

    return 0;
}

