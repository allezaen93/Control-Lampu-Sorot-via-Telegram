/*******************************************************************
 *  An example of how to use a custom reply keyboard markup.       *
 *                                                                 *
 *                                                                 *
 *  written by Brian Lough                                         *
 *******************************************************************/
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Initialize Wifi connection to the router
char ssid[] = "My WiFi";     // your network SSID (name)
char password[] = "12345678"; // your network key

// Initialize Telegram BOT
#define BOTtoken "631314xxx:AAGAxxx-EIjSU03e763xxxxxx"  // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

const int PinLampu1 = D1;
int StatusLampu1 = 1;
const int PinLampu2 = D2;
int StatusLampu2 = 0;
const int LED = D3;

void handleNewMessages(int numNewMessages) {
  Serial.println("Smart Control Merespon Perintah..!");
  Serial.println(String(numNewMessages));
  
  
  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    //bot.sendMessage(chat_id, "SMART CONTROL LAMPU SOROT AKTIF", "");
    if (from_name == "") from_name = "Guest";
    //========================= Start LAMPU 1 =========================
    if (text == "/ON") {
      digitalWrite(PinLampu1, HIGH);   // turn the LED on (HIGH is the voltage level)
      StatusLampu1 = 1;
      bot.sendMessage(chat_id, "Lampu Sorot ON", "");
    }

    if (text == "/OFF") {
      StatusLampu1 = 0;
      digitalWrite(PinLampu1, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Lampu Sorot OFF", "");
    }

    if (text == "/STATUS") {
      if(StatusLampu1){
        bot.sendMessage(chat_id, "Status Lampu Sorot Saat ini ON", "");
      } else {
        bot.sendMessage(chat_id, "Status Lampu Sorot Saat ini OFF", "");
      }
    }
    //========================= End LAMPU 1 =========================
    if (text == "/menu") {
      String keyboardJson = "[[\"/LAMPU_SOROT\",\"/about\"], [\{ \"text\" : \"www.teknologi.or.id\", \"url\" : \"https://teknologi.or.id\" \}]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Silahkan Pilih Menu", "", keyboardJson, true);
    }

    if (text == "/BACK") {
      String keyboardJson = "[[\"/LAMPU_SOROT\",\"/menu\"], [\{ \"text\" : \"www.teknologi.or.id\", \"url\" : \"https://teknologi.or.id\" \}]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Silahkan Pilih Lampu Sorot", "", keyboardJson, true);
    }

    if (text == "/LAMPU_SOROT") {
      String keyboardJson = "[[\"/ON\", \"/OFF\",\"/STATUS\"], [\"/BACK\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Silahkan Pilih Opsi Smart Control", "", keyboardJson, true);
    }
    if (text == "/start") {
      String welcome = "Assalamualaikum Wr. Wb., .\n";
      welcome += "Selamat Datang, Kak " + from_name + ".\n\n";
      welcome += "Ini adalah Applikasi Internet Of Things - untuk Smart Control Lampu Sorot via TELEGRAM, Alat ini sedang aktif di area Asrama Putri PPADN Lantai 3.\n\n";
      //welcome += "/start : Mulai menggunakan Smart Control\n";
      //welcome += "/ledon : to switch the Led ON\n";
      //welcome += "/ledon : to switch the Led ON\n";
      //welcome += "/ledoff : to switch the Led OFF\n";
      //welcome += "/status : Returns current status of LED\n";
      //welcome += "/options : returns the reply keyboard\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
    if (text == "/start") {
      String keyboardJson = "[[\"/menu\"], [\{ \"text\" : \"www.teknologi.or.id\", \"url\" : \"https://teknologi.or.id\" \}]]";
      bot.sendMessageWithReplyKeyboard(chat_id, ":)", "", keyboardJson, true);
    }
    if (text == "/about") {
      String welcome = "\n";
      welcome += "Applikasi Smart Control dibuat oleh: www.teknologi.or.id.\n\n";
      //welcome += "/start : Mulai menggunakan Smart Control\n";
      //welcome += "/ledon : to switch the Led ON\n";
      //welcome += "/ledon : to switch the Led ON\n";
      //welcome += "/ledoff : to switch the Led OFF\n";
      //welcome += "/status : Returns current status of LED\n";
      //welcome += "/options : returns the reply keyboard\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
    if (text == "/about") {
      String keyboardJson = "[[\"/menu\"], [\{ \"text\" : \"www.teknologi.or.id\", \"url\" : \"https://teknologi.or.id\" \}]]";
      bot.sendMessageWithReplyKeyboard(chat_id, ";)", "", keyboardJson, true);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  pinMode(PinLampu1, OUTPUT); // initialize digital PinLampu1 as an output.
  delay(10);
  digitalWrite(PinLampu1, HIGH); // initialize pin as on

  pinMode(PinLampu2, OUTPUT); // initialize digital PinLampu1 as an output.
  delay(10);
  digitalWrite(PinLampu2, LOW); // initialize pin as off

  pinMode(LED, OUTPUT); // initialize digital PinLampu1 as an output.
  delay(10);
  digitalWrite(LED, HIGH); // initialize pin as off
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("Menerima Perintah dari User");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}
