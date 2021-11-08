{
  description = "A S-Expression Parser for C++";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
    tinycmmc.url = "gitlab:grumbel/cmake-modules";
    logmich.url = "gitlab:logmich/logmich";
    sexpcpp.url = "gitlab:lispparser/sexp-cpp";
  };

  outputs = { self, nix, nixpkgs, flake-utils, tinycmmc, logmich, sexpcpp }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in rec {
        packages = flake-utils.lib.flattenTree {
          priocpp = pkgs.stdenv.mkDerivation {
            pname = "priocpp";
            version = "0.0.0";
            src = nixpkgs.lib.cleanSource ./.;
            nativeBuildInputs = [
              pkgs.cmake
              pkgs.ninja
              pkgs.gcc
              pkgs.pkgconfig
              tinycmmc.defaultPackage.${system}
            ];
            buildInputs = [
              logmich.defaultPackage.${system}
              sexpcpp.defaultPackage.${system}
              pkgs.glm
              pkgs.jsoncpp
            ];
           };
        };
        defaultPackage = packages.priocpp;
      });
}
