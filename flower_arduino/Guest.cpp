/**
 * @author      : stanleyarn (stanleyarn@$HOSTNAME)
 * @file        : Guest
 * @created     : Thursday Oct 24, 2024 18:08:38 CEST
 */

#include "Guest.hpp"

void Guest::init_wifi(const char *ssid, const char *password)
{
        player = Player::GUEST;

        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED) 
        {
                delay(500);
                Serial.println("Connecting to server...");
        }

        Serial.println("Connected to server");
}


void Guest::start()
{
        WiFiClient client;

        if (client.connect("flower.local", 80)) {
                opponent = &client;
                client.println("Hello from client!");
                Game::init();

                while (client.connected()) {
                        if (client.available()) {
                                String response = client.readStringUntil('\n');
                                Serial.println("Response: " + response);

                                Game::tcp_callback(response.c_str());
                        }

                        Game::update();
                }
        }

        client.stop();
        delay(5000);
}
