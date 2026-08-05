#pragma once

namespace sticky_lotus::ui {

    /**
     * Fest bekannte Grafiken der Anwendung.
     *
     * Die Engine arbeitet ausschließlich mit diesen IDs.
     * Konkrete Dateinamen und Dateiformate kennt nur die Plattform.
     */
    enum class ImageId
    {
        Skull,
        Poison
    };

    /**
     * Bestimmt, wie ein Bild in einen Zielbereich eingepasst wird.
     */
    enum class ImageFit
    {
        /**
         * Das gesamte Bild bleibt sichtbar.
         * Das Seitenverhältnis wird beibehalten.
         */
        Contain,

        /**
         * Der vollständige Zielbereich wird ausgefüllt.
         * Teile des Bildes können abgeschnitten werden.
         */
        Cover,

        /**
         * Das Bild wird exakt auf den Zielbereich gestreckt.
         * Das Seitenverhältnis kann sich dabei verändern.
         */
        Stretch
    };

} // namespace sticky_lotus::ui