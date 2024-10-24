{
  description = "Raylib Project";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, nixpkgs }: {
    devShell = nixpkgs.mkShell {
      buildInputs = [
        nixpkgs.raylib
        nixpkgs.gcc
        nixpkgs.make
      ];

      shellHook = ''
        echo "Nix shell for Raylib application is ready!"
        echo "Use 'make' to build your project."
      '';
    };

    packages.x86_64-linux = rec {
      myApp = nixpkgs.stdenv.mkDerivation {
        pname = "my-raylib-app";
        version = "0.1.0";

        src = ./.;

        buildInputs = [
          nixpkgs.raylib
          nixpkgs.gcc
        ];

        buildPhase = ''
          gcc -o main main.c -I${nixpkgs.raylib}/include -L${nixpkgs.raylib}/lib -lraylib -lm -lpthread -ldl -lrt -lX11
        '';

        installPhase = ''
          mkdir -p $out/bin
          mv main $out/bin/
        '';
      };
    };
  };
}
