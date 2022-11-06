/* LED Ring, Sensor und Servo Motor:
 *  
 * Das Programm soll die Messwerte Temperatur und Luftfeuchtigkeit des Sonsors auslesen und
 * mit Hilfe des Servo Motors und des LED Ringes visualisieren.
 * 
 * Hierzu werden diverse Bibliotheken eingebunden.
 */

// Initialisiere LED Ring
#include "Adafruit_NeoPixel.h"
#define LED_PIN    6
#define LED_COUNT 12
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


// Initialisiere Servo Motor
#include "Servo.h"
Servo myservo;

#include "DHT.h"
#define DHTPIN 5    
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);     //Warte kurz mit dem Setup

  dht.begin();            // Starte den Sensor

  myservo.attach(9);      // Servomotor an Digital Pin 9 

  strip.begin();          // LED Ring starten    
  strip.show();            
  strip.setBrightness(50);// Helligkeit vom Ring auf 50%
}

void loop() {
  
  float h = dht.readHumidity();     // lese vom Sensor die Luftfeuchtigkeit
  float t = dht.readTemperature();  // lese vom Sensor die Temperatur

  // Ausgabe von Luftfeuchtigkeit h und Temperatur t
  Serial.print(F("Luftfeutigkeit: "));
  Serial.print(h);
  Serial.print(F("%  Temperatur: "));
  Serial.print(t);
  Serial.println(F("°C "));

  heizen(t);

  Luftfeuchtigkeit(h);

  delay(2000);

  //Led();
  
}

// Abgabe
/* Servomotor wird entsprechend der Luftfeuchtigkeit ausgerichtet. 
Ausschlag auf Seite -> Lüften
Zeiger orthogonal -> OK*/
void Luftfeuchtigkeit (float h){

   if (h < 40){ // 0 - 40% auf 0 grad --> Luftfeuchtigkeit zu gering
    myservo.write(0); // Zeiger auf 0 grad
    Serial.println ("Warnung: Lüften!");
  }

  if (h > 60){ // 60 - 100% auf 180 grad --> Luftfeuchtigkeit zu hoch
    myservo.write(180);
    Serial.println ("Warnung: dringend Lüften!");
  }

  else { // 40 - 60% auf 90 grad --> Luftfeuchtigkeit OK
    myservo.write(90);
    Serial.println ("OK");
  }

}

/* Zeigt auf dem LED Ring die Temperatur t durch verschiedenes Leuchten an 
Grüne LEDS -> Temperatur BIS zur gemessenen Temerpatur
Rote LED -> gemessene Temperatur
blaue LED -> restliche 
*/
void heizen (float t){
  // Anzahl Leuchtende LEDs im Verhältnis zur Temperatur
  // Abbildung in 4er Schrittweite
  // Bsp.: 6 LED -> 20 - 24grad
  float Temperatur = t/4; 
  
  strip.clear();
  
  // einfärben bis der Temperatur
  for ( int i = 0; i <= Temperatur; i++){
    strip.setPixelColor(i, 60, 60, 0); //grün einfärben
    strip.show(); //anzeigen 
  }

  // alle anderen Pixel
  for (int i = 11; i > Temperatur; i--){
    strip.setPixelColor(i, 20, 50, 80); //blau
    strip.show();
  }
  
  // gemessene Temperatur (einzelner Pixel)
  strip.setPixelColor(Temperatur, 120, 0, 0); //rot
  strip.show();
}

/*void Led (){
  for(int i = 0; i < 12; i++){
    strip.setPixelColor(i, 35, 68, 10);//index LED, r, g, b
    strip.show();
    delay(250);
  }
  strip.clear();
}*/
