
#include <SPI.h>
#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Setting MAC Address
unsigned long int uploadTime = millis();
unsigned long int updateVochtigheid = 600000;
unsigned long int intervalTime = 120000;
unsigned long int vorigeTime = 1;
unsigned long int vorigeTime2 = 1;
const int moister = A0;
double moisterstatus;
double procent;
double grondvochtigheid;
float grondvochtigheidData;


char server[] = "192.168.0.102";
IPAddress ip(192,168,0,100); 
EthernetClient client; 

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }
  Serial.println("Verbinding aan het maken met Router:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Er kon geen verbinding worden opgezet met de Router");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("De Ethernet Shield van de Arduino is niet gevonden. :(");
    } 
    while (true) {
      delay(1);
    }
  }
  // print your local IP address:
  Serial.println("Verbinding met Router succesvol opgezet");
  Serial.print("Mijn IP adres: ");
  Serial.println(Ethernet.localIP());
}

void loop(){
  uploadTime = millis();
  //Serial.println(vorigeTime);
  //Serial.println(" nu");
  //Serial.println(uploadTime);
  //Serial.println(" intervaltime");
  //Serial.println(intervalTime);
  //Serial.println(" updateVocht");
  //Serial.println(updateVochtigheid);
  //Serial.println(" ");
  Serial.println(grondvochtigheidData);
  
  
   moisterstatus = analogRead(moister);
  procent = moisterstatus / 10.23;
  grondvochtigheid = 100 - procent;
  if(grondvochtigheid <= 0){
    grondvochtigheid = 0;
  } 
  grondvochtigheidData = grondvochtigheid;
  if(uploadTime - vorigeTime >= updateVochtigheid)
  {
  moisterstatus = analogRead(moister);
  procent = moisterstatus / 10.23;
  grondvochtigheid = 100 - procent;
  if(grondvochtigheid <= 0){
    grondvochtigheid = 0;
  } 
  grondvochtigheidData = grondvochtigheid;
  Grafiekdata_Database();
  vorigeTime = uploadTime;
  } 

  if (grondvochtigheid <= 25){
  if (uploadTime - vorigeTime2 >= intervalTime)
  {
  
  Serial.print(grondvochtigheid);
  Serial.println("% vochtigheid");
  IntervalData_Database();
   vorigeTime2 = uploadTime;
  } 
 
  } 
  
}

void IntervalData_Database()   //CONNECTING WITH MYSQL
{
   if (client.connect(server, 80)) {
    Serial.println("Verbonden met de server");
    // Make a HTTP request:
    Serial.println("Data verzonden naar de server");
    client.print("GET /Arduino/grondvochtigheid.php?grondvochtigheid=");     //YOUR URL
    client.print(grondvochtigheidData);
    Serial.print(grondvochtigheidData);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: 192.168.0.101");
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("Geen verbinding met de server");
  }
}

void Grafiekdata_Database()   //CONNECTING WITH MYSQL
{
   if (client.connect(server, 80)) {
    Serial.println("Verbonden met de server");
    // Make a HTTP request:
    Serial.println("Data verzonden naar de server | Grafiek");
    client.print("GET /Arduino/grondvochtigheid2.php?grondvochtigheid=");     //YOUR URL
    client.print(grondvochtigheidData);
    Serial.print(grondvochtigheidData);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: 192.168.0.101");
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("Geen verbinding met de server");
  }
}
