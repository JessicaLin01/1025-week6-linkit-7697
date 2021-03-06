/*
  WiFi Web Server LED Blink

  A simple web server that lets you blink an LED via the web.
  This sketch will print the IP address of your WiFi Shield (once connected)
  to the Serial monitor. From there, you can open that address in a web browser
  to turn on and off the LED on pin 9.

  If the IP address of your shield is yourAddress:
  http://yourAddress/H turns the LED on
  http://yourAddress/L turns it off

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the Wifi.begin() call accordingly.

  Circuit:
  * LinkIt 7697 HDK
  * LED attached to pin 9

  created 25 Nov 2012
  by Tom Igoe
  modified 23 May 2017
  by MediaTek Labs
*/
#include <LWiFi.h>

char ssid[] = "IOTCourse";      //  your network SSID (name)
char pass[] = " ";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);
String webSite;
void buildWeb(){
  webSite += "<html>";
  webSite +="<head>\n";
  webSite +="<meta charset='UTF-8/'>\n";
  webSite +="<title>Remote Control LED</title>\n";
  webSite +="<style>";
  webSite +="html{background-color:#e5e9dc;}";
  webSite +=".title{text-align:center;color:darkblue;}";
  webSite +="a{display:block;text-decoration:none;background-color:lightgray;";
  webSite +="width:150px;text-align:center;margin:0 auto;margin-bottom:30px;font-size:2em;}";
  webSite +=".btn1{background-color:black;color:white;}";
  webSite +=".btn2{background-color:white;color:black;}";
  webSite +="</style>";
  webSite +="</head>\n";
  webSite +="<body>";
  webSite +="<h1 class=\"title\">Remote LED Control</h1>";
  webSite +="<a class=\"btn1\" href=\"/H\">ON</a>";
  webSite +="<a class=\"btn2\" href=\"/L\">OFF</a>";
  webSite +="</body></html>";
  }
void setup() {
	Serial.begin(9600);      // initialize serial communication
	pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode
  buildWeb();
	// attempt to connect to Wifi network:
	while (status != WL_CONNECTED) {
		Serial.print("Attempting to connect to Network named: ");
		Serial.println(ssid);                   // print the network name (SSID);

		// Connect to WPA/WPA2 network. Change this line if using open or WEP network:
		status = WiFi.begin(ssid, pass);
	}
	server.begin();                           // start the web server on port 80
	printWifiStatus();                        // you're connected now, so print out the status
}


void loop() {
	WiFiClient client = server.available();   // listen for incoming clients

	if (client) {                             // if you get a client,
		Serial.println("new client");           // print a message out the serial port
		String currentLine = "";                // make a String to hold incoming data from the client
		while (client.connected()) {            // loop while the client's connected
			if (client.available()) {             // if there's bytes to read from the client,
				char c = client.read();             // read a byte, then
				Serial.write(c);                    // print it out the serial monitor
				if (c == '\n') {                    // if the byte is a newline character

					// if the current line is blank, you got two newline characters in a row.
					// that's the end of the client HTTP request, so send a response:
					if (currentLine.length() == 0) {
						// HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
						// and a content-type so the client knows what's coming, then a blank line:
						client.println("HTTP/1.1 200 OK");
						client.println("Content-type:text/html");
						client.println();

						// the content of the HTTP response follows the header:
						client.print(webSite);
						// The HTTP response ends with another blank line:
						client.println();
						// break out of the while loop:
						break;
					} else {    // if you got a newline, then clear currentLine:
						currentLine = "";
					}
				} else if (c != '\r') {  // if you got anything else but a carriage return character,
					currentLine += c;      // add it to the end of the currentLine
				}

				// Check to see if the client request was "GET /H" or "GET /L":
				if (currentLine.startsWith("GET /H")) {
					digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
				}
				if (currentLine.startsWith("GET /L")) {
					digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
				}
			}
		}
		// close the connection:
		client.stop();
		Serial.println("client disonnected");
	}
}

void printWifiStatus() {
	// print the SSID of the network you're attached to:
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());

	// print your WiFi shield's IP address:
	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);

	// print the received signal strength:
	long rssi = WiFi.RSSI();
	Serial.print("signal strength (RSSI):");
	Serial.print(rssi);
	Serial.println(" dBm");
	// print where to go in a browser:
	Serial.print("To see this page in action, open a browser to http://");
	Serial.println(ip);
}
