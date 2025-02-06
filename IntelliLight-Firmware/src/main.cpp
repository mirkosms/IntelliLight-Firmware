// main.cpp
#include "led_controller.h"

void setup() {
    initLEDs();
    // calibrateLEDs();  // Przetestujmy, jak diody rzeczywiście świecą
    // Dodatkowe czyszczenie na wypadek, gdyby pierwsze nie zadziałało
    clearLEDs();
    showLEDs();
    delay(100);  // Krótkie opóżnienie na zresetowanie diod
}

void loop() {
    // 1. Neutralne białe światło (50% ciepłe, 50% zimne)
    setWhiteTemperature(128, 128);
    showLEDs();
    delay(3000);  // 3 sekundy wyświetlania

    // 2. Zimne białe światło
    setWhiteTemperature(255, 0);
    showLEDs();
    delay(3000);

    // 3. Ciepłe białe światło
    setWhiteTemperature(0, 255);
    showLEDs();
    delay(3000);
} 
