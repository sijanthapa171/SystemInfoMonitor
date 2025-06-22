{
  description = "System Info Monitor - A system hardware monitoring tool";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          config = {
            allowUnfree = true;  # Enable unfree packages if needed
          };
        };

        # Optional NVIDIA dependencies
        nvidiaPackages = with pkgs; [
          linuxPackages.nvidia_x11
        ];

        # Base dependencies
        basePackages = with pkgs; [
          gcc
          gnumake
          pciutils    # For lspci
        ];
      in
      {
        packages.default = pkgs.stdenv.mkDerivation {
          name = "system-info-monitor";
          version = "1.0.0";

          src = ./.;

          nativeBuildInputs = basePackages;

          buildPhase = ''
            make
          '';

          installPhase = ''
            mkdir -p $out/bin
            cp build/system-info-monitor $out/bin/
          '';

          meta = with pkgs.lib; {
            description = "A system hardware monitoring tool";
            homepage = "https://github.com/sijanthapa171/SystemInfoMonitor";
            license = licenses.mit;
            platforms = platforms.linux;
            maintainers = [];
          };
        };

        # Default development shell without NVIDIA support
        devShells.default = pkgs.mkShell {
          buildInputs = basePackages;
        };

        # Development shell with NVIDIA support
        devShells.nvidia = pkgs.mkShell {
          buildInputs = basePackages ++ nvidiaPackages;
        };
      }
    );
} 