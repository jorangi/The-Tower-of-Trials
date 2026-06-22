#pragma once
#include "Core/EventBus.h"
#include "Utility/ImageRenderer.h"
#include "Network/ConnGemini.h"
namespace TTOT::Core
{
    struct GameContext
    {
        TTOT::Core::EventBus& eventBus;
        TTOT::Utilities::ImageRenderer& imageRenderer;
        TTOT::Network::ConnGemini& gemini;
        GameContext(EventBus& eb, Utilities::ImageRenderer& ir, Network::ConnGemini& gm)
            : eventBus(eb), imageRenderer(ir), gemini(gm) {}
    };
}