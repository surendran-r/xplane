#ifndef WEB_SOCK_H
#define WEB_SOCK_H

void startWebServer();
void sendThrottleToWebSockets(char* sensorName, int sensorValue);
void loopWebSockServer();
#endif