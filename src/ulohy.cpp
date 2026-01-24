#include "robotka.h"
#include "ulohy.h"

int zadek_od_stredu = 73; // v mm (7.3 cm)
int predek_od_stredu = 60; // v mm (6 cm)
int jedno_pole = 300; // v mm (30 cm)
int od_steny_na_stred_pole = jedno_pole/2 - zadek_od_stredu; // 150 - 73 = 77 mm (7.7 cm)

//Ovládání serva s klepetem pro Medvěda
void otevrit_klepeto()  { rkServosSetPosition(3, 90); }
void zavrit_klepeto()   { rkServosSetPosition(3, -30); }

//Ovládání serva s ramenem pro Kuličky
void vysun_zhazovadlo(){        //musi byt blokujici ... ceka nez dosahne stupne....
    rkServosSetPosition(4,-90);
    delay(200);
}
void zasun_zhazovadlo(){
    rkServosSetPosition(4, -20);
    delay(200);
} 

//Nastavení RGB hodnot červeného pole
bool cervena(){
    float r, g, b;
    if (rkColorSensorGetRGB("front", &r, &g, &b)) {
        printf("red: %f, green: %f, blue: %f\n", r, g, b);
        delay(10);
        if (r > g && r > b && r > 130)
        {
            printf("RED\n");
            rkLedRed(true);
            return true;
        }
        else {
            return false;
        }
    } else {
        Serial.println("Sensor 'front' not found.");
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

// Měření zda je volné pole RIGHT
bool is_free_right()    { return rkUltraMeasure(2) > 200; }

// Měření zda je volné pole FRONT
bool is_free_front()    { return rkUltraMeasure(1) > 200; }

//Srovnávání pomocí back_buttons (zadních tlačítek) v bludišti
void srovnani(){
    back_buttons(40);
    forward_acc(od_steny_na_stred_pole, 50);
    delay(100);
    turn_on_spot_left(90, 50);
    delay(100);
    back_buttons(40);
    delay(100);
    forward_acc(od_steny_na_stred_pole, 50);
    delay(100);
}

//SPRINT
void sprint_m_d(){

    //Změření vzdálenosti k pravé stěně před jízdou
    int first_distance_right = rkUltraMeasure(2);

    // Nastavení P-regulátoru a rychlosti
    const int base_speed = 60; // Základní rychlost
    const float Kp = 0.3;      // Proporční konstanta (laditelná hodnota)

    int rychlost_levy_motor = base_speed;
    int rychlost_pravy_motor = base_speed;

    // Smyčka běží, dokud je před námi volno
    while(rkUltraMeasure(1) > (od_steny_na_stred_pole + 20)){ // + 20 kvuli setrvacnosti

        // Odeslání příkazu motorům
        rkMotorsSetSpeed(rychlost_levy_motor, rychlost_pravy_motor);

        // Malá pauza pro plynulost
        delay(20);

        //Měření vzdálenosti k pravé stěně během jízdy
        int current_distance_right = rkUltraMeasure(2);

        int error_right = current_distance_right - first_distance_right;

        int distance_vpredu = rkUltraMeasure(1);

        if((current_distance_right > 200) || (current_distance_right == 0) || ((distance_vpredu < 700) && (distance_vpredu > 200))){
            rychlost_levy_motor = base_speed;
            rychlost_pravy_motor = base_speed;
        }
        else if(abs(error_right) > 5){

            // Výpočet korekce
            int correction = Kp * error_right;

            // Úprava rychlosti motorů
            rychlost_levy_motor = base_speed + correction;
            rychlost_pravy_motor = base_speed - correction;
        }
        else{
            rychlost_levy_motor = base_speed;
            rychlost_pravy_motor = base_speed;
        }
    }

    // Po skončení smyčky zastavíme
    rkMotorsSetSpeed(0, 0);

    //Rovnání se v rohu
    delay(100);
    turn_on_spot_right(90, 50);
    delay(100);
    back_buttons(40);
    forward_acc(od_steny_na_stred_pole, 50);
    delay(100);
    turn_on_spot_right(90, 50);
    delay(100);
    back_buttons(40);
    delay(100);
    forward_acc(od_steny_na_stred_pole, 50);
    delay(100);

    //Otočení k výjezdu
    turn_on_spot_left(90, 50);
}

//SLALOM
//  SLALOM  --  true    --> první radius RIGHT
//  SLALOM  --  false   --> první radius LEFT
void slalom(bool right){
    if(right){

        //Rádiusy kopírují čáru
        forward_acc(od_steny_na_stred_pole, 60);
        radius_right(100, 90, 50);
        delay(100);
        radius_left(90, 180, 50);
        delay(100);
        radius_right(90, 180, 50);
        delay(100);

        //Srovnani v rohu před vjetím do cíle
        back_buttons(30);
        delay(100);
        forward_acc(od_steny_na_stred_pole, 60);
        delay(100);
        turn_on_spot_right(90, 50);
        delay(100);
        back_buttons(30);
        delay(100);
        forward_acc(od_steny_na_stred_pole, 60);
        delay(100);
        turn_on_spot_left(90, 50);
        delay(100);
            back_buttons(30);
            delay(100);
        forward_acc(jedno_pole + od_steny_na_stred_pole, 60);
        delay(100);
    } else{

        //Rádiusy kopírují čáru
        forward_acc(od_steny_na_stred_pole + 40, 60);
        radius_left(80, 180, 50);
        delay(100);
        radius_right(80, 180, 50);
        delay(100);
        radius_left(90, 90, 50);
        delay(100);
        forward_acc(od_steny_na_stred_pole, 60);
        delay(100);
    }   
}

//MEDVĚD
void medved(){

    //Otevře klepeto
    otevrit_klepeto();
    delay(100);

    //Na 2. pole
    forward(430,40);
    delay(100);

    //Zatočení na 3. pole
    radius_right(90, 90, 40);
    delay(100);

    //Na 4. pole
    forward(jedno_pole + 50, 40);
    delay(100);

    //Vycouvání a jetí do cíle
    back_buttons(30);
    delay(10);
    zavrit_klepeto();
    delay(10);
    forward_acc(od_steny_na_stred_pole, 40);
    delay(100);
    turn_on_spot_right(90, 40);
    delay(10);
    back_buttons(30);
    delay(10);
    forward_acc(jedno_pole + od_steny_na_stred_pole, 40); // 345
    delay(10);
    turn_on_spot_left(90, 40);
    back_buttons(30);
    delay(10);
    forward_acc(jedno_pole + od_steny_na_stred_pole, 40);
    delay(10);
}

//KULIČKY
void kulicky(){
    int cekani = 1500;

    //Na pozici první kuličky
    forward_acc(60, 30);
    vysun_zhazovadlo();
    delay(cekani);
    zasun_zhazovadlo();

    //Na pozici druhé kuličky
    forward_acc(jedno_pole,50);
    vysun_zhazovadlo();
    delay(cekani);
    zasun_zhazovadlo();

    //Na pozici třetí kuličky
    forward_acc(jedno_pole,50);
    vysun_zhazovadlo();
    delay(cekani);
    zasun_zhazovadlo();

    //Srovnání se
    backward(od_steny_na_stred_pole, 30);
    turn_on_spot_left(90, 50);
    delay(100);
    back_buttons(od_steny_na_stred_pole);
    delay(100);

    //Vjetí do cíle
    forward_acc(od_steny_na_stred_pole + jedno_pole, 50);

}

//BLUDIŠTĚ
void bludiste(){

    forward_acc(jedno_pole,40);

    //Provádí na začátku, aby se nomohl omylem vrátit zpět na startu
    for(int i=0; i< 5; i++){
        if(is_free_right()){                // v pravo je volno
            turn_on_spot_right(90, 40);
            delay(10);
            forward_acc(jedno_pole,40);
            delay(10);
        }
        else if(is_free_front()){           // ve predu je volno
            forward_acc(jedno_pole,40);
            delay(10);
        }
        else{                               // volno v levo/v zadu         
            turn_on_spot_left(90, 40);
            if(is_free_front()){            // v levo je volno
                srovnani(); 
                turn_on_spot_right(90, 40); 
            }
            else{                           // v zadu je volno --> musíme couvat
                rkLedBlue(true);
                srovnani();
            }
        }
        delay(100);
    }

    //Provádí se až nehrozí že se vrátí zpět na start
    while(!cervena()){
        rkBuzzerSet(true);
        delay(200);
        rkBuzzerSet(false);
        delay(100);
        if(is_free_right()){                // v pravo je volno
            turn_on_spot_right(90, 40);
            delay(100);
            forward_acc(jedno_pole,40);
        }
        else if(is_free_front()){ 
            
            //Rovnání se o pravou stěnu
            turn_on_spot_left(90, 40);
            back_buttons(30);
            delay(100);
            forward_acc(od_steny_na_stred_pole, 40);
            delay(100);
            turn_on_spot_right(90, 40);

            // ve predu je volno
            forward_acc(jedno_pole,40);
            delay(100);
        }
        else{                               // volno v levo/v zadu     
            turn_on_spot_left(90, 40);
            if(is_free_front()){            // v levo je volno
                srovnani(); 
                turn_on_spot_right(90, 40); 
            }
            else{ srovnani(); }             // v zadu je volno --> musíme couvat
        }
        delay(100);
    }
}
