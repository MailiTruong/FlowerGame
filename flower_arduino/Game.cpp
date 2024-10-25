/**
 * @author      : stanleyarn (stanleyarn@$HOSTNAME)
 * @file        : Game
 * @created     : Thursday Oct 24, 2024 18:11:26 CEST
 */

#include "Game.hpp"
#include <cstring>

Game::Game()
{

}

Game::~Game()
{

}

void Game::init()
{

}

void Game::update()
{
        // Game display and logic
        if (isover) return;

        char key = keypad->getKey();

        display->clearDisplay();

        if (type_of_player == current_player)
        {
                // View of the player current player
                if (key == 'L')
                {
                        flower.rot++;
                        if (flower.rot > flower.petal_number - 1) flower.rot = 0;
                }
                else if (key == 'R')
                {
                        flower.rot--;
                        if (flower.rot < 0) flower.rot = flower.petal_number - 1;
                }
                else if (key == 'D' && remaining_petal_to_remove > 0 && flower.petals[flower.rot] != 1)
                {
                        remaining_petal_to_remove--;
                        flower.remaining_petals--;
                        flower.last_rm_petal = flower.rot;
                        flower.petals[flower.rot] = 1;

                        check_gameover();
                        if (isover)
                        {
                                send_data();
                                display->display();
                                return;
                        }
                }
                else if (key == 'A' && remaining_petal_to_remove < 3)
                {
                        if (current_player == Player::HOST) current_player = Player::GUEST;
                        else if (current_player == Player::GUEST) current_player = Player::HOST;
                        remaining_petal_to_remove = 3;
                }

                if (remaining_petal_to_remove <= 0)
                {
                        if (current_player == Player::HOST) current_player = Player::GUEST;
                        else if (current_player == Player::GUEST) current_player = Player::HOST;
                        remaining_petal_to_remove = 3;
                }

                if (key)
                {
                        send_data();
                }

                display->setCursor(4, display->height() - 8);
                display->println(String(remaining_petal_to_remove));
                display->setCursor(2, 2);
                display->println("your");
                display->setCursor(display->width() - 30, 2);
                display->println("turn");
        }
        else
        {
                check_gameover();
                if (isover)
                {
                        display->display();
                        return;
                }
                display->setCursor(2, 2);
                display->println("wait..");
        }

        display->drawBitmap((display->width() - Flower::bitmap_center_radius) / 2, (display->height() - Flower::bitmap_center_radius) / 2, Flower::bitmap_centre, Flower::bitmap_center_radius, Flower::bitmap_center_radius, WHITE);

        for (int i = 0; i < flower.petal_number; i++)
        {
                float theta = 2 * 3.1415f / Flower::petal_number * i;
                int xo = display->width() / 2 + flower.center_radius * cos(theta);
                int yo = display->height() / 2 + flower.center_radius * sin(theta);
                if (flower.petals[(i + flower.rot + 3 * flower.petal_number / 4) % flower.petal_number] == 0)
                {
                        draw_rotated_petal(theta, xo, yo); 
                }
        }

        display->fillTriangle(62, 64, 66, 64, 64, 59, WHITE);

        display->display();
}

void Game::draw_rotated_petal(float theta, int xo, int yo)
{
        int x_rot;
        int y_rot;
        unsigned char *bitmap = (unsigned char *)Flower::bitmap_petal;

        for (int i = 0; i < Flower::bitmap_petal_height; i++)
        {
                int mask = 0;
                int display_data = 0;
                int x = i;
                for (int j = 0; j < Flower::bitmap_petal_width; j++)
                {
                        int y = j;
                        if (mask == 0)
                        {
                                display_data = pgm_read_byte(bitmap++);
                                mask = 0x80;
                        }

                        if (display_data & mask)
                        {
                                x_rot = cos(theta)*x - sin(theta)*y;
                                y_rot = sin(theta)*x + cos(theta)*y;
                                display->drawPixel(xo + x_rot, y_rot + yo, WHITE);
                        }
                        mask >>= 1;
                }
        }
}

void Game::check_gameover()
{
        if (flower.remaining_petals <= 0)
        {
                if (current_player == type_of_player)
                {
                        display->setCursor(4, 2);
                        display->println("you loose");
                        /* display->setCursor(display->width() - 40, 2); */
                        /* display->println("loose"); */
                }
                else
                {
                        display->setCursor(4, 2);
                        display->println("you win");
                        /* display->setCursor(display->width() - 30, 2); */
                        /* display->println("win"); */
                }
                isover = true;
        }
}

int Game::jsoneq(const char *json, jsmntok_t *tok, const char *s) 
{
        if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start && strncmp(json + tok->start, s, tok->end - tok->start) == 0) 
        {
                return 0;
        }
        return -1;
}

void Game::callback(const char *data)
{
        // Sync the two games
        jsmn_parser p;
        jsmn_init(&p);
        jsmntok_t tokens[128];
        int num_tokens = jsmn_parse(&p, data, strlen(data), tokens, 128);

        if (num_tokens > 0)
        {
                for (int i = 1; i < num_tokens; i++) {
                        if (jsoneq(data, &tokens[i], "rot") == 0) 
                        {
                                char buf[32];
                                snprintf(buf, tokens[i + 1].end - tokens[i + 1].start + 1, "%s", data + tokens[i + 1].start);
                                flower.rot = atoi(buf);
                        } 
                        else if (jsoneq(data, &tokens[i], "rm_pet") == 0) 
                        {
                                char buf[32];
                                snprintf(buf, tokens[i + 1].end - tokens[i + 1].start + 1, "%s", data + tokens[i + 1].start);
                                flower.last_rm_petal = atoi(buf);
                                if (flower.last_rm_petal != -1)
                                        flower.petals[flower.last_rm_petal] = 1;
                        } 
                        else if (jsoneq(data, &tokens[i], "cur_player") == 0) 
                        {
                                char buf[32];
                                snprintf(buf, tokens[i + 1].end - tokens[i + 1].start + 1, "%s", data + tokens[i + 1].start);
                                current_player = (Player)atoi(buf);
                        } 
                        else if (jsoneq(data, &tokens[i], "rem_pet") == 0) 
                        {
                                char buf[32];
                                snprintf(buf, tokens[i + 1].end - tokens[i + 1].start + 1, "%s", data + tokens[i + 1].start);
                                flower.remaining_petals = atoi(buf);
                        } 
                }
        }
}

void Game::send_data()
{
        // Send game info to sync
        char buf[64] = {  };
        snprintf(buf, 64, "{\"rot\":\"%i\", \"rem_pet\":\"%i\", \"rm_pet\":\"%i\", \"cur_player\":\"%i\"}", flower.rot, flower.remaining_petals, flower.last_rm_petal, (int)current_player);

        oponnent->println(buf);
}
