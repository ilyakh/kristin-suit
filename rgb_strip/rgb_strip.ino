int SDI = 2; 
int CKI = 3; 
int ledPin = 13; 

#define N 32

void setup() {
  pinMode(SDI, OUTPUT);
  pinMode(CKI, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(9600);
  
  clear();
  postFrame();
}

// contains the color-value for each of the addressable LED
long strip_colors[N];

// countes the number of iterations of the loop
int offset = 0;

// length of the wave segment
int segment_length = 10;

void loop() {
  // sets the initial color
  setAll( createRGB( 64, 5, 37 ) );
  
  // for (int n = (segment_length) -1; n >= 0 ; n--  ) {
  for ( int n = 0; n < (segment_length); n++ ) {
    float factor = sin( float(n) / segment_length ) * 1.25;
    
    // strip_colors[offset + n] = createRGB( 0, 0, map( n, 0, segment_length, 16, 255 ) );
    int value = factor * 255;
    strip_colors[offset + n] = createRGB( 0, 0, value ); 
  }
  
  // since calculation happens after the operation: use >=
  // this ensures that the 'offset' will never be larger than the maximum number of LEDs
  if ( offset >= N )
    offset = 0;
  else
    offset++;
  
  // use postFrame only once: preferrably at the end of the 'loop()'
  postFrame();
  
  delay(1);
}





/**
 *  Color manipulation and transition
 */


// [/] rename to fromRGB
long createRGB( int r, int g, int b ) {
  
  // reverses the order of colors to red, green, blue
  int colors[3] = { b, g, r };
  long result = 0; // the color-value accumulator
  
  for( int x = 0 ; x < 3 ; x++ ) {
    result <<= 8;
    result |= colors[x];
    result &= 0xFFFFF0;
  }
  
  return result;
}

// [/] rename to fromHSL
void createHSL(int hue, int sat, int val, int colors[3]) {
	// hue: 0-259, sat: 0-255, val (lightness): 0-255
	int r, g, b, base;

	if (sat == 0) { // Achromatic color (gray).
		colors[0]=val;
		colors[1]=val;
		colors[2]=val;
	} else  {
		base = ((255 - sat) * val)>>8;
		switch(hue/60) {
			case 0:
				r = val;
				g = (((val-base)*hue)/60)+base;
				b = base;
				break;
			case 1:
				r = (((val-base)*(60-(hue%60)))/60)+base;
				g = val;
				b = base;
				break;
			case 2:
				r = base;
				g = val;
				b = (((val-base)*(hue%60))/60)+base;
				break;
			case 3:
				r = base;
				g = (((val-base)*(60-(hue%60)))/60)+base;
				b = val;
				break;
			case 4:
				r = (((val-base)*(hue%60))/60)+base;
				g = base;
				b = val;
				break;
			case 5:
				r = val;
				g = base;
				b = (((val-base)*(60-(hue%60)))/60)+base;
				break;
		}
		createRGB( r, g, b );
	}
}



/**
 *  Mass operations
 *
 */

void setAll( long value ) {
  // changes the values, but does not post frame!
  // [!] posting frame inside this function will cause the program to halt significantly
  // and thus reduce the increase the delay by 500us.
  for ( int x = 0; x < N; x++ ) {
    strip_colors[x] = value;
  } 
}


void clear() {
  for(int x = 0 ; x < N ; x++) {
    strip_colors[x] = 0x000000;
  }
}


/**
 * The postFrame function, originanally known as post_frame.
 *
 * @author  Nathan Seidle
 * @url http://www.sparkfun.com/datasheets/Components/LED/LED_Strip_Example.pde
 */

//Takes the current strip color array and pushes it out
void postFrame() {
  //Pulling the clock low for 500us or more causes the IC to post the data.

  for( int LED_number = 0 ; LED_number < N ; LED_number++ ) {
    long this_led_color = strip_colors[LED_number]; //24 bits of color data

    for( byte color_bit = 23 ; color_bit != 255 ; color_bit-- ) {
      //Feed color bit 23 first (red data MSB)
      
      digitalWrite(CKI, LOW); //Only change data when clock is low
      
      long mask = 1L << color_bit;
      //The 1'L' forces the 1 to start as a 32 bit number, otherwise it defaults to 16-bit.
      
      if(this_led_color & mask) 
        digitalWrite(SDI, HIGH);
      else
        digitalWrite(SDI, LOW);
  
      digitalWrite(CKI, HIGH); //Data is latched when clock goes high
    }
  }

  //Pull clock low to put strip into reset/post mode
  digitalWrite(CKI, LOW);
  delayMicroseconds(500); //Wait for 500us to go into reset
}


