{
  description = "Sticky Lotus - Life Counter development environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-26.05-darwin";
  };

  outputs = { nixpkgs, ... }:
    let
      system = "x86_64-darwin";
      pkgs = import nixpkgs { inherit system; };
    in {
      devShells.${system}.default = pkgs.mkShell {
        packages = with pkgs; [
          clang
          cmake
          ninja
          pkg-config
          raylib
          glfw
          git
        ];

        shellHook = ''
          echo "Sticky Life Counter environment loaded"
          echo "System: ${system}"
        '';
      };
    };
}
