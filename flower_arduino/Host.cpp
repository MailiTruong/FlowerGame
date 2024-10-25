/**
 * @author      : stanleyarn (stanleyarn@$HOSTNAME)
 * @file        : Host
 * @created     : Thursday Oct 24, 2024 18:08:08 CEST
 */

#include "Host.hpp"

void Host::init_wifi(const char *ssid, const char *password)
{
        player = Player::HOST;

        WiFi.softAP(ssid, password, 6, true);
        Serial.println("Access Point Started");
        Serial.print("IP Address: ");
        Serial.println(WiFi.softAPIP()); 

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

void Host::start()
{
        WiFiClient client = server.available();

        if (client) 
        {
                opponent = &client;
                client.println("Hello guest !");

                while (client.connected()) 
                {
                        if (client.available()) 
                        {
                                String message = client.readStringUntil('\n');
                                Serial.println("Received: " + message);

                                Game::tcp_callback(message.c_str());
                        }

                        Game::update();
                        delay(10);
                }

                client.stop(); 
        }
}
