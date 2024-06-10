#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define led D2
int ldrValue = 0;
#define TRIGGER D5
#define ECHO    D6
int distance,duration;
void mqttConnect();
void PublishData(int bri,int dis);

const char* ssid = "BnSC-a2ty";
const char* password = "abcxy123";
String command;
#define ORG "svcke6"
#define DEVICE_TYPE "MYMCU"
#define DEVICE_ID "ID1"
#define TOKEN "password"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/Data/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);


void setup() {
 Serial.begin(115200);
 Serial.println();
pinMode(D7, OUTPUT);  
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
}

void loop()
{ 
  ldrValue = analogRead(A0); // read the value from the sensor
ldrValue=map(ldrValue,0,1023,255,0);
Serial.println(ldrValue); //prints the values coming from the sensor on the screen
analogWrite(led,ldrValue);
delay(100);
 digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER, LOW);
 duration=pulseIn(ECHO,HIGH);  /*echo*/
  distance=0.0340*(duration/2);
  
   Serial.println("DISTANCE=");
  Serial.println(distance);
  int d=distance;
int t=ldrValue;

if(distance<20)
{
if(ldrValue<15)
{digitalWrite(D7,LOW);

}
else
{digitalWrite(D7,HIGH);
  }
}
else
{
  digitalWrite(D7,HIGH);
}
 if (isnan(t)||isnan(d) )
{
Serial.println("Failed to read from sensor!");
delay(1000);
return;
}
PublishData(t,d);
 if (!client.loop()) {
    mqttConnect();
  }
delay(100);
  }



  void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}

void initManagedDevice() {
  if (client.subscribe(topic)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}

void PublishData(int bri,int dis)
{
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"brightness\":";
  payload += bri;
    payload+="," "\"distance\":";
  payload += dis;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}

