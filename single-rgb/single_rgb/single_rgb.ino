const int r = 2;
const int g = 3;
const int b = 4;

void setup() {
  pinMode( r, OUTPUT );
  pinMode( g, OUTPUT );
  pinMode( b, OUTPUT );
}

void loop() {
  digitalWrite( r, HIGH );
  digitalWrite( g, HIGH );
  digitalWrite( b, HIGH ); 
}
