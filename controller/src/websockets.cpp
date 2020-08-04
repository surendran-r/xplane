#include <WebSocketsServer.h>
#include <Arduino.h>
#include "websock.h"

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t lenght);

WebSocketsServer webSocket(81);
int clientId = -1;

void startWebServer()
{
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    Serial.println("WebSocket server started.");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t lenght)
{
    switch (type)
    {
    case WStype_DISCONNECTED: // if the websocket is disconnected
        Serial.printf("[%u] Disconnected!\n", num);
        clientId = -1;
        break;
    case WStype_CONNECTED:
    { // if a new websocket connection is established
        IPAddress ip = webSocket.remoteIP(num);
        clientId = num;
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        Serial.printf("Client Id: %d", clientId);
        webSocket.sendTXT(num, "Success");
        return;

    }
    case WStype_TEXT: // if new text data is received
        Serial.printf("[%u] get Text: %s\n", num, payload);
    }
}

void sendThrottleToWebSockets(char *sensorName, int sensorValue)
{
    if(clientId == -1) {
        return;
    }
    char msg[256];
    sprintf(msg, "%s: %d", sensorName, sensorValue);
    webSocket.sendTXT(clientId, msg);
}

unsigned long last_10sec = 0;
unsigned long pingcounter = 0;
void loopWebSockServer() {
    unsigned long t = millis();
    webSocket.loop();    
    if((t - last_10sec) > 10 * 1000) {
        pingcounter++;
        bool ping = (pingcounter % 2);
        int i = webSocket.connectedClients(ping);
        Serial.printf("%d Connected websocket clients ping: %d\n", i, ping);
        last_10sec = millis();
    }
}