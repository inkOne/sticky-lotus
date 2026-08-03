#include "sticky_lotus_raylib/RaylibInputProvider.h"

#include <raylib.h>

#include <cmath>

namespace sticky_lotus_raylib {

using sticky_lotus::input::GestureEvent;
using sticky_lotus::input::HardwareButtonState;
using sticky_lotus::input::InputFrame;
using sticky_lotus::input::PointerState;
using sticky_lotus::input::TouchGesture;

PointerState RaylibInputProvider::readPointer() const
{
    const Vector2 mousePosition = GetMousePosition();

    return {
        .position = {
            mousePosition.x,
            mousePosition.y
        },

        .pressed = IsMouseButtonPressed(
            MOUSE_BUTTON_LEFT
        ),

        .down = IsMouseButtonDown(
            MOUSE_BUTTON_LEFT
        ),

        .released = IsMouseButtonReleased(
            MOUSE_BUTTON_LEFT
        )
    };
}

HardwareButtonState
RaylibInputProvider::readHardwareButtons() const
{
    return {
        .leftPressed = IsKeyPressed(KEY_LEFT),
        .centerPressed = IsKeyPressed(KEY_SPACE),
        .rightPressed = IsKeyPressed(KEY_RIGHT)
    };
}

GestureEvent RaylibInputProvider::updateGesture(
    const PointerState& pointer
)
{
    // Beginn einer neuen Berührung speichern.
    if (pointer.pressed) {
        gestureStart_ = pointer.position;
        gestureActive_ = true;
    }

    // Erst beim Loslassen wird die Geste ausgewertet.
    if (
        !pointer.released ||
        !gestureActive_
    ) {
        return {};
    }

    gestureActive_ = false;

    const float deltaX =
        pointer.position.x - gestureStart_.x;

    const float deltaY =
        pointer.position.y - gestureStart_.y;

    const float absoluteX =
        std::abs(deltaX);

    const float absoluteY =
        std::abs(deltaY);

    TouchGesture detectedGesture =
        TouchGesture::Tap;

    // Bewegung oberhalb des Schwellwerts als Swipe behandeln.
    if (
        absoluteX >= swipeThreshold ||
        absoluteY >= swipeThreshold
    ) {
        if (absoluteX > absoluteY) {
            detectedGesture =
                deltaX < 0.0F
                    ? TouchGesture::SwipeLeft
                    : TouchGesture::SwipeRight;
        } else {
            detectedGesture =
                deltaY < 0.0F
                    ? TouchGesture::SwipeUp
                    : TouchGesture::SwipeDown;
        }
    }

    return {
        .gesture = detectedGesture,
        .startPosition = gestureStart_,
        .endPosition = pointer.position
    };
}

InputFrame RaylibInputProvider::poll()
{
    const PointerState pointer =
        readPointer();

    return {
        .pointer = pointer,
        .gesture = updateGesture(pointer),
        .buttons = readHardwareButtons(),
        .cancelPressed = IsKeyPressed(KEY_ESCAPE)
    };
}

} // namespace sticky_lotus_raylib