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

#define JSMN_HEADER
#include <jsmn.h>

struct Flower
{
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
                void init(Adafruit_SSD1306 *display, Keypad *keypad);
                virtual void init_wifi(const char *ssid, const char *password) = 0;
                virtual void start() = 0;

        protected:
                Adafruit_SSD1306 *display;
                Keypad *keypad;
                WiFiClient *opponent;
                Flower flower;
                Player current_player = Player::GUEST;
                Player player;
                int remaining_petal_to_remove = 3;
                bool isover = false;

                void update();
                void tcp_callback(const char *data);

        private:
                void send_data();
                bool check_gameover();
                void toggle_player();
                void draw_rotated_petal(float theta, int xo, int yo);
                int jsoneq(const char *json, jsmntok_t *tok, const char *s);
};

#endif 

