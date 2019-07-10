/*---------
  Code: Giftia, QQ: 1005056803, WeChat: Giftia, Website: https://giftia.moe
  禁止商用
  ---------*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SSD1306.h>

SSD1306 display(0x3c, SDA, SCL, OLED_RST, GEOMETRY_128_32);

const char* ssid = "";
const char* password = "";
const char* host = "esp8266";
IPAddress ip;

ESP8266WebServer server(80);

void sendHtml() {
  server.send(200, "text/html", "<!DOCTYPE html><html lang='zh-cn'><head><link rel='icon' href='data:image/ico;base64,aWNv'><meta http-equiv='X-UA-Compatible' messages='IE=edge,chrome=1'><meta name='viewport' content='width=339,minimum-scale=0.5,maximum-scale=1.0,user-scalable=no, initial-scale=1.0' /><meta charset='utf-8'><meta name='viewport' messages='minimal-ui'><title>新风控制</title><style>* {margin: 0;list-style: none;box-sizing: border-box;font-family: '思源黑体';}body {width: 100%;height: 100%; background: linear-gradient(to top, rgba(12, 12, 12, 255), rgba(35, 35, 35, 255));background-repeat: no-repeat;}.content {font-size: 150%;overflow:auto;padding: 10px;text-align: center;}</style><body><div style='width: 100%; overflow: auto; height: 100%; text-align: center;'><img src='https://i.loli.net/2019/02/18/5c6a687838973.jpg' usemap='#map'border='0' style='width: 339px; overflow: hidden; height: 605px; text-align: center;'><map id='map' name='map'><area target='_blank' shape='rect' coords='17.5,163,57,204' href='/OFF'style='outline: none;'><area target='_blank' shape='rect' coords='125.5,247,213,341' href='/loop'style='outline: none;'><area target='_blank' shape='rect' coords='103,176,238,229' href='/+'style='outline: none;'><area target='_blank' shape='rect' coords='55.5,239,108,349' href='/auto'style='outline: none;'><area target='_blank' shape='rect' coords='230,240,284,350' href='/wind'style='outline: none;'><area target='_blank' shape='rect' coords='98.5,357,240,409' href='/-'style='outline: none;'><area target='_blank' shape='rect' coords='70.5,457,108,499' href='/time'style='outline: none;'><area target='_blank' shape='rect' coords='149.5,456,190,499' href='/reload'style='outline: none;'><area target='_blank' shape='rect' coords='228.5,455,268,499' href='/plasma'style='outline: none;'></map><div class='content'><a href='/wifiset' style='text-decoration: none; color:white;'>点此配置WiFi</a></div></div></body></html>");
}

void sendHtml_off() {
  server.send(200, "text/html", "<!DOCTYPE html><html lang='zh-cn'><head><link rel='icon' href='data:image/ico;base64,aWNv'><meta http-equiv='X-UA-Compatible' messages='IE=edge,chrome=1'><meta name='viewport' content='width=339,minimum-scale=0.5,maximum-scale=1.0,user-scalable=no, initial-scale=1.0' /><meta charset='utf-8'><meta name='viewport' messages='minimal-ui'><title>新风控制</title><style>* {margin: 0;list-style: none;box-sizing: border-box;font-family: '思源黑体';}body {width: 100%;height: 100%; background: linear-gradient(to top, rgba(12, 12, 12, 255), rgba(35, 35, 35, 255));background-repeat: no-repeat;}.content {font-size: 150%;overflow:auto;padding: 10px;text-align: center;}</style><body><div style='width: 100%; overflow: auto; height: 100%; text-align: center;'><img src='https://i.loli.net/2019/02/18/5c6a687838973.jpg' usemap='#map'border='0' style='width: 339px; overflow: hidden; height: 605px; text-align: center;'><map id='map' name='map'><area target='_blank' shape='rect' coords='17.5,163,57,204' href='/ON'style='outline: none;'><area target='_blank' shape='rect' coords='125.5,247,213,341' href='/loop'style='outline: none;'><area target='_blank' shape='rect' coords='103,176,238,229' href='/+'style='outline: none;'><area target='_blank' shape='rect' coords='55.5,239,108,349' href='/auto'style='outline: none;'><area target='_blank' shape='rect' coords='230,240,284,350' href='/wind'style='outline: none;'><area target='_blank' shape='rect' coords='98.5,357,240,409' href='/-'style='outline: none;'><area target='_blank' shape='rect' coords='70.5,457,108,499' href='/time'style='outline: none;'><area target='_blank' shape='rect' coords='149.5,456,190,499' href='/reload'style='outline: none;'><area target='_blank' shape='rect' coords='228.5,455,268,499' href='/plasma'style='outline: none;'></map><div class='content'><a href='/wifiset' style='text-decoration: none; color:white;'>点此配置WiFi</a></div></div></body></html>");
}

void handleRoot() {
  display.clear();
  display.drawString(0, 0, "Welcome to NeWindCIS");
  display.display();
  sendHtml_off();
}

void handleNotFound() {
  String message = "<!DOCTYPE HTML><html lang='zh-cn'><head><meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge,chrome=1'><meta name='viewport' content='width=device-width, initial-scale=1'><title>新风控制</title></head><body>404 not found, 未找到您所访问的资源\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n</body></html>";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/html", message);
}

void handleSetWifi() {
  if (server.hasArg("DISCONNECT")) {
    display.clear();
    display.drawString(0, 0, "Client DISCONNECT");
    display.display();
    String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=0\r\nLocation: /wifiinfo\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  if (server.hasArg("WIFINAME") && server.hasArg("WIFIPWD")) {
    String wifissid = server.arg("WIFINAME");
    String wifipwd = server.arg("WIFIPWD");
    String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=1\r\nLocation: /wifiinfo\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    if (WiFi.isConnected())
    {
      WiFi.disconnect();
    }
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifissid.c_str(), wifipwd.c_str());
    byte count = 0;
    while (WiFi.status() != WL_CONNECTED && count < 15)
    {
      count ++;
      delay(500);
      display.clear();
      display.drawString(0, 0, "Connecting...");
      display.display();
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      display.drawString(0, 0, "Connecting...Success.");
      display.display();
    }
    else
    {
      display.drawString(0, 0, "Connecting...Failed.");
      display.drawString(0, 16, "Auto Retry.");
      display.display();
      Serial.write(0);
      while (1);
    }
    Serial.write(255);
    delay(300);
    ip = WiFi.localIP();
    int buf[3];
    for (int i = 0; i < 4; i++)
    {
      buf[i] = ip[i];
    }
    String temp = "IP:";
    temp += String(buf[0]);
    temp += '.';
    temp += String(buf[1]);
    temp += '.';
    temp += String(buf[2]);
    temp += '.';
    temp += String(buf[3]);
    display.clear();
    display.drawString(0, 0, temp);
    display.drawString(0, 16, "WIFI OK.");
    display.display();

    return;
  }
  display.clear();
  display.drawString(0, 0, "IP:192.168.233.233");
  display.drawString(0, 16, "Now Enter WiFi Setup");
  display.display();
  String content = "<!DOCTYPE html><html lang='zh-cn'><head><link rel='icon' href='data:image/ico;base64,aWNv'><meta http-equiv='X-UA-Compatible' messages='IE=edge,chrome=1'><meta charset='utf-8'><meta name='viewport' messages='minimal-ui'><title>新风控制</title><style>* {margin: 0;list-style: none;box-sizing: border-box;font-family: '思源黑体';}body {width: 100%;height: 100%;background: #eee;margin-bottom: 100px;}.header {background: #333;width: 100%;height: 100px;color: #fff;line-height: 34px;font-size: 300%;padding: 3% 5%;}.content {font-size: 300%;width: 100%;height: 100%;overflow: auto;padding: 5px;text-align: center;}</style></head><body><div class='header'><span style='float: left;'>新风控制——物联网系统配置</span></div><div style='border-radius: 25px; border: 5px solid #555; padding: 20px; width: auto; height: auto;'><form method='POST'><h1 class='content'>WiFi配置</h1><h1>Wifi名:<input type='text' name='WIFINAME' placeholder='请输入家中的WiFi名' class='content'><br>Wifi密码:<input type='password' name='WIFIPWD' placeholder='请输入WiFi密码' class='content'><br><input type='submit' name='SUBMIT' value='提交' style='width:100%;background-color:#4CAF50;border:none;color:white;padding:15px 32px;text-align:center;text-decoration:none;display:inline-block;font-size:100px;border-radius:20px;margin:auto;' onclick='javascript:window.location.href='/ON''></form></h1></div></body></html>";
  server.send(200, "text/html", content);
}

void setup(void) {
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  IPAddress softLocal(192, 168, 233, 233);
  IPAddress softGateway(192, 168, 233, 1);
  IPAddress softSubnet(255, 255, 255, 0);
  WiFi.softAPConfig(softLocal, softGateway, softSubnet);
  WiFi.softAP("新风控制IoT交付_未授权", "13867521227");
  display.clear();
  display.drawString(0, 0, "IP:192.168.233.233");
  display.drawString(0, 16, "NeWind CIoT System STBY");
  display.display();

  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  //默认上电启动


  if (MDNS.begin(host)) {
    MDNS.addService("http", "tcp", 80);
  }

  server.on("/", handleRoot);

  server.on("/ON", []() {
    sendHtml();
    digitalWrite(LED_BUILTIN, HIGH);
    display.clear();
    display.drawString(0, 16, "Power ON");
    display.display();
  });

  server.on("/OFF", []() {
    sendHtml_off();
    digitalWrite(LED_BUILTIN, LOW);
    display.clear();
    display.drawString(0, 16, "Power OFF");
    display.display();
  });

  server.on("/loop", []() {
    if (server.hasArg("outdoor_time") && server.hasArg("indoor_time") && server.hasArg("pause_time")) {
      String ot = server.arg("outdoor_time");
      String it = server.arg("indoor_time");
      String pt = server.arg("pause_time");
      display.drawString(0, 0, "ot:" + ot + ",it:" + it + ",pt:" + pt);
      display.display();
      Serial.write(ot.toInt());
      Serial.write(it.toInt());
      Serial.write(pt.toInt());
      return;
    }
    server.send(200, "text/html", "<!DOCTYPE html><html lang='zh-cn'><head><link rel='icon' href='data:image/ico;base64,aWNv'><meta http-equiv='X-UA-Compatible' messages='IE=edge,chrome=1'><meta charset='utf-8'><meta name='viewport' messages='minimal-ui'><title>新风控制</title><style>* {margin: 0;list-style: none;box-sizing: border-box;font-family: '思源黑体';}body {width: 100%;height: 100%;background: #eee;margin-bottom: 100px;}.header {background: #333;width: 100%;height: 100px;color: #fff;line-height: 34px;font-size: 300%;padding: 3% 5%;}.content {font-size: 300%;width: 100%;height: 100%;overflow: auto;padding: 5px;text-align: center;}</style></head><body><div class='header'><span style='float: left;'>新风控制——循环配置</span></div><div style='border-radius: 25px; border: 5px solid #555; padding: 20px; width: auto; height: auto;'><a class='content'><form method='POST'><div>室外循坏: <input type='time' name='outdoor_time' style='font-size: 80%;text-align: center;'/></div><br><div>室内循环: <input type='time' name='indoor_time' style='font-size: 80%;text-align: center;'/></div><br><div>暂停循环: <input type='time' name='pause_time' style='font-size: 80%;text-align: center;'/></div><br><input type='submit' name='SUBMIT' value='提交' style='width:100%;background-color:#4CAF50;border:none;color:white;padding:15px 32px;text-align:center;text-decoration:none;display:inline-block;font-size:100px;border-radius:20px;margin:auto;'/></form></a></div></body></html>");
    display.clear();
    display.drawString(0, 16, "loop setting");
    display.display();
  });

  server.on("/+", []() {
    sendHtml();
    display.clear();
    display.drawString(0, 16, "+");
    display.display();
    Serial.write(255);
  });

  server.on("/-", []() {
    sendHtml();
    display.clear();
    display.drawString(0, 16, "-");
    display.display();
    Serial.write(255);
  });

  server.on("/auto", []() {
    sendHtml();
    display.clear();
    display.drawString(0, 16, "auto");
    display.display();
    Serial.write(255);
  });

  server.on("/wind", []() {
    sendHtml();
    display.clear();
    display.drawString(0, 16, "wind");
    display.display();
    Serial.write(1);
  });

  server.on("/time", []() {
    sendHtml();
    display.clear();
    display.drawString(0, 16, "time");
    display.display();
    Serial.write(255);
  });

  server.on("/reload", []() {
    sendHtml();
    display.clear();
    display.drawString(0, 16, "reload");
    display.display();
    Serial.write(3);
  });

  server.on("/plasma", []() {
    sendHtml();
    display.clear();
    display.drawString(0, 16, "plasma");
    display.display();
    Serial.write(2);
  });

  server.onNotFound(handleNotFound);
  server.on("/wifiset", HTTP_GET, handleSetWifi);
  server.on("/wifiset", HTTP_POST, handleSetWifi);
  server.begin();
}

void loop(void) {
  server.handleClient();
}
