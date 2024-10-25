/**
 * @author      : stanleyarn (stanleyarn@$HOSTNAME)
 * @file        : Guest
 * @created     : Thursday Oct 24, 2024 18:08:21 CEST
 */

#ifndef GUEST_HPP
#define GUEST_HPP

#include "Game.hpp"


class Guest : public Game
{
  public:
    void init_wifi(const char *ssid, const char *password);
    void start();
};

#endif 

