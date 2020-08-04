#include "xplane.h"
#include <WiFiUdp.h>

#define XPLANE_SERVER_IP "192.168.0.112"
#define XPLANE_SERVER_PORT 49010

void sendPacket(char *udpacket, int len) {
  WiFiUDP Udp;
  Udp.beginPacket(XPLANE_SERVER_IP, XPLANE_SERVER_PORT);
  Udp.write(udpacket, len);
  Udp.endPacket();
}

void sendCMND(char *cmd) {
  char drefPacket[509] = "CMND0";
  int i = 0;
  int currIdx = 5;
  while(cmd[i] != 0) {
    drefPacket[currIdx++] = cmd[i++];
  }
  sendPacket(drefPacket, currIdx+1);
}

///Now the direct x-plane stuff
void sendDREF(char *dref, float value) {
  union converter {
    float data;
    char bytes[4];
  } conv;
  char drefPacket[509] = "DREF0";

  conv.data = value;
  int i = 0;
  int currIdx = 5;
  for(i=0;i<4;i++) {
    drefPacket[currIdx++] = conv.bytes[i]; 
  }
  i = 0;
  while(dref[i] != 0) {
    drefPacket[currIdx++] = dref[i++];
  }
  drefPacket[currIdx++] = 0;
  //Now the final space fillers
  while(currIdx < 509) {
    drefPacket[currIdx++] = ' ';
  }
  sendPacket(drefPacket, 509);
}
