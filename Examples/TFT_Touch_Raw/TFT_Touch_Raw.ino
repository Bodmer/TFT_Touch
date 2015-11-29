/* FILE:    TFT_Touch_Example1
   DATE:    10/06/14
   VERSION: 0.1
   ORIGINAL AUTHOR:  Andrew Davies

This is an example of how to use the TFT_Touch library. It does not display
anything on the screen!

Open up the Serial Monitor Window from the IDE and set to 38400 baud to see the
information from screen contacts

By using a stylus at the screen extremes or touch zone limits this sketch can be
used to check tha calibration values.

IMPORTANT: The 2.4" screen orientation used for calibration is with the SD Card slot on the left!

The library provides the ability to read the current status of the touch screen.
It does not provide any graphics drawing functions for controlling the TFT screen itself
it is intended for providing touch screen support to any graphics sketches and libraries.
However the library will work standalone as this example sketch will demonstrate.

The example will read the current state of the touch screen and output it to the serial
port as raw X and Y coordinates, as the current position in pixels, and as a zone number.

No information will be displayed on the TFT screen itself whilst this example is running.

You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.
This software may not be used directly for the purpose of selling products that
directly compete with Hobby Components Ltd's own range of products.

THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR LACK OF NEGLIGENCE.
HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR ANY DAMAGES,
INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY
REASON WHATSOEVER.

Adapted by Rowboteer 13/5/15
*/

// These are the pins used to interface between the 2046 touch controller and Arduino Mega
#define DOUT A0  /* Data out pin of touch screen */
#define DIN  A1  /* Data in pin of touch screen */
#define DCS  A2  /* Chip select pin of touch screen */
#define DCLK A3  /* Clock pin of touch screen */

// These are the default min and maximum values, set to 0 and 4095 to test the screen
#define XMIN 0
#define XMAX 4095
#define YMIN 0
#define YMAX 4095

// This is the screen size for the raw to coordinate transformation
// width and height specified for portrait orientation
#define XRES 240 /* Default screen resulution for X axis */
#define YRES 320 /* Default screen resulution for Y axis */

#include <TFT_Touch.h>

/* Create an instance of the touch screen library */
TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);

void setup()
{
  Serial.begin(38400);
  touch.setCal(XMIN, XMAX, YMIN, YMAX, XRES, YRES); // Raw xmin, xmax, ymin, ymax, width, height
  touch.setRotation(1);
}

/* Main program */
void loop()
{
  unsigned int X_Raw;
  unsigned int Y_Raw;

  /* Check if the touch screen is currently pressed*/
  // Raw and coordinate values are stored within library at this instant
  // for later retrieval by GetRaw and GetCoord functions.
  // This avoids getting duff values returned
  if (touch.Pressed()) // Note this function updates coordinates stored within library variables
  {
    /* Read the current X and Y axis as raw co-ordinates at the last touch time*/
    // The values returned were captured when Pressed() was called!
    
    X_Raw = touch.RawX();
    Y_Raw = touch.RawY();

    /* Output the results to the serial port */
    Serial.print("Raw x,y = ");
    Serial.print(X_Raw);
    Serial.print(",");
    Serial.println(Y_Raw);
    delay(10);
  }
  delay(10);
}

