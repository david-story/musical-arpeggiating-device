// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef lcd
#define	lcd

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    void lcd_init();
    void lcdcmd(char cmdout);
    void lcddata(char dataout);
    void screen_setup();

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

