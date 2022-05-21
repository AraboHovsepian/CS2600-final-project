#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

#define PIN_LED 12
#define SDA 33
#define SCL 32

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Wifi
const char *ssid = "";
const char *password = "";

// MQTT Broker
const char *mqtt_broker = "192.168.1.123";
const char *topicSend = "esp32/send";
const char *topicReceive = "esp32/Receive";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(PIN_LED, OUTPUT);

  Wire.begin(SDA, SCL);
  lcd.init();
  lcd.backlight();
  lcd.noCursor();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    lcd.setCursor(0, 0);
    lcd.print("WiFi connecting");
    delay(1000);
  }
  lcd.clear();
  lcd.print("Wifi connected");
  delay(1000);

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected())
  {
    String client_id = "esp32-client-";
    if (client.connect(client_id.c_str()))
    {
      lcd.clear();
      lcd.print("MQTT connected");
      delay(1000);
    }
    else
    {
      lcd.clear();
      lcd.print("Failed");
      lcd.setCursor(0, 1);
      lcd.print("State: ");
      lcd.print(client.state());
      delay(1000);
    }
  }
  client.subscribe(topicReceive);

  lcd.clear();
  lcd.print("Ready!");
}

void callback(char *topic, byte *payload, unsigned int length)
{
  String str = "";
  lcd.setCursor(0, 0);
  lcd.print("Message:");
  for (int i = 0; i < length; i++)
  {
    str += (char)payload[i];
  }
  lcd.print(str);
  if (str == "1")
  {
    digitalWrite(PIN_LED, HIGH);
    client.publish(topicSend, "LED ON");
    lcd.setCursor(0, 1);
    lcd.print("LED ON");
    delay(1000);
    lcd.clear();
  }
  else if (str == "0")
  {
    digitalWrite(PIN_LED, LOW);
    client.publish(topicSend, "LED OFF");
    lcd.setCursor(0, 1);
    lcd.print("LED OFF");
    delay(1000);
    lcd.clear();
  }
}

void loop()
{
  client.loop();
}
