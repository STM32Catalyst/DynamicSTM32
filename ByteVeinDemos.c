#include "SebEngine.h"

static ByteVein_t BV; // for test program
static u8 BVR[512]; // the assigned buffer for this byte strand


void BV_Test(void) {
  
  u32 n;
  NewBV(&BV, (u32)BVR, (s32)sizeof(BVR));
    
  for(n=0;n<32;n++)
    AddToBV(&BV, n); 

  for(n=0;n<32;n++)
    ClipBV_Down(&BV);
  
  for(n=0;n<32;n++)
    AddToBV(&BV, 100 + n);
  
  for(n=0;n<32;n++) 
    ClipBV_Down(&BV);

  for(n=0;n<32;n++) {
    BV.In = 200 + n; GlueBV_Down(&BV);
  };
  
  for(n=0;n<32;n++) 
    ClipBV_Up(&BV);
  
  while(1);
}

