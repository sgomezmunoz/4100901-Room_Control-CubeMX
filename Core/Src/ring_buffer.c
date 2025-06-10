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
/// @param data         Pointer to where the read data will be stored.
/// @return             True if read was successful, false if the buffer is empty.

bool ring_buffer_read(ring_buffer_t *rb, uint8_t *data)
{
    // Check if the buffer is empty
    if (ring_buffer_is_empty(rb)) {
        return false; // Buffer is empty, cannot read
    }

    // Read data from the buffer at the tail index
    *data = rb->buffer[rb->tail];
    // Move the tail index forward
    rb->tail = (rb->tail + 1) % rb->capacity;
    // Reset full flag since we are reading data
    rb->is_full = false;

    return true; // Read successful
}
/// @brief  Returns the number of elements currently in the ring buffer.
uint16_t ring_buffer_count(ring_buffer_t *rb)
{
    if (rb->is_full) {
        return rb->capacity; // If the buffer is full, return its capacity
    }
    
    // Calculate the number of elements in the buffer
    if (rb->head >= rb->tail) {
        return rb->head - rb->tail; // Normal case
    } else {
        return rb->capacity - (rb->tail - rb->head); // Wrap-around case
    }

}
/// @brief  Checks if the ring buffer is empty. 
/// @param rb           Pointer to the ring buffer structure.

bool ring_buffer_is_empty(ring_buffer_t *rb)
{  
    // If head and tail are equal and the buffer is not full, it is empty
    return (rb->head == rb->tail && !rb->is_full);

}
/// @brief  Checks if the ring buffer is full.
/// @param rb           Pointer to the ring buffer structure.
bool ring_buffer_is_full(ring_buffer_t *rb)
{   
    // If the buffer is full, the head and tail indices are equal
    return rb->is_full;

}
/// @brief  Flushes the ring buffer, clearing all data.
void ring_buffer_flush(ring_buffer_t *rb)
{   
    rb->head = 0;               // Reset head index to 0
    rb->tail = 0;               // Reset tail index to 0
    rb->is_full = false;        // Set full flag to false
    // Optionally, you can clear the buffer memory if needed
    // memset(rb->buffer, 0, rb->capacity * sizeof(uint8_t));

}