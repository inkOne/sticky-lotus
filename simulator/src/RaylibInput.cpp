#include "sticky_lotus_sim/RaylibInput.h"

#include <cmath>

namespace sticky_lotus_sim {

PointerState RaylibInput::readPointer() const
{
    return {
        .position = GetMousePosition(),

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
RaylibInput::readHardwareButtons() const
{
    return {
        .leftPressed = IsKeyPressed(
            KEY_LEFT
        ),

        .centerPressed = IsKeyPressed(
            KEY_SPACE
        ),

        .rightPressed = IsKeyPressed(
            KEY_RIGHT
        )
    };
}

GestureEvent RaylibInput::updateGesture(
    const PointerState& pointer
)
{
    /*
     * Beginn einer neuen Berührung.
     *
     * Die aktuelle Mausposition wird als Startpunkt gespeichert.
     */
    if (pointer.pressed) {
        gestureStart_ = pointer.position;
        gestureActive_ = true;
    }

    /*
     * Eine Geste wird erst beim Loslassen ausgewertet.
     *
     * Solange die Taste noch gedrückt ist oder keine Berührung
     * begonnen wurde, liefern wir TouchGesture::None zurück.
     */
    if (
        !pointer.released ||
        !gestureActive_
    ) {
        return {};
    }

    gestureActive_ = false;

    /*
     * Bewegung zwischen Start und Ende berechnen.
     */
    const float deltaX =
        pointer.position.x - gestureStart_.x;

    const float deltaY =
        pointer.position.y - gestureStart_.y;

    const float absoluteX =
        std::abs(deltaX);

    const float absoluteY =
        std::abs(deltaY);

    /*
     * Standardmäßig wird eine kurze Bewegung als Tap behandelt.
     */
    TouchGesture detectedGesture =
        TouchGesture::Tap;

    /*
     * Sobald die Bewegung den Schwellwert überschreitet,
     * wird sie als Swipe gewertet.
     *
     * Die Achse mit der größeren Bewegung bestimmt,
     * ob es ein horizontaler oder vertikaler Swipe ist.
     */
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

} // namespace sticky_lotus_sim