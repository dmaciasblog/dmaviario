#include <RTClib.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define Boton 3

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  //seteamos el lcd
RTC_DS3231 rtc;

String daysOfTheWeek[7] = { "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado" };
String monthsNames[12] = { "Enero", "Febrero", "Marzo", "Abril", "Mayo",  "Junio", "Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre" };

 int inicio = 1;
 int estadoAC = 0;
 int estadoAN = 0;
const int led2 = 8;
byte dim_led2 = 255;
byte dim2;
const int led1 = 6;
byte dim_led1 = 255;
byte dim;
long tiempo = 7031;
long tiempo2 =1000;
long previousMillis = 0;
unsigned long currentMillis;
long tiempoUltimeLectura=0;
//int h = ( now.hour() );
//int m = ( now.minute() );

void setup() {

  lcd.begin(20,4);
  dht.begin();
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(Boton, INPUT);
  dim = 0;
  dim2 = 0;

  if (!rtc.begin()) {
     Serial.println(F("Couldn't find RTC"));
     while (1);
  }

  if (rtc.lostPower()) {
     // Fijar a fecha y hora de compilacion
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

     // January 21, 2014 at 3am you would call:
     //rtc.adjust(DateTime(2021, 1, 30, 11, 14, 0));
  }


}

void printDate(DateTime date) {
   lcd.print(date.year(), DEC);
   lcd.print('/');
   lcd.print(date.month(), DEC);
   lcd.print('/');
   lcd.print(date.day(), DEC);
   lcd.print(" (");
   lcd.print(daysOfTheWeek[date.dayOfTheWeek()]);
   lcd.print(") ");
   lcd.print(date.hour(), DEC);
   lcd.print(':');
   lcd.print(date.minute(), DEC);
   lcd.print(':');
   lcd.print(date.second(), DEC);
   lcd.println();

}
void loop() {
/*  estadoAC = digitalRead (Boton);
  if(estadoAC && estadoAN == 0) {
    inicio = 1 - inicio;
  }
  estadoAN = estadoAC;

  if(inicio == 1){
    lcd.backlight();
  }
  else {
    lcd.noBacklight();
  }*/
///////////////////////////////////
///   TEMPERATURA Y HUMEDAD
///////////////////////////////////

  if(millis() - tiempoUltimeLectura > 2000){
    float h = dht.readHumidity();
    float t = dht.readTemperature();
  lcd.setCursor(0,1);
  lcd.print("HR:");
  if ((float)h < 10){
    lcd.print("0");
    lcd.print((float)h);
    lcd.print("%");
  }
  else{
  lcd.print((float)h);
  lcd.print( "%" );
  }
  lcd.setCursor(12,1);
  lcd.print("T:");
  if ((float)t < 10){
    lcd.print("0");
    lcd.print((float)t);
    lcd.print("C");
  }
  else{
  lcd.print((float)t);
  lcd.print( "C" );
  }



  tiempoUltimeLectura = millis();
  }



///////////////////////////
/// MOSTRAR FECHA HORA
//////////////////////////

   DateTime now = rtc.now();
   lcd.setCursor(0,0);
   if (now.day() < 10){
    lcd.print("0");
    lcd.print(now.day());
   }
   else{
   lcd.print(now.day());
   }
   lcd.print("/");
   if (now.month() < 10){
    lcd.print("0");
    lcd.print(now.month());
   }
   else{
   lcd.print(now.month());
   }
   lcd.print("/");
   lcd.print(now.year());
   lcd.setCursor(15,0);
   if (now.hour() < 10){
    lcd.print("0");
    lcd.print(now.hour());
   }
   else{
   lcd.print(now.hour());
   }
   lcd.print(":");
   if(now.minute() < 10){
    lcd.print("0");
    lcd.print(now.minute());
   }
   else{
   lcd.print(now.minute());
   }


/////////////////////////////////////////
///  HORARIOS EVENTOS
/////////////////////////////////////////
int hours = ( (now.hour() * 60 ) + (now.minute()) );
int hours2 = ( (now.hour() * 60 ) + (now.minute()) );
    if (now.month() == 1 ){
        if ( now.day() == 31 ) {
            if ( hours < 480 ){
                luna();
            }
            if ( hours2 < 510 ) {
                lunaH();
            }
            if ( hours >= 550 && hours < 555 ) {
                alba();  // funcion que amanece la linea 1
            }
            if (hours2 >= 560 && hours2 < 590 ) { //tengo que poner el rango x si se va la luz o algo
                albaH(); // funcion que amanece la linea 2
            }
            if ( hours > 510 && hours < 1140 ) {
                amaeme();
            }
            if ( hours2 > 540 && hours2 < 1080 ) {
                amaemeH();
            }
            if ( hours > 1140 ) {
                ocaso();
            }
            if ( hours2 > 1080 ) {
                ocasoH();
            }
            if ( hours > 1170 ) {
                luna();
            }
            if ( hours2 > 1110) {
                lunaH();
            }
        }
    }

/*  --- DIA 11 ENERO
        if(now.day() == 11 ){
            if(hours > 7.42 && hours < 8.07){
                alba();
            }
            if(hours > 8.07 && hours < 19.30){
                lcd.setCursor(11,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours > 19.30 && hours <= 19.68){
                ocaso();
            }
            if((hours >= 0.0 && hours <= 7.70) || (hours >= 19.68)){
                lcd.setCursor(11,2);
                lcd.print(hours);
                luna();
            }
        }
//  --- DIA 12 ENERO
        if(now.day() == 12 ){
            if(hours > 7.65 && hours < 8.13){
                alba();
            }
            if(hours > 8.13 && hours < 19.20){
                lcd.setCursor(11,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours > 19.20 && hours <= 19.68){
                ocaso();
            }
            if((hours >= 0.0 && hours <= 7.70) || (hours > 19.68)){
                lcd.noBacklight();
                luna();
            }
        }


//  --- DIA 13 ENERO
        if(now.day() == 13 ){
            if(hours >= 7.60 && hours <= 8.10){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 8.10 && hours < 19.30){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 19.30 && hours <= 19.80){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.60) || (hours > 19.80)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }

        }


//  --- DIA 14 ENERO
        if(now.day() == 14 ){
            if(hours >= 7.55 && hours <= 8.05){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 8.06 && hours < 19.38){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 19.38 && hours <= 19.88){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.55) || (hours > 19.88)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 15 ENERO
        if(now.day() == 15 ){
            if(hours >= 7.50 && hours <= 8.10){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 8.10 && hours < 20.00){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 19.50 && hours <= 20.00){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.50) || (hours > 20.00)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 16 ENERO
        if(now.day() == 16 ){
            if(hours >= 7.45 && hours <= 7.95){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 7.95 && hours < 19.60){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 19.60 && hours <= 20.10){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.45) || (hours > 20.10)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 17 ENERO
        if(now.day() == 17 ){
            if(hours >= 7.40 && hours <= 7.90){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 7.90 && hours < 19.70){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 19.70 && hours <= 20.20){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.40) || (hours > 20.20)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 18 ENERO
        if(now.day() == 18 ){
            if(hours >= 7.35 && hours <= 7.85){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 7.85 && hours < 19.80){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 19.80 && hours <= 20.30){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.35) || (hours > 20.03)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 19 ENERO
        if(now.day() == 19 ){
            if(hours >= 7.30 && hours <= 7.80){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 7.80 && hours < 19.90){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 19.90 && hours <= 20.40){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.30) || (hours > 20.40)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 20 ENERO
        if(now.day() == 20 ){
            if(hours >= 7.25 && hours <= 7.75){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 7.75 && hours < 20.00){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 20.00 && hours <= 20.50){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.25) || (hours > 20.50)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 21 ENERO
        if(now.day() == 21 ){
            if(hours >= 7.20 && hours <= 7.70){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 7.70 && hours < 20.10){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 20.10 && hours <= 20.60){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.20) || (hours > 20.60)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 22 ENERO
        if(now.day() == 22 ){
            if(hours >= 7.15 && hours <= 7.65){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 7.65 && hours < 20.20){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 20.20 && hours <= 20.70){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.15) || (hours > 20.70)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 23 ENERO
        if(now.day() == 23 ){
            if(hours >= 7.10 && hours <= 7.60){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 7.60 && hours < 20.30){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 20.30 && hours <= 20.80){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.10) || (hours > 20.80)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 24 ENERO
        if(now.day() == 24 ){
            if(hours >= 7.06 && hours <= 7.56){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 7.56 && hours < 20.40){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 20.40 && hours <= 20.90){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.06) || (hours > 20.90)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 25 ENERO
        if(now.day() == 25 ){
            if(hours >= 7.03 && hours <= 7.53){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 7.53 && hours < 20.50){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 20.50 && hours <= 21.00){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.03) || (hours > 21.00)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 26 ENERO
        if(now.day() == 26 ){
            if(hours >= 7.01 && hours <= 7.51){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 7.51 && hours < 20.60){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 20.60 && hours <= 21.10){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.01) || (hours > 21.10)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 27 ENERO
        if(now.day() == 27 ){
            if(hours >= 7.00 && hours <= 7.50){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 7.50 && hours < 20.70){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 20.70 && hours <= 21.20){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.00) || (hours > 21.20)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 28 ENERO
        if(now.day() == 28 ){
            if(hours >= 7.00 && hours <= 7.50){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 7.50 && hours < 20.80){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 20.80 && hours <= 21.30){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.00) || (hours > 21.30)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 29 ENERO
        if(now.day() == 29 ){
            if(hours >= 7.00 && hours <= 7.50){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 7.50 && hours < 20.70){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 20.90 && hours <= 21.40){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.00) || (hours > 21.40)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 30 ENERO
/*        if(now.day() == 30 ){
            if( now.hour() == 13 && now.minute() == 03 ){
            //if(hours >= 11 && hours <= 7.50){
                //lcd.backlight();
                //lcd.setCursor(13,2);
                //lcd.print(hours);
                alba();
            }
            if( now.hour()== 13 && now.minute() == 06 ){
                //lcd.setCursor(13,2);
                //lcd.print("amaH");
                albaH();
            }

            if(hours > 7.50 && hours < 21.00){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 21.00 && hours <= 21.50){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.00) || (hours > 21.50)){
            //    lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
        }


//  --- DIA 31 ENERO
        if(now.day() == 31 ){
            if(hours >= 7.00 && hours <= 7.50){
                lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 7.50 && hours < 21.10){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 21.10 && hours <= 21.60){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 7.00) || (hours > 21.60)){
                lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
      }

    if(now.month() >= 2 && now.month() <= 6 ){

//  --- DE FEBRERO A JUNIO
            if(hours >= 6.00 && hours <= 6.50){
               // lcd.backlight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                alba();
            }
            if(hours > 6.50 && hours < 21.10){
                lcd.setCursor(13,2);
                lcd.print(hours);
                amaeme();
            }
            if(hours >= 21.10 && hours <= 21.60){
                lcd.setCursor(13,2);
                lcd.print(hours);
                ocaso();
            }
            if((hours >= 0.0 && hours < 6.00) || (hours > 21.60)){
               // lcd.noBacklight();
                lcd.setCursor(13,2);
                lcd.print(hours);
                luna();
            }
       }*/
}

 //--------------------------------------
 //   FUNCION BORRAR LINEA
 //--------------------------------------

 void rline() {
  lcd.print("                    ");
}

///////////////////////////////////////
/// FUNCION AMANECER
//////////////////////////////////////

void alba(){
//Data d = rtc.getData();
  currentMillis = millis();
  if(currentMillis - previousMillis > tiempo)
  {
    previousMillis = currentMillis;
    if ( dim < dim_led1 )
    {
      dim += 1;
      lcd.setCursor(0,2);
      lcd.print("dim: ");
      lcd.print((int)dim);

      if (dim >= dim_led1){
        dim = dim_led1;


      }

    analogWrite( led1, dim );
    lcd.setCursor(0,3);
    lcd.print("AMANECE1");
    if (dim == 255){
      lcd.setCursor(0,2);
      rline();
      lcd.setCursor(0,3);
      rline();
      lcd.setCursor(0,3);
      lcd.print("DIA");
    }
   }
  }
}



///////////////////////////////
////// prueba segundo led
///////////////////////////////

void albaH(){
//Data d = rtc.getData();
  currentMillis = millis();
  if(currentMillis - previousMillis > tiempo)
  {
    previousMillis = currentMillis;
    if ( dim2 < dim_led2 )
    {
      dim2 += 1;
      lcd.setCursor(12,2);
      lcd.print("dim: ");
      lcd.print((int)dim2);

      if (dim2 >= dim_led2){
        dim2 = dim_led2;


      }

    analogWrite( led2, dim2 );
    lcd.setCursor(10,3);
    lcd.print("AMANECER2");
    if (dim2 == 255){
      lcd.setCursor(0,2);
      rline();
      lcd.setCursor(0,3);
      rline();
      lcd.setCursor(10,3);
      lcd.print("DIA");
    }
   }
  }
}

//////////////////////////////////////
///  FUNCION AMANECER EMERGENCIA
//////////////////////////////////////

void amaeme(){
//Data d = rtc.getData();
  currentMillis = millis();
  if(currentMillis - previousMillis > tiempo2)
  {
    previousMillis = currentMillis;
    if ( dim < dim_led1 ){
      dim += 1;
      lcd.setCursor(3,2);
      lcd.print("dim: ");
      lcd.print((int)dim);

      if (dim >= dim_led1){
        dim = dim_led1;
      }

    analogWrite( led1, dim );
    lcd.setCursor(3,2);
    lcd.print("dim: ");
    lcd.print((int)dim);
    lcd.setCursor(0,3);
    lcd.print("AMA-EME");

    if (dim == 255){
      lcd.setCursor(0,2);
      rline();
      lcd.setCursor(0,3);
      rline();
      lcd.setCursor(0,3);
      lcd.print("DIA");
    }
   }
  }
}


void amaemeH(){
//Data d = rtc.getData();
  currentMillis = millis();
  if(currentMillis - previousMillis > tiempo2)
  {
    previousMillis = currentMillis;
    if ( dim2 < dim_led2 ){
      dim2 += 1;
      lcd.setCursor(10,2);
      lcd.print("dim: ");
      lcd.print((int)dim2);

      if (dim2 >= dim_led2){
        dim2 = dim_led2;
      }

    analogWrite( led2, dim2 );
    lcd.setCursor(10,2);
    lcd.print("dim: ");
    lcd.print((int)dim2);
    lcd.setCursor(10,3);
    lcd.print("AMA-EME2");

    if (dim2 == 255){
      lcd.setCursor(0,2);
      rline();
      lcd.setCursor(0,3);
      rline();
      lcd.setCursor(10,3);
      lcd.print("DIA");
    }
   }
  }
}

////////////////////////////////////
/// FUNCION ATARDECER
////////////////////////////////////

void ocaso(){
  currentMillis = millis();
  if(currentMillis - previousMillis > tiempo)
  {
    previousMillis = currentMillis;

    if(dim > 0){
      dim -= 1;
      lcd.setCursor(0,2);
      lcd.print("dim: ");
      lcd.print((int)dim);
      lcd.setCursor(0,3);
      lcd.print("OCASO");

      if (dim <= 0){
        dim = 1;
      }
      if (dim == 100 || dim == 10){
          rline();
      }
    analogWrite( led1, dim );

    }
  }
}

void ocasoH(){
  currentMillis = millis();
  if(currentMillis - previousMillis > tiempo)
  {
    previousMillis = currentMillis;

    if(dim2 > 0){
      dim2 -= 1;
      lcd.setCursor(3,2);
      lcd.print("dim: ");
      lcd.print((int)dim2);
      lcd.setCursor(10,3);
      lcd.print("OCASO2");

      if (dim2 <= 0){
        dim2 = 1;
      }
      if (dim2 == 100 || dim2 == 10){
          rline();
      }
    analogWrite( led2, dim2 );

    }
  }
}

//////////////////////////////////////
///   FUNCION LUZ LUNA
/////////////////////////////////////

void luna(){
  analogWrite(led1, 1);
  lcd.setCursor(0,2);
  rline();
  lcd.setCursor(0,3);
  rline();
  lcd.setCursor(0,3);
  lcd.print("LUNA 1");
}

void lunaH(){
  analogWrite(led2, 1);
  lcd.setCursor(0,2);
  rline();
  //lcd.setCursor(0,3);
  //rline();
  lcd.setCursor(10,3);
  lcd.print("LUNA 2");
}
