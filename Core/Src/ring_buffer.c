#include "ring_buffer.h"

void ring_buffer_init(ring_buffer_t *rb, uint8_t *buffer, uint16_t capacity)
{
    rb->buffer = buffer;        // Assign the provided buffer to the ring buffer
    rb->capacity = capacity;    // Set the maximum capacity of the ring buffer
    rb->head = 0;               // Initialize head index to 0
    rb->tail = 0; 
    rb->is_full = false;                 // Initialize tail index to 0

}
/// @brief  Writes data to the ring buffer.
/// @param rb           Pointer to the ring buffer structure.
/// @param data         The data to be written to the buffer.
/// @return             True if write was successful, false if the buffer is full.

bool ring_buffer_write(ring_buffer_t *rb, uint8_t data)
{
    // Check if the buffer is full
    if (rb->is_full) {
        return false; // Buffer is full, cannot write
    }

    // Write data to the buffer at the head index
    rb->buffer[rb->head] = data;
    // Move the head index forward
    rb->head = (rb->head + 1) % rb->capacity;
    // Check if the buffer is now full
    if (rb->head == rb->tail) {
        rb->is_full = true; // Buffer is full
    }
    
    return true; // Write successful
}
/// @brief  Reads data from the ring buffer.
/// @param rb           Pointer to the ring buffer structure.
/// @param data         Pointer to a variable where the read data will be stored.
/// @return             True if read was successful, false if the buffer is empty.
bool ring_buffer_read(ring_buffer_t *rb, uint8_t *data)
{
    // Check if the buffer is empty
    if (rb->head == rb->tail) {
        return false; // Buffer is empty, cannot read
    }

    // Read data from the buffer at the tail index
    *data = rb->buffer[rb->tail];

    // Move the tail index forward
    rb->tail = (rb->tail + 1) % rb->capacity;

    return true; // Read successful
}

uint16_t ring_buffer_count(ring_buffer_t *rb)
{

}

bool ring_buffer_is_empty(ring_buffer_t *rb)
{

}

bool ring_buffer_is_full(ring_buffer_t *rb)
{

}

void ring_buffer_flush(ring_buffer_t *rb)
{

}