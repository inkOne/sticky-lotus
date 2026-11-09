#pragma once

#include "sticky_lotus/ui/Geometry.h"

namespace sticky_lotus::input {

    /**
     * Aktueller Zustand einer Touch- oder Zeigereingabe.
     *
     * Im Simulator stammen die Werte von der Maus.
     * Auf dem Sticky stammen sie später vom Touchcontroller.
     */
    struct PointerState
    {
        ui::Point position{};

        bool pressed = false;
        bool down = false;
        bool released = false;
    };

    /**
     * Die von Sticky Lotus unterstützten Gesten.
     */
    enum class TouchGesture
    {
        None,
        Tap,
        LongPress,
        SwipeLeft,
        SwipeRight,
        SwipeUp,
        SwipeDown
    };

    /**
     * Eine vollständig erkannte Geste.
     *
     * Eine Geste wird beim Loslassen des Fingers beziehungsweise
     * der Maustaste abgeschlossen.
     */
    struct GestureEvent
    {
        TouchGesture gesture = TouchGesture::None;

        ui::Point startPosition{};
        ui::Point endPosition{};
    };

    /**
     * Zustand der drei physischen Sticky-Tasten.
     */
    struct HardwareButtonState
    {
        bool leftPressed = false;
        bool centerPressed = false;
        bool rightPressed = false;
    };

    /**
     * Sämtliche Eingaben eines einzelnen Programmdurchlaufs.
     *
     * cancelPressed ist nur eine allgemeine Zurück-/Abbrechen-Aktion.
     * Im Simulator wird sie durch Escape ausgelöst. Auf dem Sticky
     * kann sie später unbelegt bleiben oder passend zugeordnet werden.
     */
    struct InputFrame
    {
        PointerState pointer{};
        GestureEvent gesture{};
        HardwareButtonState buttons{};

        bool cancelPressed = false;

        /*
         * Würfeln bzw. Würfelauswahl öffnen. Auf dem Sticky löst die
         * Firmware beides direkt über Application aus; im Simulator
         * kommen sie von den Tasten R und D.
         */
        bool dicePressed = false;
        bool diceSelectorPressed = false;
    };

    /**
     * Plattformunabhängige Eingabeschnittstelle.
     *
     * Implementierungen:
     *
     * - RaylibInputProvider für den Desktop-Simulator
     * - später StickyInputProvider für Touch und Hardwaretasten
     */
    class InputProvider
    {
    public:
        virtual ~InputProvider() = default;

        /**
         * Liest alle seit dem letzten Programmdurchlauf
         * entstandenen Eingaben.
         */
        [[nodiscard]]
        virtual InputFrame poll() = 0;

        /**
         * Schaltet akustisches Klickfeedback auf Eingaben ein
         * oder aus, sofern die Plattform das unterstützt.
         *
         * Standardmäßig ein No-op - nicht jede Plattform hat
         * dafür Hardware (z.B. der Desktop-Simulator).
         */
        virtual void setSoundEnabled(bool enabled)
        {
            (void)enabled;
        }
    };

} // namespace sticky_lotus::input