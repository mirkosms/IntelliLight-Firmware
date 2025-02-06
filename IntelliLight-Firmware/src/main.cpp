// main.cpp
#include "led_controller.h"

void setup() {
    initLEDs();
    clearLEDs();
    showLEDs();
    delay(100);
}

void loop() {
    // 1. Test regulacji jasności (odkomentuj, aby przetestować)
    // setAllLEDs(255, 255, 255);  // Ustawienie białego koloru przed regulacją jasności
    // showLEDs();

    // setBrightness(255);  // Maksymalna jasność
    // showLEDs();          // Odświeżenie wyświetlania
    // delay(2000);

    // setBrightness(50);   // Minimalna jasność
    // showLEDs();          // Odświeżenie wyświetlania
    // delay(2000);

    // 2. Test efektu tęczy (odkomentuj, aby przetestować)
    rainbowEffect(50);

    // 3. Test efektu migotania (odkomentuj, aby przetestować)
    // twinkleEffect(CRGB::Blue, 30, 200);

    // 4. Test strefowego ustawienia koloru (odkomentuj, aby przetestować)
    // setZoneColor(0, NUM_LEDS / 2, CRGB::Red);    // Pierwsza połowa na czerwono
    // setZoneColor(NUM_LEDS / 2, NUM_LEDS - 1, CRGB::Green);  // Druga połowa na zielono
    // showLEDs();
    // delay(3000);

    // 5. Test efektu pulsacji (odkomentuj, aby przetestować)
    // pulsingEffect(CRGB::Purple, 30);

    // 6. Test trybu nocnego (odkomentuj, aby przetestować)

    // Ustaw tryb dzienny przed trybem nocnym dla porównania
    // setBrightness(255);               // Pełna jasność (tryb dzienny)
    // setAllLEDs(255, 255, 255);        // Białe światło
    // showLEDs();
    // delay(3000);                      // Czas na obserwację jasnego światła

    // nightMode();                      // Przełączenie na tryb nocny
    // delay(5000);                      // Czas na obserwację ciemniejszego światła
} 
