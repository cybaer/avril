/*
 * ssd1306.h
 *
 *  Created on: 11.11.2015
 *      Author: cybaer
 */

#ifndef SSD1306_H_
#define SSD1306_H

#include "string.h"
#include "avrlib/time.h"
#include "Adafruit_GFX.h"
#include "avrlib/spi.h"

namespace avrlib
{
static const uint8_t SSD1306_SETCONTRAST = 0x81;
static const uint8_t SSD1306_DISPLAYALLON_RESUME = 0xA4;
static const uint8_t SSD1306_DISPLAYALLON = 0xA5;
static const uint8_t SSD1306_NORMALDISPLAY = 0xA6;
static const uint8_t SSD1306_INVERTDISPLAY = 0xA7;
static const uint8_t SSD1306_DISPLAYOFF = 0xAE;
static const uint8_t SSD1306_DISPLAYON = 0xAF;

static const uint8_t SSD1306_SETDISPLAYOFFSET = 0xD3;
static const uint8_t SSD1306_SETCOMPINS = 0xDA;

static const uint8_t SSD1306_SETVCOMDETECT = 0xDB;

static const uint8_t SSD1306_SETDISPLAYCLOCKDIV = 0xD5;
static const uint8_t SSD1306_SETPRECHARGE = 0xD9;

static const uint8_t SSD1306_SETMULTIPLEX = 0xA8;

static const uint8_t SSD1306_SETLOWCOLUMN = 0x00;
static const uint8_t SSD1306_SETHIGHCOLUMN = 0x10;

static const uint8_t SSD1306_SETSTARTLINE = 0x40;

static const uint8_t SSD1306_MEMORYMODE = 0x20;
static const uint8_t SSD1306_COLUMNADDR = 0x21;
static const uint8_t SSD1306_PAGEADDR =   0x22;

static const uint8_t SSD1306_COMSCANINC = 0xC0;
static const uint8_t SSD1306_COMSCANDEC = 0xC8;

static const uint8_t SSD1306_SEGREMAP = 0xA0;

static const uint8_t SSD1306_CHARGEPUMP = 0x8D;

static const uint8_t SSD1306_EXTERNALVCC = 0x1;
static const uint8_t SSD1306_SWITCHCAPVCC = 0x2;

// Scrolling #defines
static const uint8_t SSD1306_ACTIVATE_SCROLL = 0x2F;
static const uint8_t SSD1306_DEACTIVATE_SCROLL = 0x2E;
static const uint8_t SSD1306_SET_VERTICAL_SCROLL_AREA = 0xA3;
static const uint8_t SSD1306_RIGHT_HORIZONTAL_SCROLL = 0x26;
static const uint8_t SSD1306_LEFT_HORIZONTAL_SCROLL = 0x27;
static const uint8_t SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29;
static const uint8_t SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL = 0x2A;


static const uint8_t BLACK = 0;
static const uint8_t WHITE = 1;
static const uint8_t INVERSE = 2;


  template<uint8_t Width, uint8_t Height, typename spi_master, typename DataControl, typename Reset>
  class SSD1306 : public Adafruit_GFX
  {
  public:

    static const uint8_t SSD1306_LCDWIDTH = Width;
    static const uint8_t SSD1306_LCDHEIGHT = Height;
    static uint8_t buffer[((SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH) / 8)];


    // constructor for hardware SPI - we indicate DataCommand, ChipSelect, Reset
    SSD1306(void)
    : Adafruit_GFX(SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT)
    {}


    static void init(void)
    {

      // set pin directions
      DataControl::set_mode(DIGITAL_OUTPUT);
      Reset::set_mode(DIGITAL_OUTPUT);


      /*if (reset) {*/
        // Setup reset pin direction (used by both SPI and I2C
        Reset::High();
        // VDD (3.3V) goes high at start, lets just chill for a ms
        _delay_ms(1);
        // bring reset low
        Reset::Low();
        // wait 10ms
        _delay_ms(10);
        // bring out of reset
        Reset::High();
        // turn on VCC (9V?)



        // Init sequence for 128x64 OLED module
        command(SSD1306_DISPLAYOFF);                    // 0xAE
        command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
        command(0x80);                                  // the suggested ratio 0x80
        command(SSD1306_SETMULTIPLEX);                  // 0xA8
        command(0x3F);
        command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
        command(0x0);                                   // no offset
        command(SSD1306_SETSTARTLINE | 0x0);            // line #0
        command(SSD1306_CHARGEPUMP);                    // 0x8D
        /*if (vccstate == SSD1306_EXTERNALVCC)
          { ssd1306_command(0x10); }
        else*/
          { command(0x14); }
        command(SSD1306_MEMORYMODE);                    // 0x20
        command(0x00);                                  // 0x0 act like ks0108
        command(SSD1306_SEGREMAP | 0x1);
        command(SSD1306_COMSCANDEC);
        command(SSD1306_SETCOMPINS);                    // 0xDA
        command(0x12);
        command(SSD1306_SETCONTRAST);                   // 0x81
        /*if (vccstate == SSD1306_EXTERNALVCC)
          { ssd1306_command(0x9F); }
        else*/
          { command(0xCF); }
        command(SSD1306_SETPRECHARGE);                  // 0xd9
        /*if (vccstate == SSD1306_EXTERNALVCC)
          { ssd1306_command(0x22); }
        else*/
          { command(0xF1); }
        command(SSD1306_SETVCOMDETECT);                 // 0xDB
        command(0x40);
        command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
        command(SSD1306_NORMALDISPLAY);                 // 0xA6


      command(SSD1306_DISPLAYON);//--turn on oled panel
    }



    void drawPixel(int16_t x, int16_t y, uint16_t color)
    {
      if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
         return;

       // check rotation, move pixel around if necessary
       switch (getRotation())
       {
       case 1:
         swap(x, y);
         x = WIDTH - x - 1;
         break;
       case 2:
         x = WIDTH - x - 1;
         y = HEIGHT - y - 1;
         break;
       case 3:
         swap(x, y);
         y = HEIGHT - y - 1;
         break;
       }

       // x is which column
         switch (color)
         {
           case WHITE:   buffer[x+ (y/8)*SSD1306_LCDWIDTH] |=  (1 << (y&7)); break;
           case BLACK:   buffer[x+ (y/8)*SSD1306_LCDWIDTH] &= ~(1 << (y&7)); break;
           case INVERSE: buffer[x+ (y/8)*SSD1306_LCDWIDTH] ^=  (1 << (y&7)); break;
         }
    }

    static void command(uint8_t c)
    { // SPI only
      spi_master::End();   // SS high()
      DataControl::Low();  // no Data --> Low

      spi_master::Begin();
      fastSPIwrite(c);
      spi_master::End();
    }

    static void display(void)
    {
      command(SSD1306_COLUMNADDR);
      command(0);   // Column start address (0 = reset)
      command(SSD1306_LCDWIDTH-1); // Column end address (127 = reset)

      command(SSD1306_PAGEADDR);
      command(0); // Page start address (0 = reset)
      //#if SSD1306_LCDHEIGHT == 64
        command(7); // Page end address
      /*#endif
      #if SSD1306_LCDHEIGHT == 32
        command(3); // Page end address
      #endif
      #if SSD1306_LCDHEIGHT == 16
        command(1); // Page end address
      #endif
*/
      // SPI only
      spi_master::End();
      DataControl::High();  // Data --> High
      spi_master::Begin();

      for (uint16_t i=0; i<(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/(8)); i++)
      {
        fastSPIwrite(buffer[i]);
        //ssd1306_data(buffer[i]);
      }
      spi_master::End();

    }

    void clear(void)
    {
      memset(buffer, 0, (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8));
    }

  private:
    static inline void fastSPIwrite(uint8_t d)
    {
      spi_master::Send(d);
    }

  };

  template<uint8_t Width, uint8_t Height, typename spi_master, typename DataControl, typename Reset>
  uint8_t SSD1306<Width, Height, spi_master, DataControl, Reset>::buffer[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8];

}

#endif

