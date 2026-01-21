#include "robotka.h"
#include "ulohy.h"
#include <Wire.h>
#include <Adafruit_VL53L0X.h>

byte Bbutton1 = 35;
byte Bbutton2 = 27;



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


    pinMode(14, PULLUP);
    pinMode(26, PULLUP);
    // Spust I2C sbernice
    Wire1.begin(14, 26, 100000);
    delay(100);
    Wire1.setTimeOut(1);
    
    //Inicializuj senzor
    rk_laser_init("laser", Wire1, lox, 0, 0x30);

    
    printf("Starting main loop\n");
    //start tlacitko pro kalibraci klepet

    rkLedBlue(false);
    rkLedGreen(false);
    rkLedYellow(false);
    rkLedRed(false);
    test_batery();
    
    zavrit_klepeto();

    //Ovladani testu
    float r, g, b;
    while(true){
        if(rkButtonOn())    { forward_acc(1000, 50); }
        if(rkButtonLeft())  { turn_on_spot_left(180, 50); }
        if(rkButtonRight()) { turn_on_spot_right(180, 50); }
        if(rkButtonDown())  { radius_left(200, 90, 50); } 
        if(rkButtonUp())    { radius_right(200, 90, 50); }
        if(digitalRead(Bbutton1) == LOW) { vysun_zhazovadlo(); }
        if(digitalRead(Bbutton2) == LOW) { zasun_zhazovadlo(); }
        if (rkColorSensorGetRGB("front", &r, &g, &b)) {
            Serial.print("R: "); Serial.print(r, 3);
            Serial.print(" G: "); Serial.print(g, 3);
            Serial.print(" B: "); Serial.println(b, 3);
        } else { Serial.println("Sensor 'front' not found."); }
        int distance = rk_laser_measure("laser");
        Serial.print("Laser distance: "); Serial.println(distance);
        delay(100);
    }

    //Testy
    //forward_acc(100, 50);
    // rkServosSetPosition(4, 0);         //up
    // delay(5000);
    // rkServosSetPosition(4, -90);        //down
    // rkServosSetPosition(3, 90);     //opened 
    // delay(5000);
    //rkServosSetPosition(3, 0);    //closed
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

void setup() { configurating();
    rkLedBlue(false);
    rkLedGreen(false);
    rkLedYellow(false);
    rkLedRed(false); }

void loop() {
    // Serial.print("PRAVY: "); Serial.println(rkIrLeft());// 1 je ten v pravo....
    // Serial.print("LEVY: "); Serial.println(rkIrRight());// kdyz je to nizke tak cerna
    // Serial.println();
    // delay(100);


    

    Serial.print("starting loop\n");

    switch(getPressed()) {
        case UP_VYHREJ:
            rkLedGreen(true); // Zelená pro výhru
            delay(10000);
            // Kombinace úkolů pro "vyhrej"
            sprint_m_d(); 
            rkBuzzerSet(true);
            delay(200);
            rkBuzzerSet(false);
            delay(10000);
            slalom(true); 
            delay(10000);
            bludiste();
            //srovnej_se_v_pravo();
            turn_on_spot_left(90, 50);
            delay(100);
            back_buttons(40);
            delay(100);
            forward_acc(od_steny_na_stred_pole, 50);
            delay(10000);
            medved();
            delay(10000);
            kulicky();
            break;

        case OFF_MEDVED:
            rkLedRed(true); // Červená pro medvěda
            delay(10000);
            medved();
            break;

        case DOWN_KULICKY:
            rkLedBlue(true); // Modrá pro kuličky
            delay(10000);
            kulicky();
            break;
            
        case RIGHT_BLUDISTE:
            rkLedYellow(true); // Žlutá pro bludiště
            delay(10000);
            bludiste();
            break;
            
        case LEFT_SLALOM:
            rkLedRed(true);
            rkLedYellow(true); // Oranžová pro slalom
            delay(10000);
            slalom(true);
            
            break;
            
        case ON_SPRINT:
            rkLedBlue(true);
            rkLedRed(true); // Fialová pro sprint
            delay(10000);
            sprint_m_d();
            // while(true){
            //     delay(1000);
            //     Serial.print("IR levý: "); Serial.println(rkIrRight());
            //     Serial.print("IR pravý: "); Serial.println(rkIrLeft());

            // }
            break;
            
        case BUTTON1_KOMBINACE1:
            rkLedGreen(true);
            rkLedYellow(true); // Zeleno-žlutá pro kombinaci 1
            delay(10000);
            
            break;
            
        case BUTTON2_KOMBINACE2:
            // rkLedRed(true);
            // rkLedGreen(true);
            // rkLedBlue(true);
            rkLedYellow(true); // Bílá (všechny barvy) pro kombinaci 2
            delay(10000);
            //srovnej_na_caru();
            break;
        
        case NONE:
            break;
    }
    delay(100);
}
