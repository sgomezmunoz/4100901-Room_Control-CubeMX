#ifndef KEYPAD_DRIVER_H
#define KEYPAD_DRIVER_H

#include "main.h"
#include <stdint.h>
#include "stm32l4xx_hal.h" // O la cabecera HAL correspondiente a tu microcontrolador

// Definiciones para el tamaño del teclado
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

// Estructura para manejar los pines y puertos del keypad
typedef struct {
    GPIO_TypeDef* row_ports[KEYPAD_ROWS];
    uint16_t      row_pins[KEYPAD_ROWS];
    GPIO_TypeDef* col_ports[KEYPAD_COLS];
    uint16_t      col_pins[KEYPAD_COLS];
} keypad_handle_t;

// --- Prototipos de Funciones Públicas ---

/**
 * @brief Inicializa el estado de las filas del keypad para la detección por interrupción.
 * @param keypad Puntero a la estructura del keypad.
 */
void keypad_init(keypad_handle_t* keypad);

/**
 * @brief Escanea el keypad tras una interrupción para identificar la tecla presionada.
 * @param keypad Puntero a la estructura del keypad.
 * @param col_pin El pin de la columna que generó la interrupción.
 * @return El carácter de la tecla ('0'-'9', 'A'-'D', '*', '#') o '\0' si es inválida.
 */
char keypad_scan(keypad_handle_t* keypad, uint16_t col_pin);

#endif /* INC_KEYPAD_DRIVER_H_ */