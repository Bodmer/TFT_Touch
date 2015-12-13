/*
This is a sketch to calibrate the screem and list the values to use in the
setCal() function, or to set as defaults in the Calibratio.h file within
the TFT_Touch library.

The calibration report is sent to the Serial port at 38400 baud.

Designed for use with the TFT_ILI9341 library
https://github.com/Bodmer/TFT_ILI9341

Uses font 2 only.

Created by Rowboteer for the ILI9341 3.4" 320 x 240 TFT touch display: 22/11/15
*/

// Call up the TFT driver library
#include <TFT_ILI9341.h> // Hardware-specific TFT library
#include <SPI.h>

// Call up touch screen library
#include <TFT_Touch.h>

// Invoke custom TFT driver library
TFT_ILI9341 tft = TFT_ILI9341();       // Invoke custom library

// These are the pins I used to interface between the 2046 touch controller and Arduino Mega
// they can be changed to other digital pins

#define DOUT A0  /* Data out pin (T_DO) of touch screen */
#define DIN  A2  /* Data in pin (T_DIN) of touch screen */
#define DCS  9  /* Chip select pin (T_CS) of touch screen */
#define DCLK 8  /* Clock pin (T_CLK) of touch screen */
//                  The touch screen interrupt request pin (T_IRQ) is not used

/* Create an instance of the touch screen library */
TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);

// These are the default min and maximum values,
// they are set to 0 and 4095 to calibrate the screen
#define XMIN 0    /* Default raw X axis measurement for Left of visible screen */
#define XMAX 4095 /* Default raw X axis measurement for Right of visible screen */
#define YMIN 0    /* Default raw Y axis measurement for bottom of visible screen */
#define YMAX 4095 /* Default raw Y axis measurement for top of visible screen */

// This is the screen size for the raw to coordinate transformation
// width and height specified for LANDSCAPE orientation
#define HRES 320 /* Default screen resulution for horizontal axis */
#define VRES 240 /* Default screen resulution for vertical axis */

int X_Raw = 0, Y_Raw = 0;

void setup()
{
  // Messages are sent to the serial port
  Serial.begin(38400);

  // Initialise the TFT
  tft.init();

  // Set the TFT screen to landscape orientation
  tft.setRotation(1);

  tft.setTextDatum(TC_DATUM);  // Set text plotting reference datum to Top Centre (TC)
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set text to white on black

  Serial.println("TFT_Touch Calibration, follow TFT screen prompts..");
}

/* Main program */
void loop()
{
  int xlo, ylo;
  int xhi, yhi;
  int xlo2, yhi2;
  
  // Reset the min and max calibration values
  touch.setCal(XMIN, XMAX, YMIN, YMAX, VRES, HRES);
  
  // Set TFT the screen to landscape orientation
  tft.setRotation(1);
  
  // Keep the touch screen orientation as 0 ***for calibration only***
  // normally the tft and touch rotations would be the same
  touch.setRotation(0);

  // Clear the screen
  tft.fillScreen(TFT_BLACK);

  // Show the screen prompt
  drawPrompt();

  drawCross(30, 30, TFT_RED);
  while (!touch.Pressed());
  delay(100);
  
  getCoord(); // This function assigns values to X_Raw and Y_Raw

  drawCross(30, 30, TFT_BLACK);
  Serial.print("First point : Coord x,y = ");
  Serial.print(X_Raw);
  Serial.print(",");
  Serial.println(Y_Raw);

  xlo = X_Raw;
  ylo = Y_Raw;

  drawCross(160, 120, TFT_RED);
  delay(10);
  
  while (getCoord()); // This waits for the centre area to be touched
  
  drawCross(160, 120, TFT_BLACK);
  Serial.print("Second point : Coord x,y = ");
  Serial.print(X_Raw);
  Serial.print(",");
  Serial.println(Y_Raw);

  drawCross(290, 210, TFT_RED);
  
  while (!getCoord()); // This waits until the centre area is no longer pressed
  delay(10);           // Wait a little for touch bounces to stop after release
  
  getCoord();
  drawCross(290, 210, TFT_BLACK);
  Serial.print("Third point : Coord x,y = ");
  Serial.print(X_Raw);
  Serial.print(",");
  Serial.println(Y_Raw);

  xhi = X_Raw;
  yhi = Y_Raw;

  drawCross(160, 120, TFT_RED);
  delay(10);
  
  while (getCoord()); // This waits for the centre area to be touched
  
  drawCross(160, 120, TFT_BLACK);
  Serial.print("Fourth point : Coord x,y = ");
  Serial.print(X_Raw);
  Serial.print(",");
  Serial.println(Y_Raw);

  drawCross(30, 210, TFT_RED);
  
  while (!getCoord()); // This waits until the centre area is no longer pressed
  delay(10);           // Wait a little for touch bounces to stop after release
  
  getCoord();
  drawCross(30, 210, TFT_BLACK);
  Serial.print("Fifth point : Coord x,y = ");
  Serial.print(X_Raw);
  Serial.print(",");
  Serial.println(Y_Raw);

  xlo2 = X_Raw;
  yhi2 = Y_Raw;

  int temp;
  if (xhi < xlo) {
    temp = xlo;
    xlo = xhi;
    xhi = temp;
  }
  if (yhi < ylo) {
    temp = ylo;
    ylo = yhi;
    yhi = temp;
  }

  int xmin = xlo - (xhi - xlo) * 3 / 18;
  int xmax = xhi + (xhi - xlo) * 3 / 18;

  int ymin = ylo - (yhi - ylo) * 3 / 26;
  int ymax = yhi + (yhi - ylo) * 3 / 26;

  Serial.println();
  Serial.println("These are the values you can use in Calibration.h");
  Serial.print("#define _XMIN ");
  Serial.println(xmin);
  Serial.print("#define _YMIN ");
  Serial.println(ymin);

  Serial.print("#define _XMAX ");
  Serial.println(xmax);
  Serial.print("#define _YMAX ");
  Serial.println(ymax);

  Serial.println();
  Serial.println("  //This is the calibration line to use in your sketch");
  Serial.println("  //you can copy and paste into your sketch setup()");
  Serial.print("  touch.setCal(");
  Serial.print(xmin); Serial.print(", ");
  Serial.print(xmax); Serial.print(", ");
  Serial.print(ymin); Serial.print(", ");
  Serial.print(ymax); Serial.print(", ");
  Serial.print(240); Serial.print(", ");
  Serial.print(320);
  Serial.println(");");

  Serial.println();
  Serial.println("Test the touch screen, green crosses appear at the touch coordinates!");
  Serial.println("Send any character from the serial monitor window to restart calibration");

  // These are the calibration settings the sketch has calculated to try out!
  touch.setCal(xmin, xmax, ymin, ymax, 240, 320); // Raw xmin, xmax, ymin, ymax, width, height


  // Keep TFT and Touch rotations the same, try values 0 to 3
  // Start with the current orientation
  // Receiving any serial character moves on to next orientation test
  tft.setRotation(1);
  touch.setRotation(1);
  test();

  tft.setRotation(2);
  touch.setRotation(2);
  test();

  tft.setRotation(3);
  touch.setRotation(3);
  test();

  tft.setRotation(0);
  touch.setRotation(0);
  test();
}

void test(void)
{
  tft.fillScreen(TFT_BLACK);

  drawCross(30, 30, TFT_WHITE);

  drawCross(tft.width() - 30, tft.height() - 30, TFT_WHITE);

  int centre = tft.width()/2; // Get and work out x coord of screen centre

  String text;
  text+= "Screen rotation = ";
  text+= tft.getRotation();
  char buffer[30];
  text.toCharArray(buffer,30);
  
  tft.drawString(buffer, centre, 50, 2);

  tft.drawString("Touch anywhere on screen", centre, 70, 2);
  tft.drawString("to test settings", centre, 90, 2);

  tft.drawString("Send a character from the", centre, 120, 2);
  tft.drawString("IDE Serial Monitor to", centre, 140, 2);
  tft.drawString("continue!", centre, 160, 2);

  while (Serial.available()) Serial.read(); // Empty the serial buffer before we start
  
  while (!Serial.available()) {
    if (touch.Pressed()) // Note this function updates coordinates stored within library variables
    {
      /* Read the current X and Y axis as co-ordinates at the last touch time*/
      // The values returned were captured when Pressed() was called!
      int X_Coord = touch.X();
      int Y_Coord = touch.Y();

      drawCross(X_Coord, Y_Coord, TFT_GREEN);

      //delay(20);
      tft.setCursor(centre, 0,  2);
      tft.print("X = ");tft.print(X_Coord);tft.print("   ");
      tft.setCursor(centre, 20,  2);
      tft.print("Y = ");tft.print(Y_Coord);tft.print("   ");

    }
  }
}

void drawPrompt(void)
{
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  int centre = tft.width()/2; // Get and work out x coord of screen centre

  tft.drawString("CALIBRATION", centre, 20, 2);

  tft.drawString("Touch the red cross accurately", centre, 61, 2);
  tft.drawString("( using a cocktail stick works well! )", centre, 81, 2);
}

void drawCross(int x, int y, unsigned int color)
{
  tft.drawLine(x - 5, y, x + 5, y, color);
  tft.drawLine(x, y - 5, x, y + 5, color);
}

bool getCoord()
{
  bool Xwait = 1, Ywait = 1;
  int X_Temp1 = 9999, Y_Temp1 = 9999;
  int X_Temp2 = -1, Y_Temp2 = -1;
  X_Raw = -1;
  Y_Raw = -1;

  while (Xwait || Ywait) {
    if (touch.Pressed()) // Note this function updates coordinates stored within library variables
    {
      /* Read the current X and Y axis as co-ordinates at the last touch time*/
      // The values returned were captured when Pressed() was called!
      X_Temp1 = touch.RawX();
      Y_Temp1 = touch.RawY();
    }
    delay(5);
    if (touch.Pressed()) // Note this function updates coordinates stored within library variables
    {
      /* Read the current X and Y axis as co-ordinates at the last touch time*/
      // The values returned were captured when Pressed() was called!
      X_Temp2 = touch.RawX();
      Y_Temp2 = touch.RawY();
    }

    #define RAW_ERROR 10
    
    if ( (abs(X_Temp1 - X_Temp2) < RAW_ERROR) && Xwait ) {
      X_Raw = (X_Temp1 + X_Temp2) / 2;
      Xwait = 0;
    }
    if ( (abs(Y_Temp1 - Y_Temp2) < RAW_ERROR) && Ywait ) {
      Y_Raw = (Y_Temp1 + Y_Temp2) / 2;
      Ywait = 0;
    }
  }

  // Check if press is near middle third of screen
  if ((X_Raw > 1365) && (X_Raw < 2731) && (Y_Raw > 1365) && (Y_Raw < 2371)) return 0;
  
  // otherwise it is near edge for calibration points
  else return 1;
}

