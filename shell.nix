{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    nativeBuildInputs = with pkgs; [
      xorg.libX11.dev
	  gcc12
    ];
}
