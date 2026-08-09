#pragma once

#include "sticky_lotus/input/InputProvider.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <mutex>

namespace touch_service {
    struct TouchEventInfo;
}

namespace sticky_lotus_sticky {

    class StickyInputProvider final
        : public sticky_lotus::input::InputProvider
    {
    public:
        StickyInputProvider();

        [[nodiscard]]
        sticky_lotus::input::InputFrame poll() override;

        [[nodiscard]]
        bool hasPendingInput() const;

    private:
        static constexpr float swipeThreshold = 45.0F;

        /*
         * Maximal 32 noch nicht verarbeitete Touch-Eingaben.
         *
         * Dadurch gehen schnelle Mehrfachtipps nicht verloren,
         * während die App Eingaben verarbeitet oder das
         * E-Paper später aktualisiert wird.
         */
        static constexpr std::size_t maximumPendingFrames = 32;

        static void touchEventHandler(
            const touch_service::TouchEventInfo& event,
            void* context
        );

        void handleTouchEvent(
            const touch_service::TouchEventInfo& event
        );

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

        /*
         * Ringpuffer für abgeschlossene Gesten.
         */
        std::array<
            sticky_lotus::input::InputFrame,
            maximumPendingFrames
        > pendingFrames_{};

        std::size_t readIndex_ = 0;
        std::size_t writeIndex_ = 0;
        std::size_t pendingCount_ = 0;

        sticky_lotus::ui::Point gestureStart_{};
        sticky_lotus::ui::Point lastPosition_{};

        bool gestureActive_ = false;
        std::int64_t gestureStartTimeUs_ = 0;

        static constexpr std::int64_t longPressThresholdUs =
            500'000;
    };

} // namespace sticky_lotus_sticky