#pragma once

namespace TTOT::Core::Events
{
    enum class GameLoopAction
    {
        Exitgame
    };
    struct GameLoopEvent
    {
        GameLoopAction action;
    };
}