


/*






uint16_t line[160];

for (int x = 0; x < 160; x++) {
    line[x] = blend_rgb565(bg[x], fg[x], alpha);
}

st7735_push_pixels(line, 160);
// Sending 160 pixels at once is much faster than per-pixel SPI writes.



/*
If you’re not already using DMA, you’re leaving performance on the table.

ESP-IDF: spi_device_queue_trans()

Arduino-ESP32: SPI.writeBytes() with DMA-capable buffers

Your line buffer should be:
*/
//uint16_t line[160] __attribute__((aligned(4)));







/*

uint16_t line[160] __attribute__((aligned(4)));

for (int y = 0; y < 128; y++) {
    for (int x = 0; x < 160; x++) {
        line[x] = blend_rgb565(bg[y][x], fg[y][x], alpha);
    }
    tft.pushColors(line, 160, true);
}




tft.setSPISpeed(40000000); // 40 MHz (try 27 MHz if unstable)
tft.setRotation(1); // or 3 depending on wiring

*/
