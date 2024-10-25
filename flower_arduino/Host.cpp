/**
 * @author      : stanleyarn (stanleyarn@$HOSTNAME)
 * @file        : Host
 * @created     : Thursday Oct 24, 2024 18:08:08 CEST
 */

#include "Host.hpp"

Host::Host() : Game()
{

}

Host::~Host()
{
}

void Host::init_wifi(const char *ssid, const char *password)
{
        WiFi.softAP(ssid, password, 6, true);

        Serial.println("Access Point Started");
        Serial.print("IP Address: ");
        Serial.println(WiFi.softAPIP()); 

        type_of_player = Player::HOST;

        // Start mDNS
        if (!MDNS.begin("flower")) 
        {
                Serial.println("Error setting up mDNS responder!");
                while (1) 
                {
                        delay(1000);
                }
        }
        Serial.println("mDNS responder started");
        MDNS.addService("http", "tcp", 80); 

        server.begin();
}

void Host::update()
{
        WiFiClient client = server.available();

        if (client) 
        {
                oponnent = &client;
                client.println("Hello guest !");
                Game::init();

                while (client.connected()) 
                {
                        if (client.available()) 
                        {
                                String message = client.readStringUntil('\n');
                                Serial.println("Received: " + message);

                                Game::callback(message.c_str());
                        }

                        Game::update();
                        delay(10);
                }

                client.stop(); 
        }
}
