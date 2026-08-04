#pragma once

#include <cstddef>

namespace sticky_lotus::app {

    /**
     * Alle derzeit verfügbaren Ansichten der Anwendung.
     */
    enum class ScreenId
    {
        Game,
        Settings,
        CommanderDamage,
        Poison
    };

    /**
     * Zentraler Navigationszustand.
     *
     * selectedPlayer wird später beim Commander-Damage-Screen
     * als Quelle des Commander-Schadens verwendet.
     */
    class NavigationState
    {
    public:
        [[nodiscard]]
        ScreenId currentScreen() const
        {
            return currentScreen_;
        }

        [[nodiscard]]
        std::size_t selectedPlayer() const
        {
            return selectedPlayer_;
        }

        void showGame()
        {
            currentScreen_ = ScreenId::Game;
        }

        void showSettings()
        {
            currentScreen_ = ScreenId::Settings;
        }

        void showCommanderDamage(
            const std::size_t playerIndex
        )
        {
            selectedPlayer_ = playerIndex;
            currentScreen_ = ScreenId::CommanderDamage;
        }
        void showPoison(
            const std::size_t playerIndex
        )
        {
            selectedPlayer_ = playerIndex;
            currentScreen_ = ScreenId::Poison;
        }

    private:
        ScreenId currentScreen_ = ScreenId::Game;
        std::size_t selectedPlayer_ = 0;
    };

} // namespace sticky_lotus::app