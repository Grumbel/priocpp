{
  description = "A S-Expression Parser for C++";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-21.11";
    flake-utils.url = "github:numtide/flake-utils";

    tinycmmc.url = "gitlab:grumbel/cmake-modules";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";
    tinycmmc.inputs.flake-utils.follows = "flake-utils";

    logmich.url = "gitlab:logmich/logmich";
    logmich.inputs.nixpkgs.follows = "nixpkgs";
    logmich.inputs.flake-utils.follows = "flake-utils";
    logmich.inputs.tinycmmc.follows = "tinycmmc";

    sexpcpp.url = "gitlab:lispparser/sexp-cpp";
    sexpcpp.inputs.nixpkgs.follows = "nixpkgs";
    sexpcpp.inputs.flake-utils.follows = "flake-utils";
    sexpcpp.inputs.tinycmmc.follows = "tinycmmc";
  };

  outputs = { self, nixpkgs, flake-utils, tinycmmc, logmich, sexpcpp }:
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
            ];
            buildInputs = [
              tinycmmc.defaultPackage.${system}
            ];
            propagatedBuildInputs = [
              logmich.defaultPackage.${system}
              sexpcpp.defaultPackage.${system}
              pkgs.jsoncpp
            ];
           };
        };
        defaultPackage = packages.priocpp;
      });
}
