
#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

#include "Guest.hpp"
#include "Host.hpp"

Adafruit_SSD1306 display(128, 64, &Wire, -1);

const byte ROWS = 3;
const byte COLS = 3; 
char hexaKeys[ROWS][COLS] = {
        { 'U', 'L', 'D' },
        { 'R', 'S', ' ' },
        { 'A', 'B', ' ' },
};

byte rowPins[ROWS] = { 20, 10, 0 };
byte colPins[COLS] = { 7, 8, 1 };

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

Game *game;

void setup() 
{
        Serial.begin(115200);
        Wire.begin(5, 6);
        srand(analogRead(0));

        if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
        {
                Serial.println(F("SSD1306 allocation failed"));
                for(;;);
        }

        display.clearDisplay();

        // Choose host or join menu
        char anwser = 0;
        display.clearDisplay();

        display.setTextSize(1); 
        display.setTextColor(WHITE);

        display.setCursor(0, 10);
        display.println("<A> Join a party");
        display.println("<B> Host a party");

        display.display();

        while(1)
        {
                anwser = customKeypad.getKey();
                if (anwser == 'A' || anwser == 'B') break;
                delay(100);
        }

        // Join menu
        if (anwser == 'A')
        {
                int iname = 0;
                char name[9] = "________";
                int icode = 0;
                char code[9] = "________";
                char *edited = (char *)name;
                int *iedited = &iname;

                while(1)
                {
                        display.clearDisplay();

                        char key = customKeypad.getKey();

                        if (key == 'D' || key == 'U')
                        {
                                if (edited == name) 
                                {
                                        edited = (char *)code;
                                        iedited = &icode;
                                }
                                else if (edited == code) 
                                {
                                        edited = (char *)name;
                                        iedited = &iname;
                                }
                        }
                        else if (key == 'B' || key == 'A')
                        {
                                if (*iedited < 8)
                                {
                                        edited[*iedited] = key;
                                        (*iedited)++;
                                }
                        }
                        else if (key == 'S')
                        {
                                if (icode == 8)
                                {
                                        for (int i = 0; i < 8; i++)
                                        {
                                                if (name[i] == '_') name[i] = 0;
                                                if (code[i] == '_') code[i] = 0;
                                        }

                                        game = new Guest();
                                        game->display = &display;
                                        game->keypad = &customKeypad;
                                        game->init_wifi(name, code);
                                        break;
                                }
                        }

                        display.setCursor(0, 10);
                        display.println("Name:" + String(name));
                        display.println("Code:" + String(code));

                        display.display();
                        delay(100);
                }
        }
        // Host menu
        else if (anwser == 'B')
        {
                int iname = 0;
                char name[9] = "________";
                char code[9] = {  };

                while(1)
                {
                        display.clearDisplay();

                        char key = customKeypad.getKey();

                        if (key == 'B' || key == 'A')
                        {
                                if (iname < 8)
                                {
                                        name[iname] = key;
                                        iname++;
                                }
                        }
                        else if (key == 'S')
                        {
                                for (int i = 0; i < 8; i++)
                                {
                                        if (name[i] == '_') name[i] = 0;
                                }

                                for (int i = 0; i < 8; i++)
                                {
                                        code[i] = (char)(65 + (rand() % 2));
                                }

                                code[8] = '\0';
                                game = new Host();
                                game->display = &display;
                                game->keypad = &customKeypad;
                                game->init_wifi(name, code);
                                break;
                        }

                        display.setCursor(0, 10);
                        display.println("Name:" + String(name));

                        display.display();
                        delay(100);
                }   

                display.clearDisplay();
                display.setCursor(0, 10);
                display.println("Party name:" + String(name));
                display.println("Party code:" + String(code));
                display.println("<Select> to start");
                display.display();

                while(1)
                {
                        char key = customKeypad.getKey();
                        if (key == 'S') break;
                        delay(100);
                }
        }
}

void loop()
{
        display.clearDisplay();
        display.setCursor(0, 10);
        display.println("Loading...");
        display.display();

        game->update();
}
