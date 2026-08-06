{
  description = "Sticky Lotus ESP32-S3 firmware environment";

  inputs = {
    /*
     * Eigene Nixpkgs-Basis für die Firmware-Entwicklung.
     *
     * Sie ist bewusst unabhängig von der Simulator-Flake.
     */
    nixpkgs.url =
      "github:NixOS/nixpkgs/nixpkgs-26.05-darwin";
  };

  outputs = { self, nixpkgs }:
    let
      /*
       * Dein bisheriges Setup läuft als x86_64-darwin.
       *
       * Falls du die Firmware später nativ auf Apple Silicon
       * ausführen möchtest, kann hier aarch64-darwin stehen.
       */
      supportedSystems = [
        "x86_64-darwin"
        "aarch64-darwin"
      ];

      forEachSystem =
        nixpkgs.lib.genAttrs supportedSystems;
    in
    {
      devShells = forEachSystem (
        system:
        let
          pkgs = import nixpkgs {
            inherit system;
          };

          /*
           * ESP-IDF 5.5.4 unterstützt moderne Python-Versionen.
           *
           * Python wird hier als Host-Werkzeug bereitgestellt.
           * ESP-IDF erzeugt später eine eigene, versionsgebundene
           * virtuelle Python-Umgebung unter .idf-tools.
           */
          python = pkgs.python312;
        in
        {
          default = pkgs.mkShell {
            packages = [
              # Quellcode und Downloads
              pkgs.git
              pkgs.git-lfs
              pkgs.curl
              pkgs.wget

              # ESP-IDF-Buildsystem
              pkgs.cmake
              pkgs.ninja
              pkgs.ccache
              pkgs.pkg-config
              pkgs.gnumake

              # Parser- und Buildwerkzeuge
              pkgs.flex
              pkgs.bison
              pkgs.gperf

              # Python-Grundlage
              python

              # USB und Flash-Werkzeuge
              pkgs.libusb1
              pkgs.dfu-util

              # Verschiedene ESP-IDF-Voraussetzungen
              pkgs.openssl
              pkgs.libffi
              pkgs.ncurses
            ];

            /*
             * Die ESP-IDF-Installation liegt bewusst innerhalb
             * des Projekts und nicht global unter ~/.espressif.
             *
             * Dadurch bleibt die Umgebung von anderen Projekten
             * und ESP-IDF-Versionen getrennt.
             */
            shellHook = ''
              export STICKY_FIRMWARE_ROOT="$PWD"

              export IDF_VERSION="v5.5.4"
              export IDF_INSTALL_ROOT="$PWD/.esp-idf"
              export IDF_PATH="$IDF_INSTALL_ROOT/esp-idf-$IDF_VERSION"
              export IDF_TOOLS_PATH="$PWD/.idf-tools"

              # Verhindert Konflikte zwischen Nix-Python und der
              # von ESP-IDF verwalteten Python-Umgebung.
              unset PYTHONPATH

              echo
              echo "Sticky Lotus firmware environment"
              echo "System:       ${system}"
              echo "ESP-IDF:      $IDF_VERSION"
              echo "IDF_PATH:     $IDF_PATH"
              echo "IDF_TOOLS:    $IDF_TOOLS_PATH"
              echo

              setup-idf() {
                mkdir -p "$IDF_INSTALL_ROOT"
                mkdir -p "$IDF_TOOLS_PATH"

                if [ ! -d "$IDF_PATH/.git" ]; then
                  echo "Unvollständige ESP-IDF-Kopie wird entfernt ..."
                  rm -rf "$IDF_PATH"

                  echo "Klone ESP-IDF $IDF_VERSION ..."

                  git clone \
                    --branch "$IDF_VERSION" \
                    --recursive \
                    https://github.com/espressif/esp-idf.git \
                    "$IDF_PATH"
                else
                  echo "ESP-IDF-Repository ist bereits vorhanden."

                  git -C "$IDF_PATH" submodule update \
                    --init \
                    --recursive
                fi

                echo
                echo "Installiere die Werkzeuge für ESP32-S3 ..."

                (
                  cd "$IDF_PATH"
                  ./install.sh esp32s3
                )

                source "$IDF_PATH/export.sh"

                echo
                echo "ESP-IDF wurde installiert:"
                idf.py --version
              }

              use-idf() {
                if [ ! -f "$IDF_PATH/export.sh" ]; then
                  echo "ESP-IDF ist nicht vollständig installiert."
                  echo "Führe zuerst setup-idf aus."
                  return 1
                fi

                source "$IDF_PATH/export.sh"
                idf.py --version
              }

              if [ ! -f "$IDF_PATH/export.sh" ]; then
                echo "ESP-IDF ist noch nicht vollständig installiert."
                echo
                echo "Einmalig ausführen:"
                echo "  setup-idf"
                echo
              else
                source "$IDF_PATH/export.sh" >/dev/null

                echo "Aktiv:"
                idf.py --version
                echo
              fi
            '';
          };
        }
      );
    };
}