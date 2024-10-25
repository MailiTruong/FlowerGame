/**
 * @author      : stanleyarn (stanleyarn@$HOSTNAME)
 * @file        : Host
 * @created     : Thursday Oct 24, 2024 18:07:27 CEST
 */

#ifndef HOST_HPP
#define HOST_HPP

#include "Game.hpp"

#include <ESPmDNS.h>


class Host : public Game
{
        public:
                Host();
                virtual ~Host();

                WiFiServer server = WiFiServer(80);
                void init_wifi(const char *ssid, const char *password);
                void update();

        private:

};

#endif 

