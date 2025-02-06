#include "led_controller.h"

void setup() {
    initLEDs();  // Inicjalizacja LED (FastLED wciąż w setup)
}

void loop() {
    setAllLEDs(255, 255, 255);  // Wszystkie diody na biało
    showLEDs();                 // Wyświetlenie zmian
    delay(1000);
}
