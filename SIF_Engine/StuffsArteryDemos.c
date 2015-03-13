
#include "SebEngine.h"

// reading the strand content: The strand does not kill the bits when cut!

static StuffsArtery_t SA; // for test program
static u8 SAR[512]; // the assigned buffer for this byte strand


void SA_Test(void) {
  
  u32 n;
  NewSA(&SA, (u32)SAR, (s32)sizeof(SAR));
    
  for(n=0;n<32;n++) { 
    SA.In = n; GlueSA_Up(&SA); 
  };

  for(n=0;n<32;n++)
    ClipSA_Down(&SA);
  
  for(n=0;n<32;n++) { 
    SA.In = 100 + n; GlueSA_Up(&SA);
  };
  
  for(n=0;n<32;n++) 
    ClipSA_Down(&SA);

  for(n=0;n<32;n++) {
    SA.In = 200 + n; GlueSA_Down(&SA);
  };
  
  for(n=0;n<32;n++) 
    ClipSA_Up(&SA);
  
  while(1);
}




