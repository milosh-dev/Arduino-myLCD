/**
 * @file mylcd.h
 *
 * @date 14.06.2014
 * @author Raoul Lättemäe
 * @extends LiquidCrystal
 */

#ifndef MYLCD_H_
#define MYLCD_H_
#include "Arduino.h"
//add your includes for the project null here
#include <LiquidCrystal.h>
#include <avr/pgmspace.h>
#include "math.h"

//add your function definitions for the project null here
/*************************************//**
 * myLCD Class.
 * Class myLCD extends LiquidCrystal class.
 *
 * @example usage
 * @brief Here we show the basic usage of myLCD class
 * The myLCD class extends LiqyidCrystal class
 * @code{.cpp}
 *
 * 	#include "myLCD.h"
 * 	myLCD lcd(7,8,6,5,4,3); // define lcd
 *
 * 	// define PROGMEM strings
 * 	static const prog_char VESSEL_S[] PROGMEM = "Masina";
 * 	static const prog_char CONFIGURATION[] PROGMEM = "seadistamine";
 *
 * 	void setup(){
 * 		lcd.begin(16, 2);
 * 		lcd.display(0, 0, F("Hello"));
 * 		lcd.display(0, 1, "Word!"));
 * 		lcd.display(10, 1, 3.1415926,2);
 * 		delay(1000);
 * 		lcd.clear();
 * 		lcd.center(0, F("Hello"));
 * 		delay(1000);
 * 		lcd.clear();
 * 		lcd.display(0, 0, VESSEL_S);
 * 		lcd.display(0, 1, CONFIGURATION);
 * 		delay(1000);
 * 		lcd.clear();
 * 		lcd.display(0, 0, (upper)VESSEL_S); // converts PROGMEM text to UPPERCASE "MASINA"
 * 		lcd.display(0, 1, (lower)VESSEL_S); // converts PROGMEM text to lowercase "masina"
 * 		delay(1000);
 * 		lcd.clear();
 * 		lcd.setCursor(0,0);
 * 		lcd.printf(F("Nr %5.1f", 3.1415926); // displays floating number trough printf command
 * 		char ht = 'f';
 * 		char pp = 'c';
 * 		// more ellaborate printf command (displays from second pos in the second row)
 * 		lcd.displayf(1, 1, PSTR("%01i:%02i:%02i %c%c %*.*f"), 0, 2, 30, ht, pp, 4, 1, 16.4533));
 * 	}
 * @endcode
 *
 ***************************************/

/**
 * This is a trick to make uppercase, lowercase and First case text
 * from progmem.
 *
 * Usage (upper)text;
 * (lower)text;
 */

template <typename T, typename N> // N is used for tagging
struct strong_typedef {
		//! Default constructor does not initialize the value.
		strong_typedef() {
		}

		//! Construction from a fundamental value.
		strong_typedef(T value)
				: value(value) {
		}

		//! Implicit conversion back to the fundamental data type.
		inline operator T() const {
			return value;
		}

		//! The actual fundamental value.
		T value;
};

namespace types {
	struct progmemID {
	};
	struct upperID {
	};
	struct lowerID {
	};
}

typedef strong_typedef <const PROGMEM char*, types::progmemID> progmem;
typedef strong_typedef <const PROGMEM char*, types::upperID> upper;
typedef strong_typedef <const PROGMEM char*, types::lowerID> lower;

#define PAD_RIGHT 1
#define PAD_ZERO 2
#define SCROLLTIME 300

class myLCD: public LiquidCrystal {
	public:
		// construct
		/**
		 * Constructor
		 * @param rs
		 * @param enable
		 * @param d0
		 * @param d1
		 * @param d2
		 * @param d3
		 */
		myLCD(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2,
				uint8_t d3)
				: _numcols(0), LiquidCrystal(rs, enable, d0, d1, d2, d3) {
			scrollStart = millis();
		}
		;
		template <typename T>
		void display(byte pos, byte line, T text);
		/**
		 * @brief Extends LCD functionality in order to display number
		 * in specified position.
		 *
		 * @param pos Cursor Position
		 * @param line Cursor Line
		 * @param n Number to write
		 * @param digits Number of digits afrer comma
		 */
		template <typename T>
		void display(byte pos, byte line, T n, int digits);
		/**
		 * @brief  Extends LCD functionality in order to display number
		 * in specified position.
		 *
		 * @param pos
		 * @param line
		 * @param text
		 */
		template <typename T>
		void display(byte pos, byte line, int digits, T text);
		/**
		 * Displays text in the center of the screen
		 * @param line Cursor Line
		 * @param text Text to be displayed.
		 */
		void center(byte line, const __FlashStringHelper *text);
		void center(byte line, const char *text);
		void center(byte line, upper text);
		void center(byte line, lower text);
		void center(byte line, const String &text);

		void clean(byte col, byte row, byte rept);
		/**
		 * Extends usage
		 * @code lcd.begin(16,2);
		 *
		 * @param cols
		 * @param rows
		 * @param charsize
		 */
		void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS) {
			LiquidCrystal::begin(cols, rows, charsize);
			_numcols = cols;
		}
		/**
		 * Create custom character from PROGMEM
		 * @param location
		 * @param charDef
		 */
		void createChar(uint8_t location, const byte *charDef);

		/**
		 * Scrolls text in custom location
		 * @param col Curson Position
		 * @param row Cursor Line
		 * @param text PROGMEM text
		 * @param len scroll lenght
		 */
		void scroll(byte col, byte row, const PROGMEM char *text, byte len);
		void scroll(byte col, byte row, const PROGMEM char *text[], byte len);

		using Print::print;
		size_t print(const PROGMEM char *text);
		size_t print(const PROGMEM char* text[]);
		size_t print(upper text);
		size_t print(lower text);

		size_t printw(uint8_t width, const PROGMEM char *text, boolean padleft, const char symbol); //!< text
		size_t printw(uint8_t width, int value, boolean padleft, const char symbol); //!< integer
		size_t printw(uint8_t width, double value, uint8_t digits, boolean padleft, const char symbol); //!< float

//		size_t print(const PROGMEM char *text, uint8_t len);
//		size_t print(upper text, uint8_t len);
//		size_t print(lower text, uint8_t len);
//
//		size_t print(uint8_t len, const PROGMEM char *text);
//		size_t print(uint8_t len, upper text);
//		size_t print(uint8_t len, lower text);

		// printf library....

		/* assuming sizeof(void *) == sizeof(int) */
		size_t printf(const PROGMEM char *format, ...);
		size_t _printf(const char *format, va_list args);
		void displayf(byte col, byte row, const PROGMEM char *format, ...);

	private:
		//size_t _printf(const PROGMEM char **out, int *varg);
		void _clean(byte width, byte len, uint8_t value);
		uint8_t len(long value);
		uint8_t len(double value, byte digits);

		uint8_t _numcols;
		unsigned long scrollStart;
//		byte screen[32];
};

/**
 * @brief Displays text in specified position
 *
 * @param pos Cursor Position
 * @param line Cursor Line
 * @param text Text to be displayed
 */
template <typename T>
inline void myLCD::display(byte pos, byte line, T text) {
	setCursor(pos, line);
	print(text);
}

//template <typename T>
//inline void myLCD::display(byte pos, byte line, T n, int digits) {
//	setCursor(pos, line);
//	print(n, digits);
//}

template <typename T>
inline void myLCD::display(byte pos, byte line, int digits, T text) {
	setCursor(pos, line);
	print(digits, text);
}

inline void myLCD::center(byte line, const __FlashStringHelper *text) {
// alternative len function - testing and functioning:
//	const prog_char *p = reinterpret_cast <const prog_char *>(text);
//	size_t i = _numcols;
//	while (1) {
//		unsigned char c = pgm_read_byte(p++);
//		if (c == 0) break;
//		--i;
//	}
	int len = strlen_P((PGM_P) text);
	int pos = max((_numcols - len) / 2, 0);
	this->display(pos, line, text);
}

inline void myLCD::center(byte line, const char* text) {
	int len = strlen_P(text); // strlen if we are not using PROGMEM
	int pos = max((_numcols - len) / 2, 0);
	this->display(pos, line, text);
}

inline void myLCD::center(byte line, upper text) {
	int len = strlen_P(text); // strlen if we are not using PROGMEM
	int pos = max((_numcols - len) / 2, 0);
	this->display(pos, line, text);
}

inline void myLCD::center(byte line, lower text) {
	int len = strlen_P(text); // strlen if we are not using PROGMEM
	int pos = max((_numcols - len) / 2, 0);
	this->display(pos, line, text);
}

inline void myLCD::center(byte line, const String& text) {
	int len = text.length();
	int pos = max((_numcols - len) / 2, 0);
	this->display(pos, line, text);
}

template <typename T>
inline void myLCD::display(byte pos, byte line, T n, int digits) {
	setCursor(pos, line);
	print(n, digits);
}

/**
 * PROGMEM PRINT
 * @param text
 * @return length of printed text
 */
inline size_t myLCD::print(const char *text) {
	size_t n = 0;
	while (pgm_read_byte(text) != 0) {
		n += write(pgm_read_byte(text++));
	}
	return n;
}

/**
 * PROGMEM UPPERCASE PRINT
 * @param text
 * @return length of printed text
 */
inline size_t myLCD::print(upper text) {
	size_t n = 0;
	const char* txt = (const char*) text;
	while (pgm_read_byte(txt) != 0) {
		char t = (char) (pgm_read_byte(txt));
		if (t >= 'a' && t <= 'z') {
			t = t - 'a' + 'A';
		}
		n += write(t);
		txt++;
	}
	return n;
}
/**
 * PROGMEM LOWERCASE PRINT
 * @param text
 * @return length of printed text
 */
inline size_t myLCD::print(lower text) {
	size_t n = 0;
	const char* txt = (const char*) text;
	while (pgm_read_byte(txt) != 0) {
		char t = (char) (pgm_read_byte(txt));
		if (t >= 'A' && t <= 'Z') {
			t = t + 'a' - 'A';
		}
		n += write(t);
		txt++;
	}
	return n;
}

/**
 * Progmem char
 */
inline void myLCD::createChar(uint8_t location, const byte *charDef) {
	location &= 0x7; // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	for (byte i = 0; i < 8; i++) {
		write(pgm_read_byte(charDef++));
	}
}

/**
 * Cleans screen in the pos
 * @param col Cursor Position
 * @param row Cursor Line
 * @param rept Positions to clean
 */
inline void myLCD::clean(byte col, byte row, byte rept) {
	setCursor(col, row);
	for (byte i = 0; i < rept; ++i) {
		write(" ");
	}
}

inline void myLCD::scroll(byte col, byte row, const char* text, byte lenght) {

	if(millis() - scrollStart < SCROLLTIME) return;

	static char pos = 0; // this is static value i.e will retain its value after function has been run
	char len = strlen_P(text) - 1; // strlen if we are not using PROGMEM
	uint16_t readpos;

	if (pos > len) pos = 0; // constrain pos to the length of text;

//	setCursor (0,0);
//	static const prog_char RRR[] PROGMEM = "################";
//	print(RRR);
	setCursor(col, row);
//
//	for (byte i = 0; i < lenght - pos; i++){
//		write(" ");
//	}

	for (byte j = 0; j < lenght; j++) {
		readpos = (uint16_t) text + pos + j; // reading position
		if (pos + j > len) { // kustutab tagumise soga
			write(" ");
		} else {
			write(pgm_read_byte(readpos));
		}
	}

//	char j;
//
////	for (byte j = lenght; j > 0; j--) {
////		readpos = (uint16_t) text + pos + j; // reading position
////		if (pos + j > len) {
////			write(" ");
////		} else {
////			write(pgm_read_byte(readpos));
////		}
////	}
//
//	// Alternatiivne interpretatsioon
//	// ts�klite-ekraanide arv: pos = len + lenght
//
//	pos = 0; // algseade
//	if (pos >= len + lenght) pos = 0; // max ekraanide arv
//
//	for (byte i = 0; i < lenght; i++){
//	// joonista algt�hemikud ekraanile
//	if (pos + i < lenght){
//		write(" ");
//	}
//	// joonista tekst ekraanile
//	readpos = (uint16_t) text + pos + i; // reading position
//	if ()
//
//	// joonista tekst ekraanile
//	while ()
//
//
//		if (lenght - pos - i> 0){
//			write(" ");
//		} else if () {
//
//		}
//	}

	// move to next pos, so next time the function is called, the text will be scrolled
	pos++;
	scrollStart = millis();
}

/**
 * Progmem print with text width
 * prints multiple texts from PROGMEM
 */
//inline size_t myLCD::print(const char* text, uint8_t len) {
//	size_t n;
//	int clean = len - strlen_P(text);
//	n += print(text);
//	while (clean > 0) {
//		n += write(" ");
//		--clean;
//	}
//	return n;
//}
inline size_t myLCD::print(const char* text[]) {
	size_t n;
	byte nr = sizeof(text) / sizeof(char*);
	for (byte j = 0; j < nr; ++j) {
		if (j > 0) write(" ");
		n += print((char*) pgm_read_word(&(text[j])));
	}
	return n;
}

//inline size_t myLCD::print(upper text, uint8_t len) {
//	size_t n = 0;
//	int clean = len - strlen_P(text);
//	n += print(text);
//	while (clean > 0) {
//		n += write(" ");
//		--clean;
//	}
//	return n;
//}
inline void myLCD::scroll(byte col, byte row, const char* text[], byte len) {
	static byte pos = 0; //!> this is static value i.e will retain its value after function has been run
	byte nr = sizeof(text) / sizeof(char*); //!> number of strings in array

	setCursor(col, row);

	// Find a reading position
	boolean found = false;
	byte j = 0;
//	byte stringpos = 0;
	byte stringlen = 0;

	while (!found) {
		stringlen = strlen_P(text[j]);
		if (pos < stringlen) {
			stringlen = pos;
			found = true;
			++pos;
		} else {
			++j;
			if (j > nr) {
				pos = 0;
				j = 0;
			}
		}
	}
}

//inline size_t myLCD::print(lower text, uint8_t len) {
//	size_t n = 0;
//	int clean = len - strlen_P(text);
//	n += print(text);
//	while (clean > 0) {
//		n += write(" ");
//		--clean;
//	}
//	return n;
//}
//
//inline size_t myLCD::print(uint8_t len, const char* text) {
//	size_t n = 0;
//	int clean = len - strlen_P(text);
//	while (clean > 0) {
//		n += write(" ");
//		--clean;
//	}
//	n += print(text);
//	return n;
//}
//inline size_t myLCD::printf(const char *format, ...) {
//	int *varg = (int *) (&format);
//	return _printf(0, varg);
//}

/**
 * For use of printf see:
 * http://www.cplusplus.com/reference/cstdio/printf/
 *
 * A format specifier follows this prototype:
 * %[flags][width][.precision][length]specifier
 *
 * For example:
 *
 *
 */
inline size_t myLCD::printf(const char *data, ...){
	va_list args;
	va_start(args, data);
	_printf(data, args);
	va_end(args);
}
/*
inline size_t myLCD::printf(const char *data, ...) {
	int *varg = (int *) (&data);
	varg++;
	int width, pad, post_decimal, dec_width;
	int pc = 0;
	char t;
	char padchar = ' ';
	size_t len = 0;

	while (pgm_read_byte(data) != 0) { // read PROGMEM text
		t = (char) (pgm_read_byte(data++));
		if (t == '%') { // it's a flag

			width = 0;
			pad = 0;
			t = (char) (pgm_read_byte(data++));

			if (t == '%') goto out;
			if (t == '-') { // left justify within the given field width
				t = (char) (pgm_read_byte(data++));
				pad = PAD_RIGHT; // pad = 1
			}
			if (t == '+') { // Forces to precede the result with + or - sign even if the number is positive
				if (*varg > 0) write('+');
				if (*varg == 0) write(' ');
				t = (char) (pgm_read_byte(data++));
			}
			while (t == '0') { // PAD with zeros.
				t = (char) (pgm_read_byte(data++));
				padchar = '0';
			}

			// float support
			post_decimal = 0;
			while (t == '.' || (t >= '0' && t <= '9') || t == '*') {
				if (t == '.') { // this is float
					post_decimal = 1;
					dec_width = 0;
					t = (char) (pgm_read_byte(data++));

				} else if ((t >= '0' && t <= '9')) { // read number
					if (post_decimal) { // this is float numbers
						dec_width *= 10;
						dec_width += t - '0';
					} else { // this is width (before decimal points)
						width *= 10;
						width += t - '0';
					}
					t = (char) (pgm_read_byte(data++));

				} else if (t == (char)'*') { // width is in argument. For some reason it is not working as expected
					if (post_decimal) {
						dec_width = *varg++;
					} else {
						width = *varg++;
					}
					t = (char) (pgm_read_byte(data++));
				} else {
					break;
				}
			}

			// now the flag itself:
			len = 0;
			switch (t) {
				case 's': // s = string
					printw(width, (const char*) *varg++, pad, padchar);
					break;
				case 'd': // signed decimal integer
				case 'i': // signed decimal integer
				case 'u': // unsigned decimal integer
					printw(width, *varg++, pad, padchar); // pc += printi (out, *varg++, 10, 1, width, pad, 'a');
					break;
				case 'f': {
#ifdef USE_NEWLIB
					char *cptr = (char *) varg; //lint !e740 !e826  convert to double pointer
					uint caddr = (uint) cptr;
					if ((caddr & 0xF) != 0) {
						cptr += 4;
					}
					double *dblptr = (double *) cptr; //lint !e740 !e826  convert to double pointer
#else
					double *dblptr = (double *) varg; //lint !e740 !e826  convert to double pointer
#endif
					double dbl = *dblptr++;   //  increment double pointer
					double dbl1 = dbl;
					varg = (int *) dblptr; //lint !e740  copy updated pointer back to base pointer

					printw(width, dbl, dec_width, pad, padchar);
				}
					break;
				case 'c': // character
				default:
					if(!pad) while((width - 1) > 0) write(padchar);
					write(t);
					if(pad) while((width - 1) > 0) write(padchar);
					break;
			}

		} else { // its not flag: write the letter
			out: pc += write(t);
		}
	}
	return pc;
}
*/

/**
 * Printf in given position
 *
 * For use of printf see:
 * http://www.cplusplus.com/reference/cstdio/printf/
 *
 * A format specifier follows this prototype:
 * %[flags][width][.precision][length]specifier
 *
 * For example
 * @code
 * displayf(0, 0, PSTR("%5s %4.1f %*.*f"), PSTR("Tekst"), 5.3, 2, 1, 56.3);
 * @endcode
 *
 * @param col
 * @param row
 * @param format
 */
inline void myLCD::displayf(byte col, byte row, const char *format, ...) {
	setCursor(col, row);
	va_list args;
	va_start(args, format);
	_printf(format, args);
	va_end(args);
}

/**
 * Actual implementation of printf
 *
 * @param data
 * @param args
 * @return
 */
inline size_t myLCD::_printf(const char *data, va_list args){
	int *varg = (int *) (args);
//	varg++;
	int width, pad, post_decimal, dec_width;
	int pc = 0;
	char t;
	char padchar;
	size_t len = 0;
	boolean forcesign;

	while (pgm_read_byte(data) != 0) { // read PROGMEM text
		t = (char) (pgm_read_byte(data++));
		if (t == '%') { // it's a flag

			width = 0;
			pad = 0;
			padchar = ' ';
			forcesign = false;
			t = (char) (pgm_read_byte(data++));

			if (t == '%') goto out;
			if (t == '-') { // left justify within the given field width
				t = (char) (pgm_read_byte(data++));
				pad = PAD_RIGHT; // pad = 1
			}
			if (t == '+') { // Forces to precede the result with + or - sign even if the number is positive
				//! @todo We need global variable in order to implement this functionality in printw(...);
				forcesign = true;
//				if (*varg > 0) write('+');
//				if (*varg == 0) write(' ');
				t = (char) (pgm_read_byte(data++));
			}
			while (t == '0') { // PAD with zeros.
				t = (char) (pgm_read_byte(data++));
				padchar = '0';
			}

			// float support
			post_decimal = 0;
			while (t == '.' || (t >= '0' && t <= '9') || t == '*') {
				if (t == '.') { // this is float
					post_decimal = 1;
					dec_width = 0;
					t = (char) (pgm_read_byte(data++));

				} else if ((t >= '0' && t <= '9')) { // read number
					if (post_decimal) { // this is float numbers
						dec_width *= 10;
						dec_width += t - '0';
					} else { // this is width (before decimal points)
						width *= 10;
						width += t - '0';
					}
					t = (char) (pgm_read_byte(data++));

				} else if (t == (char)'*') { // width is in argument. For some reason it is not working as expected
					if (post_decimal) {
						dec_width = *varg++;
					} else {
						width = *varg++;
					}
					t = (char) (pgm_read_byte(data++));
				} else {
					break;
				}
			}

			// now the flag itself:
			len = 0;
			switch (t) {
				case 's': // s = string
					printw(width, (const char*) *varg++, pad, padchar);
					break;
				case 'd': // signed decimal integer
				case 'i': // signed decimal integer
				case 'u': // unsigned decimal integer
					printw(width, *varg++, pad, padchar); // pc += printi (out, *varg++, 10, 1, width, pad, 'a');
					break;
				case 'f': {
#ifdef USE_NEWLIB
					char *cptr = (char *) varg; //lint !e740 !e826  convert to double pointer
					uint caddr = (uint) cptr;
					if ((caddr & 0xF) != 0) {
						cptr += 4;
					}
					double *dblptr = (double *) cptr; //lint !e740 !e826  convert to double pointer
#else
					double *dblptr = (double *) varg; //lint !e740 !e826  convert to double pointer
#endif
					double dbl = *dblptr++;   //  increment double pointer
//					double dbl1 = dbl;
					varg = (int *) dblptr; //lint !e740  copy updated pointer back to base pointer

					printw(width, dbl, dec_width, pad, padchar);
				}
					break;
				case 'c': // character
				default:
					if(!pad) while((width - 1) > 0) write(padchar);
					write((char) *varg++);
					if(pad) while((width - 1) > 0) write(padchar);
					break;
			}

		} else { // its not flag: write the letter
			out: pc += write(t);
		}
	}
	return pc;
}

/**
 * Print text with predetermined width. Missing (padding) characters will be printed with symbol param.
 *
 * @param width of the print
 * @param text to be printed
 * @param padleft if true, then left align
 * @param symbol padding symbol
 * @return
 */
inline size_t myLCD::printw(uint8_t width, const char* text, boolean padleft =
		false, const char symbol = ' ') {
	size_t n;
	uint8_t clean = width - strlen_P(text);
	if (padleft) n += print(text);
	while (clean > 0) {
		n += write(symbol);
		--clean;
	}
	if (!padleft) n += print(text);
	return n;
}

/**
 * Calculate (long) integer length (for printing)
 *
 * @param integer value
 * @return length of integer
 */
inline uint8_t myLCD::len(long value) {
	uint8_t len = !value;
	if (value < 0) len++;

	while (value) {
		len++;
		value /= 10;
	}
	return len;
}

/**
 * Calculate double digit length (for printing)
 *
 * @param value
 * @param digits
 * @return length of double
 */
inline uint8_t myLCD::len(double value, byte digits) {
	uint8_t l = len((long)value);
	if(digits>0){
		++l; // for deicmal point
		l += digits; // for digits
	}
	return l;
}

/**
 * Print integer with predetermined width. Missing (padding) characters will be printed with symbol param.
 *
 * @param width of the print
 * @param value to be printed
 * @param padleft if true, then left align
 * @param symbol padding symbol
 * @return
 */
inline size_t myLCD::printw(uint8_t width, int value, boolean padleft = false,
		const char symbol = ' ') {
	size_t n;
	uint8_t l = len(value);
	uint8_t clean = width - l;
	if (padleft) n += print(value);
	while (clean > 0) {
		n += write(symbol);
		--clean;
	}
	if (!padleft) n += print(value);
	return n;
}

/**
 * Print double with predetermined width. Missing (padding) characters will be printed with symbol param.
 *
 * @param width of the print
 * @param value to be printed
 * @param digits after comma
 * @param padleft if true, then left align
 * @param symbol padding symbol
 * @return
 */
inline size_t myLCD::printw(uint8_t width, double value, uint8_t digits,
		boolean padleft = false, const char symbol = ' ') {
	size_t n;
	uint8_t l = len(value, digits);
	uint8_t clean = width - l;
	if (padleft) n += print(value, digits);
	while (clean > 0) {
		n += write(symbol);
		--clean;
	}
	if (!padleft) n += print(value, digits);
	return n;
}


inline void myLCD::_clean(byte width, byte len, uint8_t value) {
	while (width > len) {
		write(value);
		--width;
	}
}

//inline size_t myLCD::print(uint8_t len, upper text) {
//	size_t n = 0;
//	int clean = len - strlen_P(text);
//	while (clean > 0) {
//		n += write(" ");
//		--clean;
//	}
//	n += print(text);
//	return n;
//}
//
//inline size_t myLCD::print(uint8_t len, lower text) {
//	size_t n = 0;
//	int clean = len - strlen_P(text);
//	while (clean > 0) {
//		n += write(" ");
//		--clean;
//	}
//	n += print(text);
//	return n;
//}


#endif /* MYLCD_H_ */
