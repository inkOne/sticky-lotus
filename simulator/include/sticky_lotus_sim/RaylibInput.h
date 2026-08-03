#pragma once

#include <raylib.h>

namespace sticky_lotus_sim {

/**
 * Aktueller Zustand einer Zeiger- beziehungsweise Touch-Eingabe.
 *
 * Im Simulator werden diese Werte aus der linken Maustaste erzeugt.
 * Auf dem Sticky kann später dieselbe Struktur mit Touchdaten befüllt werden.
 */
struct PointerState
{
    Vector2 position{};

    bool pressed = false;
    bool down = false;
    bool released = false;
};

/**
 * Zustand der drei physischen Sticky-Tasten.
 *
 * Belegung im Simulator:
 *
 * Pfeiltaste links  -> linke Sticky-Taste
 * Leertaste         -> mittlere Sticky-Taste
 * Pfeiltaste rechts -> rechte Sticky-Taste
 */
struct HardwareButtonState
{
    bool leftPressed = false;
    bool centerPressed = false;
    bool rightPressed = false;
};

/**
 * Von der Anwendung unterstützte Touchgesten.
 *
 * Der Name TouchGesture verhindert eine Kollision mit dem
 * bereits von Raylib definierten Typ Gesture.
 */
enum class TouchGesture
{
    None,
    Tap,
    SwipeLeft,
    SwipeRight,
    SwipeUp,
    SwipeDown
};

/**
 * Eine vollständig erkannte Geste.
 *
 * Die Geste wird erst beim Loslassen der Maustaste beziehungsweise
 * später beim Abheben des Fingers erzeugt.
 */
struct GestureEvent
{
    TouchGesture gesture = TouchGesture::None;

    Vector2 startPosition{};
    Vector2 endPosition{};
};

/**
 * Eingabeschicht des Raylib-Simulators.
 *
 * Diese Klasse übersetzt:
 *
 * - Mauszustände in PointerState
 * - Tastaturtasten in HardwareButtonState
 * - Mausbewegungen in TouchGesture-Ereignisse
 */
class RaylibInput
{
public:
    /**
     * Liest den aktuellen Mauszustand.
     */
    [[nodiscard]]
    PointerState readPointer() const;

    /**
     * Liest die simulierten Sticky-Hardwaretasten.
     */
    [[nodiscard]]
    HardwareButtonState readHardwareButtons() const;

    /**
     * Erkennt anhand des aktuellen Pointer-Zustands eine Geste.
     *
     * Während die Maustaste noch gedrückt ist, wird ein leeres
     * GestureEvent mit TouchGesture::None zurückgegeben.
     */
    GestureEvent updateGesture(
        const PointerState& pointer
    );

private:
    /**
     * Minimale Bewegung in Pixeln, ab der eine Bewegung
     * als Wischgeste und nicht mehr als Tap gilt.
     */
    static constexpr float swipeThreshold = 60.0F;

    /**
     * Gibt an, ob aktuell eine Berührung verfolgt wird.
     */
    bool gestureActive_ = false;

    /**
     * Startposition der aktuellen Berührung.
     */
    Vector2 gestureStart_{};
};

} // namespace sticky_lotus_sim