#include "sticky_lotus_sticky/StickyInputProvider.h"

#include "sticky_lotus/ui/AppRenderer.h"

#include "esp_log.h"
#include "touch_service.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <mutex>

namespace sticky_lotus_sticky {

using sticky_lotus::input::GestureEvent;
using sticky_lotus::input::InputFrame;
using sticky_lotus::input::TouchGesture;
using sticky_lotus::ui::Point;

namespace {

constexpr const char* logTag =
    "StickyInput";

} // namespace

StickyInputProvider::StickyInputProvider()
{
    /*
     * Eventhandler zuerst registrieren.
     *
     * Der Touch-Service ruft diese Funktion aus seinem
     * eigenen FreeRTOS-Task auf.
     */
    touch_service::SetEventHandler(
        &StickyInputProvider::touchEventHandler,
        this
    );

    const esp_err_t result =
        touch_service::Init();

    if (result != ESP_OK) {
        ESP_LOGE(
            logTag,
            "Touch initialization failed: %s",
            esp_err_to_name(result)
        );

        return;
    }

    ESP_LOGI(
        logTag,
        "Touch input initialized"
    );
}

void StickyInputProvider::touchEventHandler(
    const touch_service::TouchEventInfo& event,
    void* context
)
{
    if (context == nullptr) {
        return;
    }

    auto* provider =
        static_cast<StickyInputProvider*>(context);

    provider->handleTouchEvent(event);
}

    Point StickyInputProvider::mapTouchPoint(
        const std::uint16_t touchX,
        const std::uint16_t touchY
    )
{
    /*
     * Touchcontroller:
     * touchX = 0 ... 479
     * touchY = 0 ... 799
     *
     * Display:
     * screenX = 0 ... 799
     * screenY = 0 ... 479
     *
     * X bleibt horizontal gespiegelt.
     * Y wird zusätzlich gespiegelt, damit oben und unten
     * wieder dem sichtbaren Spielerfeld entsprechen.
     */
    const float screenX =
    std::clamp(
        static_cast<float>(touchY),
        0.0F,
        static_cast<float>(
            sticky_lotus::ui::AppRenderer::screenWidth - 1
        )
    );

    const float screenY =
        std::clamp(
            static_cast<float>(
                sticky_lotus::ui::AppRenderer::screenHeight -
                1 -
                touchX
            ),
            0.0F,
            static_cast<float>(
                sticky_lotus::ui::AppRenderer::screenHeight - 1
            )
        );

    return {
        screenX,
        screenY
    };
}

GestureEvent StickyInputProvider::detectGesture(
    const Point start,
    const Point end
)
{
    const float deltaX =
        end.x - start.x;

    const float deltaY =
        end.y - start.y;

    const float absoluteX =
        std::abs(deltaX);

    const float absoluteY =
        std::abs(deltaY);

    TouchGesture gesture =
        TouchGesture::Tap;

    if (
        absoluteX >= swipeThreshold ||
        absoluteY >= swipeThreshold
    ) {
        if (absoluteX > absoluteY) {
            gesture =
                deltaX < 0.0F
                    ? TouchGesture::SwipeLeft
                    : TouchGesture::SwipeRight;
        } else {
            gesture =
                deltaY < 0.0F
                    ? TouchGesture::SwipeUp
                    : TouchGesture::SwipeDown;
        }
    }

    return {
        .gesture = gesture,
        .startPosition = start,
        .endPosition = end
    };
}

void StickyInputProvider::handleTouchEvent(
    const touch_service::TouchEventInfo& event
)
{
    std::lock_guard<std::mutex> lock(
        mutex_
    );

    switch (event.phase) {
    case touch_service::TouchPhase::kBegin:
        if (event.count == 0) {
            return;
        }

        gestureStart_ =
            mapTouchPoint(
                event.points[0].x,
                event.points[0].y
            );

        lastPosition_ =
            gestureStart_;

        gestureActive_ = true;

        /*
         * Begin und Move werden noch nicht an die Engine
         * weitergegeben. Die App reagiert erst auf eine
         * vollständig abgeschlossene Geste.
         *
         * So verursachen Fingerbewegungen keinen unnötigen
         * E-Paper-Refresh.
         */
        break;

    case touch_service::TouchPhase::kMove:
        if (
            !gestureActive_ ||
            event.count == 0
        ) {
            return;
        }

        lastPosition_ =
            mapTouchPoint(
                event.points[0].x,
                event.points[0].y
            );

        break;

    case touch_service::TouchPhase::kEnd:
        if (!gestureActive_) {
            return;
        }

        gestureActive_ = false;

        pendingFrame_ = {};

        pendingFrame_.gesture =
            detectGesture(
                gestureStart_,
                lastPosition_
            );

        pendingFrame_.pointer.position =
            lastPosition_;

        pendingFrame_.pointer.released =
            true;

        pendingInput_ = true;

        ESP_LOGI(
            logTag,
            "Gesture: %d start=(%.0f,%.0f) end=(%.0f,%.0f)",
            static_cast<int>(
                pendingFrame_.gesture.gesture
            ),
            gestureStart_.x,
            gestureStart_.y,
            lastPosition_.x,
            lastPosition_.y
        );

        break;

    case touch_service::TouchPhase::kNone:
        break;
    }
}

bool StickyInputProvider::hasPendingInput() const
{
    std::lock_guard<std::mutex> lock(
        mutex_
    );

    return pendingInput_;
}

InputFrame StickyInputProvider::poll()
{
    std::lock_guard<std::mutex> lock(
        mutex_
    );

    if (!pendingInput_) {
        return {};
    }

    const InputFrame result =
        pendingFrame_;

    pendingFrame_ = {};
    pendingInput_ = false;

    return result;
}

} // namespace sticky_lotus_sticky