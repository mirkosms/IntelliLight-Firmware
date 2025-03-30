#ifndef CONFIG_H
#define CONFIG_H

// Definicje stałych dla paska LED
#define LED_PIN 14       // Pin ESP32 podłączony do Data In taśmy LED
#define NUM_LEDS 30     // Liczba diod na taśmie

// Globalne ustawienia konfiguracyjne
extern bool autoBrightnessEnabled; // Tryb automatycznej regulacji jasności (domyślnie wyłączony)
extern bool motionEnabled;         // Tryb czujnika ruchu (domyślnie wyłączony)
extern unsigned long motionTimeout; // Czas bezczynności (w ms, domyślnie 60000 ms)

#endif // CONFIG_H
