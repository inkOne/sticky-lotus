#pragma once

#include "sticky_lotus/input/InputProvider.h"

namespace sticky_lotus_raylib {

    /**
     * Raylib-Implementierung der plattformunabhängigen Eingabeschnittstelle.
     *
     * Simulatorbelegung:
     *
     * Maus links       -> Touch
     * Pfeil links      -> linke Sticky-Taste
     * Leertaste        -> mittlere Sticky-Taste
     * Pfeil rechts     -> rechte Sticky-Taste
     * Escape           -> Zurück/Abbrechen
     */
    class RaylibInputProvider final
        : public sticky_lotus::input::InputProvider
    {
    public:
        [[nodiscard]]
        sticky_lotus::input::InputFrame poll() override;

    private:
        static constexpr float swipeThreshold = 60.0F;

        bool gestureActive_ = false;
        sticky_lotus::ui::Point gestureStart_{};

        [[nodiscard]]
        sticky_lotus::input::PointerState readPointer() const;

        [[nodiscard]]
        sticky_lotus::input::HardwareButtonState
        readHardwareButtons() const;

        [[nodiscard]]
        sticky_lotus::input::GestureEvent updateGesture(
            const sticky_lotus::input::PointerState& pointer
        );
    };

} // namespace sticky_lotus_raylib