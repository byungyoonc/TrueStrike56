#include "dip_switch_matrix.h"
#include <stdint.h>
#include "util.h"

const pin_t dip_pins[] = ENCODER_SW_PINS;
const uint8_t dip_rows[] = DIRECT_SW_ROWS;
const uint8_t dip_cols[] = DIRECT_SW_COLS;

#define DIP_PINS_COUNT ARRAY_SIZE(dip_pins)

#define MATRIX_INPUT_PRESSED_STATE 0

int dip_init(void) {
    for (uint8_t idx = 0; idx < DIP_PINS_COUNT; idx++) {
        pin_t pin = dip_pins[idx];
        if (pin != NO_PIN) {
            gpio_set_pin_input_high(pin);
        }
    }

    return 0;
}

static inline bool readMatrixPin(pin_t pin) {
    if (pin != NO_PIN) {
        return gpio_read_pin(pin) == MATRIX_INPUT_PRESSED_STATE;
    } else {
        return false;
    }
}

bool dip_matrix_scan(matrix_row_t *current_matrix) {
    bool updated = false;

    for (uint8_t idx = 0; idx < DIP_PINS_COUNT; idx++) {
        uint8_t row = dip_rows[idx];
        uint8_t col = dip_cols[idx];
        pin_t pin = dip_pins[idx];
        matrix_row_t current_row_value = current_matrix[row];
        matrix_row_t row_shifter = 1 << col;
        bool is_pressed = readMatrixPin(pin);
        if (is_pressed) {
            current_row_value &= ~(row_shifter);
        }
        else {
            current_row_value |= row_shifter;
        }
        updated |= current_matrix[row] ^ current_row_value;
        current_matrix[row] = current_row_value;
    }

    return updated;
}
