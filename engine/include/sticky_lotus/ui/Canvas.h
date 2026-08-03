#pragma once

#include "sticky_lotus/ui/Geometry.h"

#include <string_view>

namespace sticky_lotus::ui {

/**
 * Vier abstrakte Helligkeitsstufen.
 *
 * Sie entsprechen bewusst nicht RGB-Farben.
 * Der spätere Sticky-Renderer kann sie direkt auf die
 * verfügbaren E-Ink-Graustufen abbilden.
 */
enum class Ink
{
    White,
    LightGray,
    DarkGray,
    Black
};

/**
 * Horizontale Textausrichtung.
 */
enum class TextAlignment
{
    Left,
    Center,
    Right
};

/**
 * Plattformunabhängige Zeichenoberfläche.
 *
 * Die UI kennt ausschließlich dieses Interface.
 *
 * Konkrete Implementierungen sind beispielsweise:
 *
 * - RaylibCanvas für den Simulator
 * - StickyCanvas für das reale E-Ink-Display
 */
class Canvas
{
public:
    virtual ~Canvas() = default;

    /**
     * Beginnt einen neuen Zeichenvorgang.
     */
    virtual void beginFrame() = 0;

    /**
     * Beendet den Zeichenvorgang.
     */
    virtual void endFrame() = 0;

    /**
     * Löscht die gesamte Zeichenfläche.
     */
    virtual void clear(Ink ink) = 0;

    /**
     * Zeichnet ein gefülltes Rechteck.
     */
    virtual void fillRect(
        const Rect& area,
        Ink ink
    ) = 0;

    /**
     * Zeichnet nur die Umrandung eines Rechtecks.
     */
    virtual void drawRect(
        const Rect& area,
        float thickness,
        Ink ink
    ) = 0;

    /**
     * Zeichnet eine Linie.
     */
    virtual void drawLine(
        Point start,
        Point end,
        float thickness,
        Ink ink
    ) = 0;

    /**
     * Zeichnet einen gefüllten Kreis.
     */
    virtual void fillCircle(
        Point center,
        float radius,
        Ink ink
    ) = 0;

    /**
     * Zeichnet die Umrandung eines Kreises.
     */
    virtual void drawCircle(
        Point center,
        float radius,
        float thickness,
        Ink ink
    ) = 0;

    /**
     * Zeichnet Text innerhalb eines Rechtecks.
     *
     * rotationDegrees wird zunächst nur mit 0 und 180 verwendet.
     */
    virtual void drawText(
        std::string_view text,
        const Rect& area,
        int fontSize,
        Ink ink,
        TextAlignment alignment = TextAlignment::Center,
        float rotationDegrees = 0.0F
    ) = 0;

    /**
     * Markiert einen Bereich für eine spätere Aktualisierung.
     *
     * Raylib benötigt dies nicht wirklich.
     * Der StickyCanvas kann daraus Partial Refreshes erzeugen.
     */
    virtual void invalidate(
        const Rect& area
    ) = 0;

    /**
     * Überträgt alle ausstehenden Änderungen auf das Display.
     */
    virtual void flush() = 0;
};

} // namespace sticky_lotus::ui