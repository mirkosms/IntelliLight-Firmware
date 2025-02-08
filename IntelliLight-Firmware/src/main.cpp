#include "led_controller.h"

LEDController ledController;

void setup() {
    ledController.init();
}

void loop() {
    ledController.rainbowEffect(50);
    // Odkomentuj poniższe linie, aby przetestować inne efekty
    // ledController.setBrightness(255);
    // ledController.pulsingEffect(CRGB::Purple, 30);
    // ledController.twinkleEffect(CRGB::Blue, 30, 200);
    // ledController.setZoneColor(0, NUM_LEDS / 2, CRGB::Red);
    // ledController.setZoneColor(NUM_LEDS / 2, NUM_LEDS - 1, CRGB::Green);
    // ledController.nightMode();
    // ledController.setWhiteTemperature(128, 128), delay(3000);  // Neutralne białe światło
    // ledController.setWhiteTemperature(255, 0), delay(3000);    // Zimne białe światło
    // ledController.setWhiteTemperature(0, 255), delay(3000);    // Ciepłe białe światło

}
