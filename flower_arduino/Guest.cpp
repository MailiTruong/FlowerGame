/**
 * @author      : stanleyarn (stanleyarn@$HOSTNAME)
 * @file        : Guest
 * @created     : Thursday Oct 24, 2024 18:08:38 CEST
 */

#include "Guest.hpp"


Guest::Guest() : Game()
{

}

Guest::~Guest()
{
}

void Guest::init_wifi(const char *ssid, const char *password)
{
        WiFi.begin(ssid, password);

        type_of_player = Player::GUEST;

        while (WiFi.status() != WL_CONNECTED) 
        {
                delay(500);
                Serial.println("Connecting to server...");
        }

        Serial.println("Connected to server");
}


void Guest::update()
{
        WiFiClient client;

        if (client.connect("flower.local", 80)) {
                oponnent = &client;
                client.println("Hello from client!");
                Game::init();

                while (client.connected()) {
                        if (client.available()) {
                                String response = client.readStringUntil('\n');
                                Serial.println("Response: " + response);

                                Game::callback(response.c_str());
                        }

                        Game::update();
                }
        }

        client.stop();
        delay(5000);
}
