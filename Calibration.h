#ifndef _Calibration_H
#define _Calibration_H

/* CHANGE THESE FOR YOUR SCREEN TO SET DEFAULT VALUES*/
// If these are set correctly the SetCal() function does not need to be called in sketch
#define _XMIN 0  /* Default raw X axis measurement for Left of visible screen */
#define _XMAX 4095 /* Default raw X axis measurement for Right of visible screen */
#define _YMIN 0  /* Default raw Y axis measurement for Top of visible screen */
#define _YMAX 4095 /* Default raw Y axis measurement for Bottom of visible screen */

// These are the resolution figures when the TFT is in "portrait" orientation
// If these are set correctly the SetCal() function does not need to be called in sketch
#define _XRES 320 /* Default screen resolution for width */
#define _YRES 240 /* Default screen resolution for height */

#define _XYSWAP 0       /* Swap x and y coordinates */
#define _XFLIP  0       /* Set to 0 for normal or 1 to flip x coordinates */
#define _YFLIP  0       /* Set to 0 for normal or 1 to flip y coordinates */

#define _ROTATION 1    /* Set default screen orientation 0 to 3 */

// When a touch is detected the library samples x and y twice
// The two x and two y values must be within RAWERR of each other
// otherwise the touch is rejected as noisy.
// Define +/- raw coord error limit for a press
// A value of about 10 seems good
#define _RAWERR 10

#endif