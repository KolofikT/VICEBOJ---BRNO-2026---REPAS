#include "robotka.h"
#include "ulohy.h"

int zadek_od_stredu = 73; // v mm (7.3 cm)
int predek_od_stredu = 60; // v mm (6 cm)
int jedno_pole = 300; // v mm (30 cm)
int od_steny_na_stred_pole = jedno_pole/2 - zadek_od_stredu; // 150 - 73 = 77 mm (7.7 cm)




void otevrit_klepeto(){
    rkServosSetPosition(3, 90);
}
void zavrit_klepeto(){
    rkServosSetPosition(3, -50);
}

void vysun_zhazovadlo(){        //musi byt blokujici ... ceka nez dosahne stupne....
    rkServosSetPosition(4,-90);
    delay(200);
}
void zasun_zhazovadlo(){
    rkServosSetPosition(4, -20);
    delay(200);
} 

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

// Assuming sensor 2 is for left
bool is_free_right()    { return rkUltraMeasure(2) > 200; }

bool is_free_front()    { return rkUltraMeasure(1) > 200; }

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
    turn_on_spot_left(90, 50);
}

void slalom(bool right){
    if(right){

        //Radiusy kopírují čáru
        forward_acc(od_steny_na_stred_pole, 60);
        radius_right(100, 90, 50);
        delay(100);
        radius_left(90, 180, 50);
        delay(100);
        radius_right(90, 180, 50);
        delay(100);

        //Srovnani pred vjetim do cile
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
        forward_acc(jedno_pole, 60);
        delay(100);
    } else{

        //Rádiusy koúírují čáru
        forward_acc(od_steny_na_stred_pole + 40, 60);
        radius_left(80, 180, 50);
        delay(100);
        radius_right(80, 180, 50);
        delay(100);
        radius_left(90, 90, 50);
        delay(100);

        // //Srovnani před vjetím do cíle
        // back_buttons(30);
        // delay(100);
        // forward_acc(od_steny_na_stred_pole, 60);
        // delay(100);
        // turn_on_spot_right(90, 50);
        // delay(100);
        // back_buttons(30);
        // delay(100);
        // forward_acc(od_steny_na_stred_pole, 60);
        // delay(100);
        // turn_on_spot_left(90, 50);
        // delay(100);
        // forward_acc(jedno_pole + od_steny_na_stred_pole, 60);
        // delay(100);
        // turn_on_spot_left(90, 50);
        // delay(100);
        forward_acc(od_steny_na_stred_pole, 60);
        delay(100);
    }

    // forward_acc(300,60);
    // turn_on_spot_right(90, 50);
    // //forward_acc(150,60);

    // //Jizda bez sledovaní cary
    // radius_left(180, 90, 50); 
    // radius_right(184, 90, 50);
    // back_buttons(30);

    // forward_acc(84, 30);
    // turn_on_spot_right(90, 50);
    // back_buttons(30);

    // forward_acc(84, 30);
    // turn_on_spot_left(90, 50);
    // forward_acc(300, 50);

    // rkMotorsSetSpeed(0, 0);
    // delay(100);
    // back_buttons(40);
    // delay(100);
    // forward_acc(od_steny_na_stred_pole , 50);
    // turn_on_spot_right(90, 50);
    // delay(100);
    // back_buttons(40);
    // delay(100);
    // forward_acc(od_steny_na_stred_pole , 50);
    // turn_on_spot_left(90, 50);
    // delay(100);
    // forward_acc(jedno_pole, 50);
    // delay(100);
    // //srovnej_se_v_levo();
    // delay(100);
    
}
void medved(){
    otevrit_klepeto();
    delay(100);
    forward(430,60);
    delay(100);
    radius_right(90, 90, 40);
    delay(100);
    forward(jedno_pole,70);
    delay(10);
    back_buttons(30);
    delay(10);
    zavrit_klepeto();
    delay(10);
    forward_acc(od_steny_na_stred_pole, 70);
    delay(100);
    turn_on_spot_right(90, 50);
    delay(10);
    back_buttons(30);
    delay(10);
    forward_acc(jedno_pole + od_steny_na_stred_pole, 70); // 345
    delay(10);
    turn_on_spot_left(90, 50);
    back_buttons(30);
    delay(10);
    forward_acc(jedno_pole + od_steny_na_stred_pole, 70);
    delay(10);
}

void kulicky(){
    int cekani = 1500;
    forward_acc(30,30);
    vysun_zhazovadlo();
    delay(cekani);
    zasun_zhazovadlo();
    forward_acc(jedno_pole,50);
    vysun_zhazovadlo();
    delay(cekani);
    zasun_zhazovadlo();
    forward_acc(jedno_pole,50);
    vysun_zhazovadlo();
    delay(cekani);
    zasun_zhazovadlo();
    backward(od_steny_na_stred_pole, 30);
    turn_on_spot_left(90, 50);
    delay(100);
    back_buttons(od_steny_na_stred_pole);
    delay(100);
    forward_acc(od_steny_na_stred_pole + jedno_pole, 50);

}

void bludiste(){

    forward_acc(jedno_pole,40);
    for(int i=0; i< 5; i++){
        if(is_free_right()){// v pravo je volno
            turn_on_spot_right(90, 50);
            delay(10);
            forward_acc(jedno_pole,40);
            delay(10);
        }
        else if(is_free_front()){// ve predu je volno
            forward_acc(jedno_pole,40);
            delay(10);
        }
        else{
            turn_on_spot_left(90, 50);
            if(is_free_front()){
                srovnani(); 
                turn_on_spot_right(90, 50); 
            }
            else{
                rkLedBlue(true);
                srovnani();
                // rkLedAll(true);
                // turn_on_spot_left(90, 50);
                // forward_acc(jedno_pole,40);
            }
        }
        delay(100);
    }

    while(!cervena()){
        rkBuzzerSet(true);
        delay(200);
        rkBuzzerSet(false);
        delay(100);
        if(is_free_right()){// v pravo je volno
            turn_on_spot_right(90, 50);
            delay(100);
            forward_acc(jedno_pole,40);
        }
        else if(is_free_front()){// ve predu je volno
            forward_acc(jedno_pole,40);
            delay(100);
        }
        else{
            turn_on_spot_left(90, 50);
            if(is_free_front()){
                srovnani(); 
                turn_on_spot_right(90, 50); 
            }
            else{
                srovnani();
                // rkLedAll(true);
                // turn_on_spot_left(90, 50);
            }
        }
        delay(100);
    }
}
