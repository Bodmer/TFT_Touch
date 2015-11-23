/*

Library for 2046 controller based TFT touch screen.

Significantly modified by Rowboteer 22/11/15
Renamed TFT_Touch

See original header text at end of file
*/


#include "Arduino.h"
#include "TFT_Touch.h"

/* Constructor to initialise the GPIO */
TFT_Touch::TFT_Touch(byte CS_Pin, byte Clk_Pin, byte Din_Pin, byte Dout_Pin)
{
  _CS = CS_Pin;
  _Clk = Clk_Pin;
  _Din = Din_Pin;
  _Dout = Dout_Pin;

  _xmin = _XMIN;
  _xmax = _XMAX;
  _ymin = _YMIN;
  _ymax = _YMAX;
  _xres = _XRES;
  _yres = _YRES;

  setRotation(_ROTATION);

  pinMode(_CS, OUTPUT); 
  pinMode(_Clk, OUTPUT); 
  pinMode(_Din, OUTPUT); 
  pinMode(_Dout, INPUT); 
  
  digitalWrite(_CS, HIGH);  
  digitalWrite(_Clk, LOW);
  digitalWrite(_Din, LOW);
}

/* Detects if the touch screen is currently pressed. Returns TRUE if pressed */
boolean TFT_Touch::Pressed(void)
{
  // Get the raw contact coordinates
  _xraw = _ReadAxis(0);
  _yraw = _ReadAxis(1);

  // Now double check the touch is still near the initial contact point
  // This helps to debounce the touch contact
  // We are working with signed integers intentionally
  delay(1);
  if (abs(_xraw - _ReadAxis(0)) > _RAWERR) return false;
  if (abs(_yraw - _ReadAxis(1)) > _RAWERR) return false;
  delay(1);
  if (abs(_xraw - _ReadAxis(0)) > _RAWERR) return false;
  if (abs(_yraw - _ReadAxis(1)) > _RAWERR) return false;

  // Check values are in calibration range
  if((_yraw > _ymin && _yraw < _ymax) && (_xraw > _xmin && _xraw < _xmax))
  {
    if (!_axis){
      _ycoord = constrain(map(_xraw, _xmin, _xmax, 0, _xres), 0, _xres);
      _xcoord = constrain(map(_yraw, _ymin, _ymax, 0, _yres), 0, _yres);
      if(_xflip) _xcoord = _yres - _xcoord;
      if(_yflip) _ycoord = _xres - _ycoord;
    }
    else {
      _xcoord = constrain(map(_xraw, _xmin, _xmax, 0, _xres), 0, _xres);
      _ycoord = constrain(map(_yraw, _ymin, _ymax, 0, _yres), 0, _yres);
      if(_xflip) _xcoord = _xres - _xcoord;
      if(_yflip) _ycoord = _yres - _ycoord;
    }

    return true; 
  }else
  {
    return false;
  }
}

/* Reads one of the axis (XAXIS or YAXIS) raw coordinates. */
int16_t TFT_Touch::_ReadAxis(boolean Axis)
{
  int Data;
  
  digitalWrite(_CS, LOW);
  if (Axis)
  {
    _OutputData(0x90);   
  }else
  {
    _OutputData(0xD0);
  }

  digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  Data = _ReadData();
  digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  digitalWrite(_CS, HIGH); 
  digitalWrite(_Din, LOW);
  
  return Data;
}

/* Reads the raw data from the touch screen */
uint16_t TFT_Touch::_ReadData(void)
{
  byte index;
  int Data;
  Data = 0;
  for (index = 12; index > 0; index--)
  {
    Data += digitalRead(_Dout) << (index-1);
    digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  }
  return Data;
}  

/* Writes to the touch screen's configuration register */
void TFT_Touch::_OutputData(byte Data)
{
  byte index;
  
  for (index = 8; index > 0; index--)
  {
    digitalWrite(_Din, (Data >> (index -1)) & 1);
    digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  }
}

/* Read the current position from the touch screen and return it as a pixel position. */
uint16_t TFT_Touch::ReadRawX(void)
{
  return _ReadAxis(_axis);;
}

/* Read the current position from the touch screen and return it as a pixel position. */
uint16_t TFT_Touch::ReadRawY(void)
{
  return _ReadAxis(!_axis);;
}

/* Read the last position from the touch screen and return it as a pixel position. */
uint16_t TFT_Touch::RawX(void)
{
  return _xraw;
}

/* Read the last position from the touch screen and return it as a pixel position. */
uint16_t TFT_Touch::RawY(void)
{
  return _yraw;
}

/* Read the last position from the touch screen and return it as a pixel position. */
uint16_t TFT_Touch::X(void)
{
  return _xcoord;
}

/* Read the last position from the touch screen and return it as a pixel position. */
uint16_t TFT_Touch::Y(void)
{
  return _ycoord;
}

/* Read the last zone from the touch screen and return it as a pixel position. */
uint32_t TFT_Touch::Zone(void)
{
  return _xcoord + (uint32_t)_ycoord *_xres;
}

/* Set the screen resolution in pixels. */
void TFT_Touch::setResolution(uint16_t xres, uint16_t yres)
{
  _xres = xres;
  _yres = yres;
}

/* Set the screen calibration values */
void TFT_Touch::setCal(uint16_t xmin, uint16_t xmax, uint16_t ymin, uint16_t ymax, uint16_t xres, uint16_t yres)
{
  _xmin = xmin;
  _xmax = xmax;
  _ymin = ymin;
  _ymax = ymax;
  _xres = xres;
  _yres = yres;
}

void TFT_Touch::setRotation(byte rotation)
{
  switch(rotation) {
    case 0:
      _axis = 1;
      _xflip = !_XFLIP;
      _yflip = _YFLIP;
      break;
    case 1:
      _axis = 0;
      _xflip = _XFLIP;
      _yflip = _YFLIP;
      break;
    case 2:
      _axis = 1;
      _xflip = _XFLIP;
      _yflip = !_YFLIP;
      break;
    case 3:
      _axis = 0;
      _xflip = !_XFLIP;
      _yflip = !_YFLIP;
      break;
  }
}


// Original header

/* FILE:    HCTFT_Touch.h
   DATE:    10/06/14
   VERSION: 0.1
   AUTHOR:  Andrew Davies

Library for 2046 controller based TFT touch screen.

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
*/
