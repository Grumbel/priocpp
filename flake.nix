{
  description = "Property I/O for C++";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";

    logmich.url = "git+https://github.com/logmich/logmich.git";
    logmich.inputs.nixpkgs.follows = "nixpkgs";
    # logmich still pulls tinycmmc transitively; we no longer depend on it here

    sexpcpp.url = "git+https://github.com/lispparser/sexp-cpp.git";
    sexpcpp.inputs.nixpkgs.follows = "nixpkgs";
    sexpcpp.inputs.flake-utils.follows = "flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, logmich, sexpcpp }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        packages = rec {
          default = priocpp;

          priocpp = pkgs.callPackage ./priocpp.nix {
            inherit self;
            logmich = logmich.packages.${system}.default;
            sexpcpp = sexpcpp.packages.${system}.default;
          };

          priocpp-sexp = pkgs.callPackage ./priocpp.nix {
            inherit self;
            logmich = logmich.packages.${system}.default;
            sexpcpp = sexpcpp.packages.${system}.default;
            withJsoncpp = false;
            withSexpcpp = true;
          };

          priocpp-json = pkgs.callPackage ./priocpp.nix {
            inherit self;
            logmich = logmich.packages.${system}.default;
            sexpcpp = null;
            withJsoncpp = true;
            withSexpcpp = false;
          };
        };
      }
    );
}
