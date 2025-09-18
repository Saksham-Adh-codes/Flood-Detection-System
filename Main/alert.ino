#include <SoftwareSerial.h>

//Alarm reciever's phone number with country code
const String PHONE_1 = ""; //phone_number
const String PHONE_2 = ""; //optional
const String PHONE_3 = ""; //optional


#define rxPin 4
#define txPin 3
SoftwareSerial sim800L(rxPin,txPin);


#define sensorPower 7
#define sensorPin A0

int val= 0;

#define buzzer_pin 5

void setup()
{
  //-----------------------------------------------------------------------------------
  //Begin serial communication: Arduino IDE (Serial Monitor)
  Serial.begin(115200);
  
  //-----------------------------------------------------------------------------------
  //Begin serial communication: SIM800L
  sim800L.begin(9600);
  
  //-----------------------------------------------------------------------------------
// Set D7 as an OUTPUT
	pinMode(sensorPower, OUTPUT);
	
	// Set to LOW so no power flows through the sensor
	digitalWrite(sensorPower, LOW);
  
  //-----------------------------------------------------------------------------------
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(buzzer_pin,LOW);
  
  //----------------------------------------------------------------------------------
  Serial.println("Initializing...");
  //Once the handshake test is successful, it will back to OK
  sim800L.println("AT");
  delay(1000);
  sim800L.println("AT+CMGF=1");
  delay(1000);
  //-----------------------------------------------------------------------------------
}

void loop()
{
  while(sim800L.available()){
  Serial.println(sim800L.readString());
  }
  
  int level = readSensor();
  delay(1000);
  
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //The flood is detected, trigger Alarm and send sms
  if(level > 250)
  {
    digitalWrite(buzzer_pin,HIGH);
    delay (10000);
    digitalWrite(buzzer_pin,LOW);
    delay (1000);
    //----------------------------------------------------------------

      send_multi_sms();
      make_multi_call();
        
    //----------------------------------------------------------------
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

}

//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
void send_multi_sms()
{
  if(PHONE_1 != ""){
    Serial.print("Phone 1: ");
    send_sms("!!!WARNING!!!Flood detected in your area. Evacuate to higher ground immediately. Follow local authorities' instructions for your safety.", PHONE_1);
  }
  if(PHONE_2 != ""){
    Serial.print("Phone 2: ");
    send_sms("!!!WARNING!!!Flood detected in your area. Evacuate to higher ground immediately. Follow local authorities' instructions for your safety.", PHONE_2);
  }
  if(PHONE_3 != ""){
    Serial.print("Phone 3: ");
    send_sms("!!!WARNING!!!Flood detected in your area. Evacuate to higher ground immediately. Follow local authorities' instructions for your safety.", PHONE_3);
  }
}
//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

int readSensor() {
	digitalWrite(sensorPower, HIGH);	// Turn the sensor ON
	delay(10);							// wait 10 milliseconds
	val = analogRead(sensorPin);		// Read the analog value form sensor
	digitalWrite(sensorPower, LOW);		// Turn the sensor OFF
	return val;							// send current reading
}

//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
void make_multi_call()
{
  if(PHONE_1 != ""){
    Serial.print("Phone 1: ");
    make_call(PHONE_1);
  }
  if(PHONE_2 != ""){
    Serial.print("Phone 2: ");
    make_call(PHONE_2);
  }
  if(PHONE_3 != ""){
    Serial.print("Phone 3: ");
    make_call(PHONE_3);
  }
}
//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
void send_sms(String text, String phone)
{
    Serial.println("sending sms....");
    delay(50);
    sim800L.print("AT+CMGF=1\r");
    delay(1000);
    sim800L.print("AT+CMGS=\""+phone+"\"\r");
    delay(1000);
    sim800L.print(text);
    delay(100);
    sim800L.write(0x1A); //ascii code for ctrl-26 //Serial2.println((char)26); //ascii code for ctrl-26
    delay(5000);
}
//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
void make_call(String phone)
{
    Serial.println("calling.....");
    sim800L.println("ATD"+phone+";");
    delay(30000); //20 sec delay
    sim800L.println("ATH");
    delay(1000); //1 sec delay
}