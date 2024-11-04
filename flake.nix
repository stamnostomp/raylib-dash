{
  description = "Raylib Project";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, nixpkgs }: let
    # Use a function to import nixpkgs with a specific system
    system = "x86_64-linux"; # Change to your system architecture if needed
    pkgs = import nixpkgs { inherit system; };

    # Define the package attribute set
    packages = {
      myApp = pkgs.stdenv.mkDerivation {
        pname = "raylib-gauge";
        version = "0.1.0";

        src = ./.;

        buildInputs = [
          pkgs.raylib
          pkgs.gcc
        ];

        buildPhase = ''
          gcc -o main main.c \
            -I${pkgs.raylib}/include \
            -L${pkgs.raylib}/lib \
            -lraylib -lm -lpthread -ldl -lrt -lX11
        '';

        installPhase = ''
          mkdir -p $out/bin
          mv main $out/bin/raylib-gauge
        '';
      };
    };
  in {
    # Provide a default package for the flake
    defaultPackage.x86_64-linux = packages.myApp;

    # Define a development shell
    devShell.x86_64-linux = pkgs.mkShell {
      buildInputs = [
        pkgs.raylib
        pkgs.gcc
        pkgs.make
      ];

      shellHook = ''
        echo "Nix shell for Raylib application is ready!"
        echo "Use 'make' to build your project."
      '';
    };
  };
}
