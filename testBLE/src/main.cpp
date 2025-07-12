#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

bool deviceConnected = false;
bool wifiConnected = false;
bool bleStarted = false;

BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;

const char* serviceUUID = "12345678-1234-5678-1234-56789abcdef0";
const char* charUUID    = "abcdef01-1234-5678-1234-56789abcdef0";

void stopBLE(); 

// ====== Custom Callbacks ======

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override {
    deviceConnected = true;
    Serial.println("[BLE] Client connected");
    // if (pCharacteristic != nullptr) {
    //   pCharacteristic->setValue("BLE_CONNECTED");
    //   pCharacteristic->notify();
    // }
  }

  void onDisconnect(BLEServer* pServer) override {
    deviceConnected = false;
    Serial.println("[BLE] Client disconnected");
  }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) override {
    std::string value = pCharacteristic->getValue();
    String wifiInfo = String(value.c_str());

    Serial.print("[BLE] Data received: ");
    Serial.println(wifiInfo);

    int ssidStart = wifiInfo.indexOf("SSID:") + 5;
    int ssidEnd = wifiInfo.indexOf(";PASS:");
    int passStart = wifiInfo.indexOf("PASS:") + 5;

    if (ssidStart > 4 && ssidEnd > ssidStart && passStart > 4) {
      String ssid = wifiInfo.substring(ssidStart, ssidEnd);
      String pass = wifiInfo.substring(passStart);

      Serial.print("[WiFi] SSID: "); Serial.println(ssid);
      Serial.print("[WiFi] PASSWORD: "); Serial.println(pass);

      WiFi.begin(ssid.c_str(), pass.c_str());
      Serial.print("[WiFi] Connecting to: "); Serial.println(ssid);

      unsigned long startAttempt = millis();
      while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 15000) {
        delay(500);
        Serial.print(".");
      }

      if (WiFi.status() == WL_CONNECTED) {
        Serial.print("\n[WiFi] Connected. IP: ");
        Serial.println(WiFi.localIP());
        
        // pCharacteristic->setValue("");
        pCharacteristic->setValue("WIFI_OK");
        pCharacteristic->notify();

        wifiConnected = true;
        stopBLE();
      } else {
        pCharacteristic->setValue("");
        Serial.println("\n[WiFi] Connection failed.");
        pCharacteristic->setValue("WIFI_FAIL");
        pCharacteristic->notify();
        wifiConnected = false;
      }
    } else {
      Serial.println("[BLE] Invalid format. Expected SSID:xxx;PASS:xxx.");
    }
  }
};

// ====== BLE Functions ======

void startBLE() {
  if (bleStarted) return;

  Serial.println("[BLE] Starting BLE...");
  BLEDevice::init("ESP32-BLE-WiFi");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService* pService = pServer->createService(serviceUUID);
  pCharacteristic = pService->createCharacteristic(charUUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY  );
  pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  pService->start();
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();
  bleStarted = true;
}

void stopBLE() {
  if (!bleStarted) return;
  Serial.println("[BLE] Stopping BLE...");
  BLEDevice::getAdvertising()->stop();
  pServer->disconnect(0); // Ngắt client
  bleStarted = false;
}

// ====== Setup / Loop ======

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  startBLE();  // bắt đầu bằng BLE
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    delay(1000);
    return;
  }

  // Nếu mất WiFi mà BLE chưa bật lại
  if (!wifiConnected || WiFi.status() != WL_CONNECTED) {
    if (!bleStarted) {
      Serial.println("[WiFi] Lost or not connected. Restarting BLE...");
      startBLE();
    }
  }

  delay(1000);
}
