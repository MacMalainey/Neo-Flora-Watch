#include <Time.h>

#include <Adafruit_NeoPixel.h>

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
int hrc = 100;
int hr;
int mn;
int sc;
int mnc = 100;
int scc = 100;
int start = 0;
int tempO;
boolean below0;
boolean belowChec;
#define tempPin A9
//HardwareSerial Serial = Serial1;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, 6, NEO_GRB + NEO_KHZ800);
int chec;
Adafruit_GPS GPS(&Serial1);
//CHANGE FOR TIME ZONE
int offset = -7;
int mode = 1;
#define buttonPin 12

// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy
void setup()  
{
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  // the nice thing about this code is you can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS for you. that makes the
  // loop code a heck of a lot easier!
  useInterrupt(true);
  delay(1000);
  // Ask for firmware version
  start = 0;
  Serial1.println(PMTK_Q_RELEASE);
  strip.begin();
  strip.show();
  // Make input & enable pull-up resistors on switch pins for pushbutton
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  for (int ledOn = 0; ledOn <= 11; ledOn++)
  {
    delay(200);
  	strip.setPixelColor(ledOn, strip.Color(0, 11, ledOn));
    strip.show();
  }
  delay(200);
}


// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
    // writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
#endif
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}
uint32_t gpsTimer = millis();
  
int LEDclock(boolean clocknew, int start)
{
   // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
  
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return 1;  // we can fail to parse a sentence in which case we should just wait for another
  }
  if (gpsTimer > millis()) gpsTimer = millis();

  if (start == 0) {
    if (GPS.fix) {
      // set the Time to the latest GPS reading
      setTime(GPS.hour, GPS.minute, GPS.seconds, GPS.day, GPS.month, GPS.year);
      delay(50);
      adjustTime(offset * SECS_PER_HOUR);
      delay(500);
      start = 1;
    }
  }    
  // approximately every 60 seconds or so, update time
  if ((millis() - gpsTimer > 60000) && (start == 1)) {
    gpsTimer = millis(); // reset the timer
    if (GPS.fix) {
      // set the Time to the latest GPS reading
      setTime(GPS.hour, GPS.minute, GPS.seconds, GPS.day, GPS.month, GPS.year);
      delay(50);
      adjustTime(offset * SECS_PER_HOUR);
      delay(500);
    }
  }
  hr = (hour());
  mn = (minute());
  sc = (second());
  mn = mn / 5;
  sc = sc / 5;

  if (hr >= 12)
    {
      hr = hr - 12;
    }
  if (clocknew == true){
    strip.setPixelColor(0, strip.Color(10, 10, 10));
    strip.setPixelColor(sc, strip.Color(0, 20, 0));
    strip.setPixelColor(mn, strip.Color(0, 0, 20));
    strip.setPixelColor(hr, strip.Color(20, 0, 0));
    strip.show();
  }
  if (hr == mn && hr == sc)
  {
    strip.setPixelColor(hrc, strip.Color(0, 0, 0));
    strip.setPixelColor(mnc, strip.Color(0, 0, 0));
    strip.setPixelColor(scc, strip.Color(0, 0, 0));
    strip.setPixelColor(hr, strip.Color(20, 0, 0));
    strip.show();
  }
  if (hr == mn)
  {
    strip.setPixelColor(hrc, strip.Color(0, 0, 0));
    strip.setPixelColor(mnc, strip.Color(0, 0, 0));
    strip.setPixelColor(hr, strip.Color(20, 0, 0));
    strip.show();
  }
  if (sc == hr)
  {
    strip.setPixelColor(hrc, strip.Color(0, 0, 0));
    strip.setPixelColor(scc, strip.Color(0, 0, 0));
    strip.setPixelColor(hr, strip.Color(20, 0, 0));
    strip.show();
  }
  
  if (hr != hrc)
  {
    strip.setPixelColor(hrc, strip.Color(0, 0, 0));
    strip.setPixelColor(hr, strip.Color(20, 0, 0));
    hrc = hr;
    strip.show();
  }
  if (sc == mn  && sc != hr)
  {
    strip.setPixelColor(mnc, strip.Color(0, 0, 0));
    strip.setPixelColor(scc, strip.Color(0, 0, 0));
    strip.setPixelColor(mn, strip.Color(0, 0, 20));
    strip.show();
  }
  if (mn != mnc && mn != hr)
  {
    strip.setPixelColor(mnc, strip.Color(0, 0, 0));
    strip.setPixelColor(mn, strip.Color(0, 0, 20));
    mnc = mn;
    strip.show();
  }
  if (sc != scc && sc != mn && sc != hr)
  {
    strip.setPixelColor(scc, strip.Color(0, 0, 0));
    strip.setPixelColor(sc, strip.Color(0, 20, 0));
    scc = sc;
    strip.show();
  }
  if (hr != 0 && mn != 0 && sc != 0)
  {
    strip.setPixelColor(0, strip.Color(10, 10, 10));
    strip.show();
  }
  Serial.println("Hour:");
  Serial.println(hr);
  Serial.println("Minute:");
  Serial.println(mn);
  Serial.println("Second:");
  Serial.println(sc); 
  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW)
  {
    delay(1000);
    return 0;
  }
  else
  {
    return 1;
  }
  
}
int LEDtemp(boolean tempnew)
{
  //getting the voltage reading from the temperature sensor
 int reading = analogRead(tempPin);  
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * 3.3;
 voltage /= 1024.0;
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
 Serial.print(temperatureC); Serial.println(" degrees C");
 int tempD = temperatureC / 3;
 Serial.println(tempD);
 if (temperatureC < 0)
 {
  tempD = tempD * -1;
   below0 = true;
 }
 else
 {
  below0 = false;
 }
 if (tempD != tempO or tempnew == true or belowChec != below0)
 {
  tempO = tempD;
  for (int ledOn = 0; ledOn < tempD; ledOn++)
  {
    if (below0 == true)
    {
      strip.setPixelColor(ledOn, strip.Color(5, 5, 20));
     strip.show();
    }
    else
    {
     strip.setPixelColor(ledOn, strip.Color(20, 5, 0));
     strip.show();
    }
  }
  for (int ledOn = 11; ledOn > tempD; ledOn--)
  {
     strip.setPixelColor(ledOn, strip.Color(0, 0, 0));
    strip.show();
  }
  belowChec = below0;
 }
 int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW)
  {
    delay(1000);
    return 1;
  }
  else
  {
    return 0;
  }
}
void loop()                     // run over and over again
{
  if (mode == 0)
  {
    if (chec != mode)
    {
      for (int ledOn = 0; ledOn <= 11; ledOn++)
      {
      strip.setPixelColor(ledOn, strip.Color(0, 0, 0));
      strip.show();
      chec = mode;
      mode = LEDtemp(true);
      }
    }
    else
    {
   		mode = LEDtemp(false);
    }
  }
  else if (mode == 1)
  {
    if (chec != mode)
    {
      for (int ledOn = 0; ledOn <= 11; ledOn++)
      {
      strip.setPixelColor(ledOn, strip.Color(0, 0, 0));
      strip.show();
      chec = mode;
      mode = LEDclock(true, start);
      }
    }
    else
    {
		mode = LEDclock(false, start);
    }
  }
  start = 1;
}
  
  

