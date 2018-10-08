#include "formats/hght.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "libraries/lodepng.h"

typedef uint16_t hght_value;

int hght_conv_hght_png(char* filepath_in, char* filepath_out) {
    // Read input HGHT file
    FILE* file_in = fopen(filepath_in, "r");

    // Get file size
    fseek(file_in, 0, SEEK_END);
    long file_in_length = ftell(file_in);
    rewind(file_in);

    if (file_in_length != sizeof(uint16_t) * 256 * 256) {
        printf("WARNING: Input file is not 256x256 units?!\n");
    }

    // Read file into memory
    uint16_t* buffer_raw = (uint16_t*) malloc(file_in_length);
    fread(buffer_raw, file_in_length, 1, file_in);

    // Close file since we're done with it
    fclose(file_in);

    // Allocate pixel buffer
    char* buffer_pixel = (char*) malloc((file_in_length / sizeof(uint16_t) * sizeof(char) * 4));

    // Fill pixel buffer with a black and white representation of the heightmap
    for (long i = 0; i < file_in_length / sizeof(uint16_t); i++) {
        uint16_t height = buffer_raw[i];
        char real_value = (uint8_t)(((float)height / UINT16_MAX) * UINT8_MAX);
        
        buffer_pixel[i * 4] = real_value;
        buffer_pixel[i * 4 + 1] = real_value;
        buffer_pixel[i * 4 + 2] = real_value;
        buffer_pixel[i * 4 + 3] = 0xFF;
    }

    // Save bitmap to png
    lodepng_encode32_file(filepath_out, buffer_pixel, 256, 256);
}