/*
  This is a simple paint sketch!

  Tested with a 2.4" ILI9341 based display of 320 x 240 pixles

  Uses font 2 only

  Created by Rowboteer: 22/11/15
*/

// Call up the TFT driver library
#include <TFT_ILI9341.h> // Hardware-specific TFT library
#include <SPI.h>

// Call up touch screen library
#include <TFT_Touch.h>

// Invoke custom TFT driver library
TFT_ILI9341 tft = TFT_ILI9341();       // Invoke custom library

// These are the pins used to interface between the 2046 touch controller and Arduino Mega
#define DOUT A0  /* Data out pin of touch screen, direct connection */
#define DIN  A1  /* Data in pin of touch screen via 10K series resistor */
#define DCS  A2  /* Chip select pin of touch screen via 10K series resistor*/
#define DCLK A3  /* Clock pin of touch screen via 10K series resistor*/

/* Create an instance of the touch screen library */
TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);

int ColorPaletteHigh = 30; // Height of palette boxes
int color = TFT_WHITE;     //Starting paint brush color

// Pallete button colour sequence
unsigned int colors[10] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_BLACK, TFT_CYAN, TFT_YELLOW, TFT_WHITE, TFT_MAGENTA, TFT_BLACK, TFT_BLACK};

void setup()
{
  //Serial.begin(38400);

  tft.init();

  // Set the TFT and touch screen to landscape orientation
  tft.setRotation(1);
  touch.setRotation(1);

  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  
  //This is the calibration line produced by the TFT_Touch_Calibrate sketch
  touch.setCal(317, 3934, 157, 3752, 240, 320);

  //Draw the pallet
  for (int i = 0; i < 10; i++)
  {
    tft.fillRect(i * 32, 0, 32, ColorPaletteHigh, colors[i]);
  }

  //Draw the clear screen button
  tft.setCursor(264, 7, 2); // x,y,font
  tft.setTextColor(TFT_WHITE);
  tft.print("Clear");
  tft.drawRect(0, 0, 319, 30, TFT_WHITE);

  // Plot the current colour in the screen clear box
  tft.fillRect(300, 9, 12, 12, color);
}

/* Main program */
void loop()
{
  int X_RawData;
  int Y_RawData;
  int X_Coord;
  int Y_Coord;

  // Check if the touch screen is currently pressed
  // Raw and coordinate values are stored within library at this instant
  // for later retrieval by GetRaw and GetCoord functions.

  if (touch.Pressed()) // Note this function updates coordinates stored within library variables
  {
    // Read the current X and Y axis as co-ordinates at the last touch time
    // The values were captured when Pressed() was called!
    X_Coord = touch.X();
    Y_Coord = touch.Y();

    // Detect  paint brush color change
    if (Y_Coord < ColorPaletteHigh + 2)
    {
      if (X_Coord / 32 > 7)
      {
        // Clear the screen to current colour
        tft.fillRect(0, 30, 399, 239, color);
      }
      else
      {
        color = colors[X_Coord / 32];
        // Update the cuurent colour in the clear box
        tft.fillRect(300, 9, 12, 12, color);
      }
    }
    else
    {
      tft.fillCircle(X_Coord, Y_Coord, 2, color);
    }
  }
}

