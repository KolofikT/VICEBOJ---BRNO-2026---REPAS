#include "robotka.h"
#include "ulohy.h"
#include <Wire.h>
#include <Adafruit_VL53L0X.h>


byte Bbutton1 = 27;
byte Bbutton2 = 35;
 


// deklarace instance senzoru
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);


void trap() {
    Serial.println("trap\n");
    while (1)
        ;
}
void test_batery(){
    printf("batery percent: %u\n", rkBatteryPercent());
    printf("batery voltage: %u\n", rkBatteryVoltageMv()/1000);
}
void configurating(){
    Serial.begin(115200);
    rkConfig cfg;
    rkSetup(cfg);

    pinMode(21, PULLUP);
    pinMode(22, PULLUP);
    // Spust I2C sbernice
    Wire.begin(21, 22, 400000);
    delay(100);
    Wire.setTimeOut(1);

    // Initialize the color sensor with a unique name and the I2C bus
    rkColorSensorInit("front", Wire, tcs);


    // pinMode(14, PULLUP);
    // pinMode(26, PULLUP);
    // // Spust I2C sbernice
    // Wire1.begin(14, 26, 100000);
    // delay(100);
    // Wire1.setTimeOut(1);
    
    // //Inicializuj senzor
    // rk_laser_init("laser", Wire1, lox, 0, 0x30);

    
    printf("Starting main loop\n");
    //start tlacitko pro kalibraci klepet

    rkLedBlue(false);
    rkLedGreen(false);
    rkLedYellow(false);
    rkLedRed(false);
    test_batery();
    
    zavrit_klepeto();
    zasun_zhazovadlo();

}   

enum RobotButton {
    NONE,
    UP_VYHREJ,
    OFF_MEDVED,
    DOWN_KULICKY,
    RIGHT_BLUDISTE,
    LEFT_SLALOM,
    ON_SPRINT,
    BUTTON1_KOMBINACE1,
    BUTTON2_KOMBINACE2
};

RobotButton getPressed() {
    if (rkButtonUp()) return UP_VYHREJ;
    if (rkButtonOff()) return OFF_MEDVED;
    if (rkButtonDown()) return DOWN_KULICKY;
    if (rkButtonRight()) return RIGHT_BLUDISTE;
    if (rkButtonLeft()) return LEFT_SLALOM;
    if (rkButtonOn()) return ON_SPRINT;
    if (digitalRead(Bbutton1) == LOW) return BUTTON1_KOMBINACE1;
    if (digitalRead(Bbutton2) == LOW) return BUTTON2_KOMBINACE2;
    return NONE;
}



void setup() {
    configurating();
}

void loop() {
    rkLedBlue(false);
    rkLedGreen(false);
    rkLedYellow(false);
    rkLedRed(false);

    Serial.print("ooooo");

    switch(getPressed()) {
        case UP_VYHREJ:
            rkLedGreen(true); // Zelená pro výhru
            // delay(10000);
            // // Kombinace úkolů pro "vyhrej"
            // sprint_m_d(); 
            // rkBuzzerSet(true);
            // delay(200);
            // rkBuzzerSet(false);
            // delay(10000);
            // slalom(true); 
            // delay(10000);
            // bludiste();
            // srovnej_se_v_pravo();
            // turn_on_spot_left(90, 50);
            // delay(100);
            // back_buttons(40);
            // delay(100);
            // forward_acc(od_steny_na_stred_pole, 50);
            // delay(10000);
            // medved();
            // delay(10000);
            // kulicky();
            break;

        case OFF_MEDVED:
            rkLedRed(true); // Červená pro medvěda
            delay(1000);
            medved();
            break;

        case DOWN_KULICKY:
            rkLedBlue(true); // Modrá pro kuličky
            delay(1000);
            kulicky();
            break;
            
        case RIGHT_BLUDISTE:
            rkLedYellow(true); // Žlutá pro bludiště
            delay(1000);
            bludiste();
            break;
            
        case LEFT_SLALOM:
            rkLedRed(true);
            rkLedYellow(true); // Oranžová pro slalom
            delay(1000);
            slalom(true);
            
            break;
            
        case ON_SPRINT:
            rkLedBlue(true);
            rkLedRed(true); // Fialová pro sprint
            delay(1000);
            sprint_m_d();        

            break;
            
        case BUTTON1_KOMBINACE1:
            rkLedGreen(true);
            rkLedYellow(true); // Zeleno-žlutá pro kombinaci 1
            delay(1000);
            Serial.print("ssssssssssss");

            
            break;
            
        case BUTTON2_KOMBINACE2:
            Serial.print("fffffffffff");
            rkLedRed(true);
            rkLedGreen(true);
            rkLedBlue(true);
            rkLedYellow(true); // Bílá (všechny barvy) pro kombinaci 2
            delay(1000);
            rkServosSetPosition(3, 90);
            //forward_acc(od_steny_na_stred_pole, 50);
            delay(100);
            break;
        
        case NONE:
            break;
    }
    delay(100);
}