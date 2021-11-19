#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED



void readConfig();
int getNrOfLines(char* filename);
int getNrOfMasters(char* filename);
int readSerialConfig(char* port);
int readEthernetConfig(char* ip, int* port);
void createSockets();
void createSerialPorts();
void sendConfig();
void sendConfigReset();

#endif
