#pragma once

#include "sticky_lotus/input/InputProvider.h"

#include <mutex>

namespace touch_service {
    struct TouchEventInfo;
}

namespace sticky_lotus_sticky {

    /**
     * Übersetzt die Ereignisse des GT911-Touchcontrollers
     * in die plattformunabhängigen Sticky-Lotus-Eingaben.
     *
     * Die Gesture-Erkennung entspricht funktional dem
     * RaylibInputProvider des Simulators:
     *
     * - kurze Bewegung: Tap
     * - überwiegend horizontal: SwipeLeft / SwipeRight
     * - überwiegend vertikal: SwipeUp / SwipeDown
     */
    class StickyInputProvider final
        : public sticky_lotus::input::InputProvider
    {
    public:
        StickyInputProvider();

        [[nodiscard]]
        sticky_lotus::input::InputFrame poll() override;

        /**
         * Liefert true, wenn eine abgeschlossene Eingabe
         * für Application::tick() bereitliegt.
         */
        [[nodiscard]]
        bool hasPendingInput() const;

    private:
        static constexpr float swipeThreshold = 45.0F;

        static void touchEventHandler(
            const touch_service::TouchEventInfo& event,
            void* context
        );

        void handleTouchEvent(
            const touch_service::TouchEventInfo& event
        );

        /**
         * Touch-Service arbeitet im Hochformat 480 × 800.
         * Sticky Lotus zeichnet im Querformat 800 × 480.
         *
         * Diese Methode dreht die Koordinaten in das
         * Koordinatensystem des AppRenderers.
         */
        [[nodiscard]]
        static sticky_lotus::ui::Point mapTouchPoint(
            std::uint16_t touchX,
            std::uint16_t touchY
        );

        [[nodiscard]]
        static sticky_lotus::input::GestureEvent detectGesture(
            sticky_lotus::ui::Point start,
            sticky_lotus::ui::Point end
        );

        mutable std::mutex mutex_;

        sticky_lotus::input::InputFrame pendingFrame_{};

        sticky_lotus::ui::Point gestureStart_{};
        sticky_lotus::ui::Point lastPosition_{};

        bool gestureActive_ = false;
        bool pendingInput_ = false;
    };

} // namespace sticky_lotus_sticky