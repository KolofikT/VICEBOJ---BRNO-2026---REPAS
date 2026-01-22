#include "robotka.h"
#include "ulohy.h"
#include <Wire.h>
#include <Adafruit_VL53L0X.h>


byte Bbutton1 = 27;
byte Bbutton2 = 35;
 
int DelayOnRed = 1000;

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

    switch(getPressed()) {
        case UP_VYHREJ:     //Provede VŠECHNY DISCIPLÍNY

                // Zelená pro Všechny disciplíny
                rkLedGreen(true); 
                delay(DelayOnRed);

        // Kombinace úkolů pro "vyhrej"
            
            //Provedení Sprintu
            sprint_m_d(); 

                //Ohlášení dokončení disciplíny
                rkBuzzerSet(true);
                delay(200);
                rkBuzzerSet(false);
            
            //Čekání na červeném poli 10s
            delay(DelayOnRed);

            //Provedení slalomu
            slalom(true); 

                //Ohlášení dokončení disciplíny
                rkBuzzerSet(true);
                delay(200);
                rkBuzzerSet(false);

            //Čekání na červeném poli 10s
            delay(DelayOnRed);
            
            //Provedení Bludiště
            bludiste();

                //Ohlášení dokončení disciplíny
                rkBuzzerSet(true);
                delay(200);
                rkBuzzerSet(false);

                //Otočení LEFT mezi Bludištěm a Medvědem
                turn_on_spot_left(90, 50);
                    delay(100);
                back_buttons(40);
                    delay(100);
                forward_acc(od_steny_na_stred_pole, 50);
            
            // //Čekání na červeném poli 10s
            // delay(DelayOnRed);
            
            // //Provedení Medvěda
            // medved();

            // //Čekání na červeném poli 10s
            // delay(DelayOnRed);

            // //Provedení Kuliček
            // kulicky();


            break;

        case OFF_MEDVED:        //Provede MEDVEDA

                // Červená pro Medvěda
                rkLedRed(true);
            
            //Čekání na červeném poli 10s
            delay(DelayOnRed);

            //Provedení Medvěda
            medved();


            break;

        case DOWN_KULICKY:      //Provede KULIČKY

                // Modrá pro Kuličky
                rkLedBlue(true);  
                
            //Čekání na červeném poli 10s
            delay(DelayOnRed);

            //Provedení Kuliček
            kulicky();


            break;
            
        case RIGHT_BLUDISTE:    //Provede BLUDIŠTĚ

                // Žlutá pro Bludiště
                rkLedYellow(true);

            //Čekání na červeném poli 10s
            delay(DelayOnRed);

            //Provedení Bludiště
            bludiste();


            break;
            
        case LEFT_SLALOM:       //Provede SLALOM

                // Oranžová (Červená + Žlutá) pro Slalom
                rkLedRed(true);
                rkLedYellow(true);
            
            //Čekání na červeném poli 10s
            delay(DelayOnRed);

            //Provedení Slalomu
            slalom(true);
            

            break;
            
        case ON_SPRINT:         //Provede SPRINT

                // Fialová (Modrá + Červená) pro Sprint
                rkLedBlue(true);
                rkLedRed(true);  
                
            //Čekání na červeném poli 10s
            delay(DelayOnRed);

            //Provedení Sprintu
            sprint_m_d();        


            break;
            
        case BUTTON1_KOMBINACE1:    //Provede 1. KOMBINACI

                // Zeleno-žlutá pro kombinaci 1
                rkLedGreen(true);
                rkLedYellow(true); 
                
            //Čekání na červeném poli 10s
            delay(DelayOnRed);

            //Provedení     První   disciplíny z 1. kombinace
            slalom(false);

            //Provedení     Druhé   disciplíny z 1. kombinace

            //Provedení     Třetí   disciplíny z 1. kombinace

            //Provedení     Čtvrté  disciplíny z 1. kombinace

            //Provedení     Páté    disciplíny z 1. kombinace

            
            break;
            
        case BUTTON2_KOMBINACE2:    //Provede 2. KOMBINACI

                // Bílá (všechny barvy) pro kombinaci 2
                rkLedRed(true);
                rkLedGreen(true);
                rkLedBlue(true);
                rkLedYellow(true);
            
            //Čekání na červeném poli 10s   
            delay(DelayOnRed);

            //Provedení     První   disciplíny z 2. kombinace

            //Provedení     Druhé   disciplíny z 2. kombinace

            //Provedení     Třetí   disciplíny z 2. kombinace

            //Provedení     Čtvrté  disciplíny z 2. kombinace

            //Provedení     Páté    disciplíny z 2. kombinace


            break;
        
        case NONE:  //Nic se neprovádí
            break;
    }
    delay(100);
}