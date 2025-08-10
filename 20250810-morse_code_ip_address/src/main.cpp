#include <M5Unified.h>
#include <Adafruit_NeoPixel.h>
#include <ESP32SerialWiFiSetup.h>

Esp32SerialWifiSetup::WiFiSetupManager wifiSetup;
Adafruit_NeoPixel led(1, G27, NEO_GRB + NEO_KHZ800);

void setup()
{
  M5.begin();

  led.begin();

  // Initialize the WiFi setup manager
  wifiSetup.begin();

  Serial.println("ESP32 Serial WiFi Setup - JSON-RPC Interface");
  Serial.println("============================================");
  Serial.println("Send JSON-RPC commands to configure WiFi:");
  Serial.println();
  Serial.println("Set WiFi credentials:");
  Serial.println("{\"jsonrpc\": \"2.0\", \"method\": \"set_wifi_creds\", \"params\": {\"ssid\": \"your_ssid\", \"pass\": \"your_password\"}, \"id\":1}");
  Serial.println();
  Serial.println("Get WiFi credentials:");
  Serial.println("{\"jsonrpc\": \"2.0\", \"method\": \"get_wifi_creds\", \"id\":1}");
  Serial.println();
  Serial.println("Get IP address:");
  Serial.println("{\"jsonrpc\": \"2.0\", \"method\": \"get_ip\", \"id\":1}");
  Serial.println();
  Serial.println("Get MAC address:");
  Serial.println("{\"jsonrpc\": \"2.0\", \"method\": \"get_mac_address\", \"id\":1}");
  Serial.println();
}

uint8_t local_ip = {0};
uint8_t ip_text[32] = {0};
uint8_t ip_text_len = 0;
void morse_code_ip_address_init(unsigned long now_ms)
{
  uint32_t ip_u32 = uint32_t(WiFi.localIP());
  local_ip = (ip_u32 >> 24) & 0xFF;

  if(local_ip >= 100) {
    ip_text[0] = '1';
    ip_text[1] = '0' + (local_ip / 10) % 10;
    ip_text[2] = '0' + local_ip % 10;
    ip_text_len = 3;
  }else if(local_ip >= 10) {
    ip_text[0] = '0' + (local_ip / 10) % 10;
    ip_text[1] = '0' + local_ip % 10;
    ip_text_len = 2;
  }else{
    ip_text[0] = '0' + local_ip;
    ip_text_len = 1;
  }
}

void led_on()
{
  led.setPixelColor(0, led.Color(255, 255, 255));
  led.show();
}

void led_off()
{
  led.setPixelColor(0, led.Color(0, 0, 0));
  led.show();
}

void morse_code_ip_address_loop(unsigned long now_ms)
{
}

uint32_t last_led_time = 0;

void loop()
{
  M5.delay(1);
  M5.update();
  unsigned long now = millis();


  if (M5.BtnA.wasReleased())
  {
    morse_code_ip_address_init(now);
    led_on();
  }
  morse_code_ip_address_loop(now);

  // Handle JSON-RPC commands from serial port
  wifiSetup.handleSerialCommands();

  // Optional: Print connection status every 30 seconds
  static unsigned long lastStatusPrint = 0;
  if (now - lastStatusPrint > 30000)
  {
    lastStatusPrint = millis();

    if (wifiSetup.isConnected())
    {
      Serial.print("Status: Connected to WiFi - IP: ");
      Serial.println(wifiSetup.getIPAddress());
    }
    else
    {
      Serial.println("Status: Not connected to WiFi");
    }
  }
}