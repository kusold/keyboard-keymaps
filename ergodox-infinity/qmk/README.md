# Ergodox Infinity QMK
Not Working:
Color change on Layer or Caps Lock

## Compiling

1. Set the layout
```bash
export LAYOUT="input_club_ergodox_infinity_layout_ergodox_mine.json"
```

2. Convert the json to c
```bash
docker run -it --rm \
  -v $PWD/$LAYOUT:/layout.json \
  -v $PWD/build/:/build \
  qmkfm/qmk_firmware \
  qmk json2c -o /build/keymap_gen.c /layout.json
```

3. Build the left half
```bash
docker run -it --rm \
  -v $PWD:/qmk_firmware/keyboards/input_club/ergodox_infinity/keymaps/docker-mount \
  -v $PWD/build/:/build \
  qmkfm/qmk_firmware \
  bash -c "make ergodox_infinity:docker-mount && cp /qmk_firmware/ergodox_infinity_docker-mount.bin /build/left.bin"
```

4. Build the right half
```bash
docker run -it --rm \
  -v $PWD:/qmk_firmware/keyboards/input_club/ergodox_infinity/keymaps/docker-mount \
  -v $PWD/build/:/build \
  qmkfm/qmk_firmware \
  bash -c "make ergodox_infinity:docker-mount MASTER=right && cp /qmk_firmware/ergodox_infinity_docker-mount.bin /build/right.bin"
```