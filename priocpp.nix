{ self
, stdenv
, lib

, cmake
, fmt_8
, gtest
, jsoncpp
, pkgconfig

, logmich
, sexpcpp
, tinycmmc
}:

stdenv.mkDerivation {
  pname = "priocpp";
  version = "0.0.0";

  src = lib.cleanSource ./.;

  cmakeFlags = [
    "-DBUILD_EXTRA=ON"
    "-DBUILD_TESTS=ON"
  ];

  postFixup = ""
  + (lib.optionalString stdenv.targetPlatform.isWindows ''
    # This is rather ugly, but functional. Nix has a 'win-dll-link.sh'
    # for this, but that's currently broken:
    # https://github.com/NixOS/nixpkgs/issues/38451
    mkdir -p $out/bin/

    ln -sfv ${jsoncpp}/bin/*.dll $out/bin/
    ln -sfv ${fmt_8}/bin/*.dll $out/bin/
  '');

  nativeBuildInputs = [
    cmake
    pkgconfig
  ];

  propagatedBuildInputs = [
    fmt_8
    gtest
    jsoncpp

    logmich
    sexpcpp
    tinycmmc
  ];
}
