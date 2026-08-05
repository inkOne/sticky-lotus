#pragma once

#include <string>

#include "sticky_lotus/EliminationReason.h"

namespace sticky_lotus {

    class DeathMessageGenerator
    {
    public:
        static std::string randomMessage(
            EliminationReason reason
        );
    };

}