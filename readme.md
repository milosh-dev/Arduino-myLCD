The main enhancements are:

 - built-in printf support (does not require memory expensive String library). 
 - support uppercase and lowercase command for strings stored in PROGMEM.
 - PROGMEM support for createChar bitmaps (saves memory)
 - static PROGMEM string array support; 
 - center command to print text into the center of the screen
 - one line command to display the text at specific position display(0,1,F("Hello Word"));
 - similarly one line command for displayf(pos, line, text, ...); 
 - scroll PROGMEM strings in screen (works also on subsection of LCD screen) (in order to enable, refresh screen every 200 milliseconds with the scroll(strng array) command. 

All LiquidCrystal commands are working as well, as this library extends standard library.  
