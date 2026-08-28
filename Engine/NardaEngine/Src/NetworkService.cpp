#include "Precompiled.h"
#include "NetworkService.h"

using namespace NardaEngine;
using namespace NardaEngine::Network;

void NetworkService::Update(float deltaTime)
{
    NetworkManager::Get()->Update(deltaTime);
}

void NetworkService::DebugUI()
{
    NetworkManager::Get()->DebugUI();
}