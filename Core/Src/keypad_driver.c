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
    int row_index = -1;
    int col_index = -1;

    // --- PASO 1: Antirrebote (Debounce) ---
    // Una pulsación mecánica no es limpia. Este retardo ignora los rebotes iniciales.
    HAL_Delay(20);

    // --- PASO 2: Identificar la columna presionada ---
    // Buscamos a qué índice (0-3) pertenece el pin que generó la interrupción.
    for (int i = 0; i < KEYPAD_COLS; i++) {
        if (keypad->col_pins[i] == col_pin) {
            // Verificamos si la tecla sigue presionada. Si ya se soltó, fue un ruido.
            if (HAL_GPIO_ReadPin(keypad->col_ports[i], keypad->col_pins[i]) == GPIO_PIN_SET) {
                return '\0'; // Falsa alarma, el pin ya está en alto.
            }
            col_index = i; // Guardamos el índice de la columna.
            break;
        }
    }

    // Si el pin no era de una columna de nuestro keypad, salimos.
    if (col_index == -1) {
        return '\0';
    }

    // --- PASO 3: Identificar la fila presionada ---
    // Para ello, cambiamos la lógica: ponemos todas las filas en ALTO (inactivas)
    // y luego vamos poniendo en BAJO una por una para ver cuál es la que está conectada.
    for (int i = 0; i < KEYPAD_ROWS; i++) {
        // Ponemos todas las filas en ALTO
        for (int j = 0; j < KEYPAD_ROWS; j++) {
            HAL_GPIO_WritePin(keypad->row_ports[j], keypad->row_pins[j], GPIO_PIN_SET);
        }

        // Ponemos solo la fila 'i' en BAJO
        HAL_GPIO_WritePin(keypad->row_ports[i], keypad->row_pins[i], GPIO_PIN_RESET);

        // Si al hacer esto, la columna presionada lee BAJO, ¡hemos encontrado la fila!
        if (HAL_GPIO_ReadPin(keypad->col_ports[col_index], keypad->col_pins[col_index]) == GPIO_PIN_RESET) {
            row_index = i; // Guardamos el índice de la fila.
            break;
        }
    }

    // --- PASO 4: Restaurar el estado del keypad ---
    // Volvemos a poner todas las filas en BAJO para que el keypad esté listo para
    // la próxima interrupción.
    keypad_init(keypad);

    // --- PASO 5: Devolver la tecla ---
    // Si encontramos una fila y una columna válidas...
    if (row_index != -1) {
        // ...buscamos el carácter en nuestra matriz de mapeo.
        key_pressed = keypad_map[row_index][col_index];

        // Esperamos a que la tecla se suelte. Esto evita que una pulsación larga
        // genere múltiples caracteres.
        while (HAL_GPIO_ReadPin(keypad->col_ports[col_index], keypad->col_pins[col_index]) == GPIO_PIN_RESET);
    }
    
    return key_pressed;
}