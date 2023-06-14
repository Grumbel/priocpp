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

, withSexpcpp ? true
, withJsoncpp ? true
}:

stdenv.mkDerivation {
  pname = "priocpp";
  version = "0.0.0";

  src = lib.cleanSource ./.;

  cmakeFlags = [
    "-DBUILD_EXTRA=ON"
    "-DBUILD_TESTS=ON"
  ]
  ++ [(if withJsoncpp then "-DPRIO_USE_JSONCPP=ON" else "-DPRIO_USE_JSONCPP=OFF")]
  ++ [(if withSexpcpp then "-DPRIO_USE_SEXPCPP=ON" else "-DPRIO_USE_SEXPCPP=OFF")];

  postFixup = ""
  + (lib.optionalString stdenv.targetPlatform.isWindows ''
    # This is rather ugly, but functional. Nix has a 'win-dll-link.sh'
    # for this, but that's currently broken:
    # https://github.com/NixOS/nixpkgs/issues/38451
    mkdir -p $out/bin/

    ln -sfv ${fmt_8}/bin/*.dll $out/bin/
  ''
  + (lib.optionalString withJsoncpp
    ''ln -sfv ${jsoncpp}/bin/*.dll $out/bin/''));

  nativeBuildInputs = [
    cmake
    pkgconfig
  ];

  propagatedBuildInputs = [
    fmt_8
    gtest

    logmich
    tinycmmc
  ]
  ++ (lib.optional withJsoncpp jsoncpp)
  ++ (lib.optional withSexpcpp sexpcpp);
}
