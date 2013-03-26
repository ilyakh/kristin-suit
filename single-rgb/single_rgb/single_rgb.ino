// mapping the pins to the colors
const int r = 10;
const int g = 9;
const int b = 11;

void setup() {
  pinMode( r, OUTPUT );
  pinMode( g, OUTPUT );
  pinMode( b, OUTPUT );
  
  analogWrite( b, 255 );
}

int counter = 0;
int sequence_length = 255;
int exhale_period = 1000;

void loop() {
  // begin step
  if ( counter > sequence_length ) {
    counter = 0;
    delay( exhale_period );
  }
  
  if ( counter <= ( sequence_length / 3 ) ) {
    analogWrite( g, counter );
    analogWrite( r, counter );
  } else {
    analogWrite( g, sequence_length - counter ); 
    analogWrite( r, sequence_length - counter );
  }
  
  // end step
  counter++;
  delay(10);
}
