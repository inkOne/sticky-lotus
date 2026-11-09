#pragma once

#include "sticky_lotus/GameState.h"
#include "sticky_lotus/app/NavigationState.h"
#include "sticky_lotus/input/InputProvider.h"
#include "sticky_lotus/screens/ScreenManager.h"
#include "sticky_lotus/ui/AppRenderer.h"
#include "sticky_lotus/ui/Canvas.h"
#include "sticky_lotus/commander/CommanderDamageDraft.h"

#include <chrono>

namespace sticky_lotus::app
{
    /**
     * Zentrale plattformunabhängige Anwendung.
     */
    class Application
    {
    public:
        Application(
            ui::Canvas& canvas,
            input::InputProvider& input
        );

        /**
         * Verarbeitet einen vollständigen Anwendungsdurchlauf.
         */
        void tick();

        /**
         * Muss unabhängig von Eingaben regelmäßig aufgerufen werden
         * (z.B. aus der Hauptschleife neben canvas.serviceRefresh()).
         *
         * Hebt die "Roll Start"-Hervorhebung auf, sobald seit der
         * letzten Eingabe 10 Sekunden ohne Aktivität vergangen sind.
         *
         * Blendet außerdem die Zähler-Änderungsanzeige (z.B. "-5")
         * aus, sobald deren letzte Änderung 15 Sekunden zurückliegt -
         * pro Spieler auf dem Game-Screen (Leben) und dem Poison-
         * Screen (Gift), sowie pro Angreifer auf dem Commander-
         * Damage-Screen.
         */
        void checkTimeouts();

        /**
         * Würfelt einen Startspieler aus und wechselt zum
         * Game-Screen, um ihn hervorzuheben.
         *
         * Für die physische untere Taste gedacht - wirkt bewusst
         * bildschirmunabhängig, genau wie der Power-Button.
         */
        void rollStartingPlayer();

        /**
         * Setzt das Spiel zurück und wechselt zum Game-Screen.
         *
         * Wirkt bildschirmunabhängig, genau wie rollStartingPlayer()
         * und der Power-Button. Die Settings lösen es über die
         * Siegerabfrage aus.
         */
        void resetGame();

        /**
         * Würfelt mit dem gewählten Würfel (oder wirft die Münze) und
         * zeigt das Ergebnis drei Sekunden lang an.
         *
         * Für die mittlere Hardwaretaste gedacht (kurz drücken).
         */
        void rollDice();

        /**
         * Öffnet die Auswahl von Münze, D4, D6, D8, D10, D12 und D20.
         *
         * Für die mittlere Hardwaretaste gedacht (halten).
         */
        void showDiceSelector();

        [[nodiscard]]
        int diceSides() const { return diceSides_; }

        void setDiceSides(int sides);

        /*
         * Spielerprofile (Namen, Siege, Sitzplätze) für die Persistenz.
         * Die Firmware speichert sie, sobald profilesDirty() gesetzt ist.
         */
        [[nodiscard]]
        const ProfileSnapshot& profileSnapshot() const
        {
            return game_.profiles.snapshot();
        }

        bool restoreProfiles(const ProfileSnapshot& saved)
        {
            return game_.profiles.restore(saved);
        }

        [[nodiscard]]
        bool profilesDirty() const
        {
            return game_.profiles.dirty();
        }

        void profilesSaved()
        {
            game_.profiles.saved();
        }

        void draw();

        void setBatteryStatus(
            int batteryPercent,
            bool charging
        );
        void showSleepScreen();
        [[nodiscard]]
        GameSnapshot createGameSnapshot() const;

        bool restoreGameSnapshot(
            const GameSnapshot& snapshot
        );
    private:
        ui::ProfileView profileView_;
        ui::ProfileView::Mode keyboardReturn_ =
            ui::ProfileView::Mode::Scores;

        void handleProfileInput(const input::InputFrame& input);
        void openProfiles(ui::ProfileView::Mode mode);

        int diceSides_ = 6;
        int diceResult_ = 1;

        enum class DiceView { Closed, Selector, Result };
        DiceView diceView_ = DiceView::Closed;
        std::chrono::steady_clock::time_point diceShownAt_{};
        void closeDice();

        GameState game_;
        NavigationState navigation_;
        commander::CommanderDamageDraft commanderDraft_;

        input::InputProvider& input_;

        ui::AppRenderer renderer_;
        screens::ScreenManager screenManager_;

        std::chrono::steady_clock::time_point lastInputTime_ =
            std::chrono::steady_clock::now();
    };
} // namespace sticky_lotus::app
