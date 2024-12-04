#include QMK_KEYBOARD_H
#include "version.h"

/**
 * Copied from https://github.com/qmk/qmk_firmware/blob/c8fe565ee6528942cfc71187e32237b39ab7320a/keyboards/input_club/ergodox_infinity/keymaps/default/keymap.c
 */
// enum custom_layers
// {
//   BASE, // default layer
//   SYMB, // symbols
//   MDIA, // media keys
// };

enum custom_keycodes
{
  PLACEHOLDER = SAFE_RANGE, // can always be here
  EPRM,
  VRSN,
  RGB_SLD
};

/**
 * START: Replace with output from json2c
 */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_ergodox(KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_ESC, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_LBRC, KC_ESC, KC_A, KC_S, KC_D, KC_F, KC_G, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_RBRC, KC_LCTL, KC_LGUI, KC_LALT, TG(1), TG(2), KC_DEL, KC_MPLY, KC_HOME, KC_SPC, KC_BSPC, KC_END, KC_CAPS, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, TT(1), KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_LEFT, KC_UP, KC_DOWN, KC_RGHT, TT(2), KC_PSCR, KC_INS, KC_PGUP, KC_PGDN, KC_ENT, KC_SPC),
	[1] = LAYOUT_ergodox(KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_TRNS, KC_TRNS, KC_EXLM, KC_AT, KC_LCBR, KC_RCBR, KC_PIPE, KC_TRNS, KC_TRNS, KC_HASH, KC_DLR, KC_LPRN, KC_RPRN, KC_GRV, KC_TRNS, KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD, KC_TRNS, EE_CLR, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, LM_NEXT, KC_TRNS, KC_TRNS, LM_BRID, LM_BRIU, KC_TRNS, KC_TRNS, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_TRNS, KC_PAST, KC_7, KC_8, KC_9, KC_PMNS, KC_F12, KC_PSLS, KC_4, KC_5, KC_6, KC_PPLS, KC_TRNS, KC_TRNS, KC_LGUI, KC_1, KC_2, KC_3, KC_PEQL, KC_TRNS, KC_TRNS, KC_P0, KC_PDOT, KC_EQL, KC_TRNS, LM_TOGG, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	[2] = LAYOUT_ergodox(QK_BOOT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_U, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN1, KC_TRNS, KC_TRNS, KC_TRNS, KC_WH_U, KC_BTN1, KC_BTN2, KC_WH_D, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, QK_BOOT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};
/**
 * END: Replace with output from json2c
 */

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  switch (keycode)
  {
  // dynamically generate these.
  case EPRM:
    if (record->event.pressed)
    {
      eeconfig_init();
    }
    return false;
    break;
  case VRSN:
    if (record->event.pressed)
    {
      SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
    }
    return false;
    break;
  case RGB_SLD:
    if (record->event.pressed)
    {
#ifdef RGBLIGHT_ENABLE
      rgblight_mode(1);
#endif
    }
    return false;
    break;
  }
  return true;
}

// Runs constantly in the background, in a loop.
void matrix_scan_user(void)
{

  uint8_t layer = get_highest_layer(layer_state);

  ergodox_board_led_off();
  ergodox_right_led_1_off();
  ergodox_right_led_2_off();
  ergodox_right_led_3_off();
  switch (layer)
  {
    // TODO: Make this relevant to the ErgoDox EZ.
  case 1:
    ergodox_right_led_1_on();
    break;
  case 2:
    ergodox_right_led_2_on();
    break;
  default:
    // none
    ergodox_right_led_3_on();
    break;
  }
}

#ifdef ST7565_ENABLE
void st7565_task_user(void)
{
  st7565_clear();
  switch (get_highest_layer(layer_state))
  {
  case 0:
    static const char PROGMEM qmk_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00};
    st7565_write_P(qmk_logo, false);
    break;
  case 1:
    st7565_write_P(PSTR("Layer: L1\n"), false);
    break;
  case 2:
    st7565_write_P(PSTR("Layer: L2\n"), false);
    break;
  default:
    // Or use the write_ln shortcut over adding '\n' to the end of your string
    st7565_write_ln_P(PSTR("Layer: Undefined"), false);
  }

  // Host Keyboard LED Status
  led_t led_state = host_keyboard_led_state();
  st7565_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
  st7565_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
  st7565_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
}
#endif