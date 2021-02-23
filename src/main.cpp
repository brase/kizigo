#include "Wrappers.h"
#include "Resources.h"
#include "MQTT.h"
#include "WiFiClient.h"

unsigned long lastcheck = 0;
int SignalStrength = 0;

WiFiClient  net;
MQTTClient client;

void publish(String topic)
{
	while (!client.connect("kizigo", "mqtt", "")) {
    	Serial.print(".");
    	delay(1000);
  	}
	client.publish(topic, "");
	client.disconnect();
}

void doBibiTina()
{
	publish("kizigo/sonos/bibitina");
}

void doArielle()
{
	publish("kizigo/sonos/arielle");
}

void doTinkerbell()
{
	publish("kizigo/sonos/tinkerbell");
}

void doRapunzel()
{
	publish("kizigo/sonos/rapunzel");
}

void doPocahontas()
{
	publish("kizigo/sonos/pocahontas");
}

void doLichterkinder()
{
	publish("kizigo/sonos/lichterkinder");
}

void doHelikopter()
{
	publish("kizigo/sonos/helikopter");
}

void doSchlangen()
{
	publish("kizigo/sonos/schlangen");
}

void messageReceived(String &topic, String &payload) {


  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void setup()
{
	GO.begin(115200);
	preferences.begin("WiFi", false);
	GO.WiFi_Mode = preferences.getInt("mode", 0);
	preferences.end();
	WiFi.mode((wifi_mode_t)GO.WiFi_Mode);
	if (GO.WiFi_Mode > 0)
	{
		WiFi.begin();
	}

	client.begin("192.168.178.122", net);
	client.onMessage(messageReceived);

	preferences.begin("Brightness", false);
	GO.Lcd.setBrightness(preferences.getUShort("light", 95));
	preferences.end();

	//The main menu. Add main menu items here
	GO.addMenuItem(0, "SONOS", "<", "OK", ">", 1, WebRadio, appReturn);
	// GO.addMenuItem(0, "SYSTEM", "<", "OK", ">", 2, System, appReturn);
	// GO.addMenuItem(0, "ABOUT", "<", "OK", ">", -1, About, appAbout);

	GO.addMenuItem(1, "BIBI UND TINA", "<", "OK", ">", -1, bibitina, doBibiTina);
	GO.addMenuItem(1, "ARIELLE", "<", "OK", ">", -1, arielle, doArielle);
	GO.addMenuItem(1, "TINKERBELL", "<", "OK", ">", -1, tinkerbell, doTinkerbell);
	GO.addMenuItem(1, "RAPUNZEL", "<", "OK", ">", -1, rapunzel, doRapunzel);
	GO.addMenuItem(1, "POCAHONTAS", "<", "OK", ">", -1, pocahontas, doPocahontas);
	GO.addMenuItem(1, "LICHTERKINDER", "<", "OK", ">", -1, lichterkinder, doLichterkinder);
	GO.addMenuItem(1, "HELIKOPTER", "<", "OK", ">", -1, helikopter, doHelikopter);
	GO.addMenuItem(1, "SCHLANGEN", "<", "OK", ">", -1, schlangen, doSchlangen);


	// GO.addMenuItem(2, "SYSTEM INFORMATIONS", "<", "OK", ">", -1, Sysinfo, appSysInfo);
	// GO.addMenuItem(2, "WIFI CONNECTION", "<", "OK", ">", -1, WifiConn, appWiFiSetup);
	// GO.addMenuItem(2, "DISPLAY BACKLIGHT", "<", "OK", ">", -1, Backlight, appCfgbrightness);

	GO.show();
}

void loop()
{
	unsigned long now = millis();
	if (now - lastcheck >= 1000)
	{
		GO.Lcd.setTextColor(WHITE, 15);
		GO.Lcd.drawString("Batt: " + String(GO.battery.getPercentage()) + " %", 10, 5, 2);
		GO.WiFi_Mode = WiFi.getMode();
		if (GO.WiFi_Mode == WIFI_MODE_STA && WiFi.isConnected())
		{
			SignalStrength = map(100 + WiFi.RSSI(), 5, 90, 0, 100);
			GO.Lcd.drawRightString("WiFi: " + String(SignalStrength) + " %", 310, 5, 2);
		}
		else if (GO.WiFi_Mode == WIFI_MODE_APSTA)
		{
			GO.Lcd.drawRightString("Clients: " + String(WiFi.softAPgetStationNum()), 300, 5, 2);
		}
		else if (GO.WiFi_Mode == WIFI_MODE_NULL)
		{
			GO.Lcd.drawRightString("Wifi OFF", 310, 5, 2);
		}
		lastcheck = now;
	}
	GO.update();
	if (GO.JOY_Y.wasAxisPressed() == 1)
	{
		GO.up();
	}
	if (GO.JOY_Y.wasAxisPressed() == 2)
	{
		GO.down();
	}
	if (GO.BtnA.wasPressed())
	{
		GO.execute();
	}
	if (GO.BtnB.wasPressed())
	{
		GO.GoToLevel(0);
	}
}