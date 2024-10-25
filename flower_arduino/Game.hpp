/**
 * @author      : stanleyarn (stanleyarn@$HOSTNAME)
 * @file        : Game
 * @created     : Thursday Oct 24, 2024 15:48:26 CEST
 */

#ifndef GAME_HPP
#define GAME_HPP

#include <Arduino.h>
#include <Wifi.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <cmath>

#define JSMN_HEADER
#include <jsmn.h>

class Flower
{
        public:
                static const int bitmap_center_radius = 5;
                static constexpr unsigned char bitmap_centre[] PROGMEM =
                {
                        0x70, 0xf8, 0xf8, 0xf8, 0x70
                };

                static const int bitmap_petal_height = 20;
                static const int bitmap_petal_width = 3;
                static constexpr unsigned char bitmap_petal[] PROGMEM = 
                {
                        0x40, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0x60, 0x60, 0x60, 
                        0x60, 0x60, 0x60, 0x40
                };

                static const int petal_number = 20;
                static const int center_radius = 5;

                bool petals[petal_number] = {  };
                int rot = 0;
                int last_rm_petal = -1;
                int remaining_petals = petal_number;
};

enum class Player: uint8_t
{
        HOST,
        GUEST
};

class Game
{
        public:
                Game();
                virtual ~Game();

                Flower flower;
                Player current_player = Player::GUEST;
                Player type_of_player;
                Adafruit_SSD1306 *display;
                Keypad *keypad;
                WiFiClient *oponnent;
                int remaining_petal_to_remove = 3;
                bool isover = false;

                void init();
                void check_gameover();
                virtual void init_wifi(const char *ssid, const char *password) = 0;
                virtual void update();
                void callback(const char *data);
                void send_data();
        private:
                void draw_rotated_petal(float theta, int xo, int yo);
                int jsoneq(const char *json, jsmntok_t *tok, const char *s);
};

#endif 

