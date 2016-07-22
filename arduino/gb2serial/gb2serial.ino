// Gautier Hattenberger <gautier.hattenberger@enac.fr>
// 2016
//
// Based on Tom Van den Bon (2012/12/21)
//
// Send and Receive Data to Gameboy via Link Cable
// and Forward to Serial
// Designed for Arduino Pro Mini

// Link Cable     Arduino      Desc (GB side)
// 6              GND          GND
// 5              3            SC
// 2              4            SI
// 3              5            SO

int SI_PIN = 5; // Arduino side (= SO GB side)
int SO_PIN = 4; // Arduino side (= SI GB side)
int CLOCK_PIN = 3;
int CLOCK_INT = 1; // pin 3 correspond to INT1
int LED = 13; // if needed

unsigned volatile long last_receive = 0;
int volatile from_gb_data = 0;
int volatile from_gb_counter = 0;
int volatile from_gb_val = 0;
int to_gb_data = 0;
int to_gb_counter = 0;

void setup() {
  // Init pins
  pinMode(SI_PIN, INPUT);
  digitalWrite( SI_PIN, HIGH);
  pinMode(SO_PIN, OUTPUT);
  //pinMode(CLOCK_PIN, OUTPUT);
  // Init serial link with PC
  Serial.begin(9600);
  // Init interrupt to read input data
  attachInterrupt(CLOCK_INT, clock, RISING);
  // Init LED
  pinMode(LED, OUTPUT);
}

// handler on received signal from the GB
void clock()
{
  if (last_receive > 0) {
    if (micros() - last_receive > 120) {
      // too long, must be a new sequence (takes about 120 microsecs for a bit)
      // reset counter and val
      from_gb_counter = 0;
      from_gb_val = 0;
    }
  }

  // read input pin state
  from_gb_data = digitalRead(SI_PIN);

  // store current bit in val
  if (from_gb_data == HIGH)
    from_gb_val |= (1 << (7 - from_gb_counter));

  if (from_gb_counter == 7) {
    // byte is complete
    // forward to serial link
    Serial.println(from_gb_val, DEC);
    from_gb_val = 0;
    from_gb_counter = -1;
  }

  // counter and timer
  from_gb_counter++;
  last_receive = micros();
}

// the loop routine runs over and over again forever
void loop() {

  // check for new characters
  if (Serial.available() > 0) {
    // read data from serial
    to_gb_data = Serial.read();

    // detach interrupt before sending
    detachInterrupt(CLOCK_INT);
    // Arduino is now in master mode and generates the clock
    pinMode(CLOCK_PIN, OUTPUT);
    // send to GB
    for (to_gb_counter = 0; to_gb_counter < 8; to_gb_counter++) {
      if (to_gb_data & 0x80 ) {
        digitalWrite(SO_PIN, HIGH);
        digitalWrite(CLOCK_PIN, LOW);
        delayMicroseconds(50);
        digitalWrite(CLOCK_PIN, HIGH);
        delayMicroseconds(50);
      }
      else {
        digitalWrite(SO_PIN, LOW);
        digitalWrite(CLOCK_PIN, LOW);
        delayMicroseconds(50);
        digitalWrite(CLOCK_PIN, HIGH);
        delayMicroseconds(50);
      }
      to_gb_data = to_gb_data << 1;
    }
    pinMode(CLOCK_PIN, INPUT);
    // enable again interrupt, Arduino is in slave mode
    attachInterrupt(CLOCK_INT, clock, RISING);
  }

}

