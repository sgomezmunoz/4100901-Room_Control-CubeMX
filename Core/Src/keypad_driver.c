#include "keypad_driver.h"

// La matriz de mapeo de teclas ya estaba definida, lo cual es correcto.
// Es 'static' porque solo se usa dentro de este archivo.
static const char keypad_map[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

/**
 * @brief Inicializa las filas del keypad poniéndolas en estado BAJO.
 * @param keypad Puntero a la estructura de configuración del keypad.
 * @note Este es un paso crucial. Al poner las filas en BAJO, cuando una tecla
 *       conecta una fila (LOW) a una columna (que está en HIGH por el pull-up),
 *       se genera el flanco de bajada que dispara la interrupción.
 */
void keypad_init(keypad_handle_t* keypad) {
    for (int i = 0; i < KEYPAD_ROWS; i++) {
        HAL_GPIO_WritePin(keypad->row_ports[i], keypad->row_pins[i], GPIO_PIN_RESET);
    }
}

/**
 * @brief Escanea el keypad después de una interrupción para encontrar la tecla presionada.
 * @param keypad Puntero a la estructura de configuración del keypad.
 * @param col_pin El pin específico de la columna que causó la interrupción.
 * @return El carácter de la tecla presionada ('1', 'A', etc.) o '\0' si es una lectura inválida.
 */
char keypad_scan(keypad_handle_t* keypad, uint16_t col_pin) {
    char key_pressed = '\0';
    int col_index = -1;

   
    for (int i = 0; i < KEYPAD_COLS; i++) {
        if (col_pin == keypad->col_pins[i]) {
            col_index = i;
            break;
        }
    }

    if (col_index == -1) {
        return '\0'; 
    }

    // Si el pin no era de una columna de nuestro keypad, salimos.
    if (col_index == -1) {
        return '\0';
    }
    HAL_Delay(2);

    // MODIFICADO: Lógica de escaneo simplificada y robusta.
    // Escanea las filas para encontrar cuál está causando la conexión.
    for (int row = 0; row < KEYPAD_ROWS; row++) {
        // Poner todas las filas en ALTO
        for (int i = 0; i < KEYPAD_ROWS; i++) {
            HAL_GPIO_WritePin(keypad->row_ports[i], keypad->row_pins[i], GPIO_PIN_SET);
        }

        // Poner la fila actual en BAJO
        HAL_GPIO_WritePin(keypad->row_ports[row], keypad->row_pins[row], GPIO_PIN_RESET);

        // Si la columna que generó la interrupción sigue en BAJO, encontramos la tecla
        if (HAL_GPIO_ReadPin(keypad->col_ports[col_index], keypad->col_pins[col_index]) == GPIO_PIN_RESET) {
            key_pressed = keypad_map[row][col_index];
            break; // Salir del bucle una vez encontrada la tecla
        }
    }

    // Restaurar el estado: todas las filas en BAJO para la próxima interrupción.
    for (int i = 0; i < KEYPAD_ROWS; i++) {
        HAL_GPIO_WritePin(keypad->row_ports[i], keypad->row_pins[i], GPIO_PIN_RESET);
    }
    
    return key_pressed;
}