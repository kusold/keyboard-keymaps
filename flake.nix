{
  description = "Flake for building qmk firmware";
  # The vial fork of qmk, stores the keyboard config in on-keyboard memory, and 
  # supports the `Vial` GUI key map config app.
  # inputs.vial-qmk = {
  #   url = "git+https://github.com/vial-kb/vial-qmk.git?submodules=1&ref=vial";
  #   flake = false;
  # };
  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-24.05";
  inputs.nixpkgs-unstable.url = "github:nixos/nixpkgs/nixos-unstable";

  inputs.qmk = {
    url = "https://github.com/qmk/qmk_firmware.git?submodules=1&ref=master";
    flake = false;
    type = "git";
    submodules = true;
  };
  inputs.flake-utils.url = "github:numtide/flake-utils";


  outputs = { self, nixpkgs, nixpkgs-unstable, flake-utils, qmk }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        unstable = import nixpkgs-unstable { inherit system; };

        # ergodox
        ergodox-keyboard = "input_club/ergodox_infinity";
        ergodox-keymap = "custom";
      in
      {
        packages = rec {
          ergodox-infinity-qmk = pkgs.stdenv.mkDerivation {
            name = "keymap";
            src = ./.;
            phases = [ "unpackPhase" "unpackQmkPhase" "patchPhase" "buildPhase" ];
            buildInputs = [ unstable.qmk ];
            unpackQmkPhase = ''
              cp -r ${qmk}/* .
              chmod -R u+w .
            '';
            patchPhase = ''
              mkdir -p ./keyboards/${ergodox-keyboard}/keymaps/${ergodox-keymap}
              # Copy the keymap to the qmk dir
              cp -r ./ergodox-infinity/qmk/*.{c,h} ./keyboards/${ergodox-keyboard}/keymaps/${ergodox-keymap}/
            '';

            buildPhase = ''
              mkdir -p $out

              make -j$NIX_BUILD_CORES ${ergodox-keyboard}:list-keymaps
              make -j$NIX_BUILD_CORES ${ergodox-keyboard}:${ergodox-keymap}
              # make -j$NIX_BUILD_CORES ${ergodox-keyboard}:${ergodox-keymap}:dfu-util-split-left
              # make -j$NIX_BUILD_CORES ${ergodox-keyboard}:${ergodox-keymap}:dfu-util-split-right
              cp -r .build/* $out/
            '';
          };
          default = ergodox-infinity-qmk;
        };
        # Build dir is taken as a param of `build` and `flash`,
        # e.g. to flash with the output of `nix build '.?submodules=1'` do `flash result`.
        devShell = pkgs.mkShell {
          # KEYBOARD = keyboard;
          # KEYMAP = keymap;
          KEYBOARD = "input_club/ergodox_infinity";
          KEYMAP = "custom";
          BUILD_DIR = "./build/src";
          # QMK_DIR="${qmk}";

          buildInputs = [ unstable.qmk ];
          shellHook = ''
            export PATH=${unstable.qmk}/bin:$PATH

            setup-qmk() {
              echo "Setting up qmk source code"
              mkdir -p $BUILD_DIR
              cp -r ${qmk}/* $BUILD_DIR
              chmod -R u+w $BUILD_DIR

              sync-keymap
            }

            sync-keymap() {
              echo "Syncing your keymaps to the qmk source code"

              echo "Copying $(ls ./ergodox-infinity/qmk/*.{c,h}) to $BUILD_DIR/keyboards/${ergodox-keyboard}/keymaps/${ergodox-keymap}/"
              mkdir -p $BUILD_DIR/keyboards/${ergodox-keyboard}/keymaps/${ergodox-keymap}
              # Copy the keymap to the qmk dir
              cp -rf ./ergodox-infinity/qmk/*.{c,h} $BUILD_DIR/keyboards/${ergodox-keyboard}/keymaps/${ergodox-keymap}/
            }
            build() {
              sync-keymap;
              pushd $BUILD_DIR
              make -j$NIX_BUILD_CORES ${ergodox-keyboard}:${ergodox-keymap}
              popd
            }
            flash-left() {
              sync-keymap;
              pushd $BUILD_DIR
              make -j$NIX_BUILD_CORES ${ergodox-keyboard}:${ergodox-keymap}:dfu-util-split-left
              popd
            }
            flash-right() {
              sync-keymap;
              pushd $BUILD_DIR
              make -j$NIX_BUILD_CORES ${ergodox-keyboard}:${ergodox-keymap}:dfu-util-split-right
              popd
            }
            setup-qmk;
            type sync-keymap;
            type build;
            type flash-left;
            type flash-right;
          '';
        };
      }
    );
}
