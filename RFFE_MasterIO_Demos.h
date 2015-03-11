
#ifndef _RFFE_MASTER_IO_DEMOS_H_
#define _RFFE_MASTER_IO_DEMOS_H_

// MIPI RFFE Serial Bus, up to 26MHz (here more like 1 MHz as it is implemented with GPIOs, blocking mode)
// In the example, we use ST's HVDAC code STHVDAC-253M which provides triple 7 bit 20V DAC from 3-5V supply.
// This DAC is primarily used to drive a voltaged driven bias variable capactitor (PARASCAN)
void RFFE_Test(void);

#endif
