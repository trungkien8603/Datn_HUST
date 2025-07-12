#include <Arduino.h>
#include <HTTPUpdate.h>
#define SSID "kientrung"
#define PASSWORD "08062003"
void update_FOTA();
void setup()
{
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}
 
uint32_t updateCounter = 0;
String version = "1.2";
String key = "31b86e70-715d-42fc-9f44-6dfe5ddc43fd";
 
void loop()
{
  Serial.print("Ver: ");
  Serial.println(version);
  delay(500);
  if (WiFi.status() == WL_CONNECTED)
  {
    updateCounter++;
    if (updateCounter > 10)
    {
      updateCounter = 0;
      Serial.println("Check update");
      update_FOTA();
    }
  }
}
 
String getChipId()
{
  String ChipIdHex = String((uint32_t)(ESP.getEfuseMac() >> 32), HEX);
  ChipIdHex += String((uint32_t)ESP.getEfuseMac(), HEX);
  return ChipIdHex;
}
 
void update_FOTA()
{
  String url = "http://otadrive.com/deviceapi/update?";
  url += "k=" + key;
  url += "&v=" + version;
  url += "&s=" + getChipId(); // định danh thiết bị trên Cloud
 
  WiFiClient client;
  httpUpdate.update(client, url, version);
}