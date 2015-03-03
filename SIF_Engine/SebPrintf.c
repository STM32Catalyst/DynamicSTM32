
#include "sebEngine.h"
#include <stdarg.h>	// this is for the printf function
#include "SebPrintf.h"

// Here is the generic printf
const u8 u4ToHexChar[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
// Low to UpperCase ASCI convertion



#define PutChar(a)         ((u32(*)(u32, u32))T->fnPutChar)(T->ctPutChar, (a));//      LCD_cog_GOTO_Next_Line();
#define PutCharHex(a)      PutChar(u4ToHexChar[(a) & 0xF]);
#define PutCharDec(a)      PutChar('0'+((a)%10));

u32 SebPrintf(PrintfHk_t* T, const char *str,...)
{
  u32 arg, arg1;
  s8 k;	
  u32 pow;
  va_list ap;
  va_start(ap, str);
  
  if(T->fnPutChar==0) while(1); // error defining this!
 
  while (*str) 
  {  
    if (*str == 92) { // backslash (\)
      str++;
      if (*str == 'n') {
        //LCD_cog_Writedata('\n'); // does not go to the next line in hyper terminal. Adding 0x0A/0D or 0D0A will not help.
        PutChar(0x0A);//LCD_cog_GOTO_Next_Line();
      }else
      if(*str == 't') {
        // tabulation, goes to middle of the screen
        PutChar(0x09);//LCD_cog_GOTO_HorizontalCenter();
      }else{
        LCD_cog_Writedata_to_RAM(92); // backslash (\)
        LCD_cog_Writedata_to_RAM(*str);
      }  
    }else{ /* else 1 */
    if (*str != '%') {
      PutChar(*str);           
    }else{ /* else 2 */
      pow = 1;
      str++;  
      switch (*str) {
      case '%':
        PutChar('%');
        break; // '%%' = '%' !
      //=====-----> 1234 => %d => "1234"
      case 'd': /* decimal */
        arg = va_arg(ap, u32);
         
        if (arg & 0x80000000) { /* negative */
           PutChar('-');
           arg = (u32) (- ((s32) arg));
        }     
        while ((u32)(arg/(pow*10)))
        pow*=10;
        do{  
           PutCharDec(arg/pow);
           pow/=10;
        } while (pow >= 1);  
     break;	
     //=====-----> 1234 => %D => "123.4"
     case 'D': /* decimal, DECI 0.1 (/10) */
        arg = va_arg(ap, u32);
         
        if (arg & 0x80000000) { /* negative */
           PutChar('-');
           arg = (u32) (- ((s32) arg));
        }     
        while ((u32)(arg/(pow*10)))
          pow*=10;
     
        pow = max2(10,pow);
        do{ 
           PutCharDec(arg/pow);
           if(pow==10)
             PutChar('.');
           pow/=10;
        } while (pow >= 1);  
     break;	
     //=====-----> 1234 => %C => "12.34"
      case 'C': /* decimal, DECI 0.01 (/100) */
        arg = va_arg(ap, u32);
         
        if (arg & 0x80000000) { /* negative */
          PutChar('-');
          arg = (u32) (- ((s32) arg));
        }     
        while ((u32)(arg/(pow*10)))
          pow*=10;
     
        pow = max2(100,pow);
        do{ 
          PutCharDec(arg/pow);
          if(pow==100)
            PutChar('.');
          pow/=10;
        } while (pow >= 1);  
     break;	
     //=====-----> 1234 => %M => "1.234"
     case 'M': /* decimal, MILI 0.001 (/1000) */
       arg = va_arg(ap, u32);
        
       if (arg & 0x80000000) { /* negative */
          PutChar('-');
          arg = (u32) (- ((s32) arg));
       }     

       while ((u32)(arg/(pow*10)))
         pow*=10;
    
       pow = max2(1000,pow);
       do{ 
          PutCharDec(arg/pow);
          if(pow==1000)
            PutChar('.');
          pow/=10;
       } while (pow >= 1);  
      break;           
     //=====-----> 65 => %c => 'A'
     case 'c': /* char */
        arg = va_arg(ap, u32);				 
        PutChar(arg);
     break;
     // we also need to add binary showing! TODO
     //=====-----> 1024 => %X => "400"
     case 'x': /* Hexadecimal */
     case 'X':
        arg = va_arg(ap, u32);
        arg1 = arg;
        arg1 = ((arg >> 4) & 0x0000000F);
        PutCharHex(arg1);
        arg1 = (arg & 0x0000000F);
        PutCharHex(arg1);
     break;
     //=====-----> 1024 => %2X => "4000"
     case '2':
       str++;
       if ((*str == 'x') || (*str == 'X')) {
         arg = va_arg(ap, u32);					 
         arg1 = arg;
         for (k=12; k>=0 ;k-=4) {
           arg1 = ((arg >> k) & 0x0000000F);
           PutCharHex(arg1);
          }
        }
      break;           
     //=====-----> 1024 => %2X => "00004000"
     case '4':
        str++;
        if ((*str == 'x') || (*str == 'X')) {
          arg = va_arg(ap, u32); // u32 original
          arg1 = arg;
                          
          for (k=28; k>=0 ;k-=4) {
            arg1 = ((arg >> k) & 0x0000000F);
            PutCharHex(arg1);
          }
        }
      break;                 
      
    default:
      break ;
    }/* end switch */
   }/* end else 2*/
  }/* end else 1 */
  str++;   
  }/* end while() */

  va_end(ap); 
  return 0;
}/* end SERIAL_Printf() */




u32 u8_To_u32_BitFieldFormat(u8 byte)
{
	u32 answer = 0;
	
	if(byte&0x01)
		answer |= 0x00000001;

	if(byte&0x02)
		answer |= 0x00000010;

	if(byte&0x04)
		answer |= 0x00000100;

	if(byte&0x08)
		answer |= 0x00001000;

	if(byte&0x10)
		answer |= 0x00010000;

	if(byte&0x20)
		answer |= 0x00100000;

	if(byte&0x40)
		answer |= 0x01000000;

	if(byte&0x80)
		answer |= 0x10000000;

	return answer;
}
