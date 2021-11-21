#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define LINE_WIDTH 2

#define HUD_SCALE 1.0f

#define TEXT_SCALE 1.0f

#define Y_FOV 30

#define LANDING_SPEED 145.0f

extern float landing_speed1;
extern float landing_speed2;
extern float landing_weight1;
extern float landing_weight2;
extern float glass_width;
extern float glass_height;
extern float glass_x;
extern float glass_y;
extern int draw_glass;
extern int glass_type;
extern int glass_type2;
extern int th1;

void readConfig();
int getNrOfLines(char* filename);
int getNrOfMasters(char* filename);
int readSerialConfig(char* port);
int readEthernetConfig(char* ip, int* port);
void createSockets();
void createSerialPorts();
void sendConfig();
void sendConfigReset();
void debugLog(const char* fmt, ...);

#endif
