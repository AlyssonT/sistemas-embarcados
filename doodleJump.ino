#include <Adafruit_NeoPixel.h>

#define LEFT D5
#define RIGHT D6
#define LED_PIN (D3)
#define LED_COUNT 144
#define ROW 12
#define COL 12
#define INTERRUPT 2

int px=6;
int py=11;
int jumpTime=7;
int bx1=random(10);
int by1=8;
int bx2=random(10);
int by2=2;
int nbarras=0;
bool primeiro=true;
bool pulando=false;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int converteXY(int x, int y){
    return y%2 ? COL*y+(COL-x-1) : x+COL*y;
}

void movimentaPersonagemX(){
    if(digitalRead(LEFT)==HIGH ){
        px = px>0 ? px-1 : px;
    }
    else if(digitalRead(RIGHT)==HIGH){
        px = px<11 ? px+1 : px;
    }
}

void desenhaBarras(){
    for(int i=0;i<3;i++){
        strip.setPixelColor(converteXY(bx1+i,by1),0,255,0);
        strip.setPixelColor(converteXY(bx2+i,by2),0,255,0);
    }
}

void desenhaPersonagem(){
    strip.setPixelColor(converteXY(px,py),255,0,0);
    strip.setPixelColor(converteXY(px,py-1),255,0,0);
}

void recriaBarra1(){
    by1=0;
    bx1=random(10);
}

void recriaBarra2(){
    by2=0;
    bx2=random(10);
}

void pulaCai(){
    if(pulando && jumpTime>0){
        if(py==3){
            by1++;
            by2++;
            jumpTime--;
            if(by1>11){
                recriaBarra1();
            }
            if(by2>11){
                recriaBarra2();
            }
        }
        else{
            py--;
            jumpTime--;
        }
    }
    else{
        pulando=false;
        if((py+1==by1 && (px>=bx1 && px<bx1+3)) || (py+1==by2 && (px>=bx2 && px<bx2+3))){
            jumpTime=7;
            pulando=true;
            primeiro=false;
            nbarras++;
        }
        else {
            py = py<11 ? py+1 : py;
        }
    }
    if(nbarras==0 && py==11) {pulando=true;jumpTime=7;}
}

void restart(){
    px=6;
    py=11;
    jumpTime=7;
    bx1=random(10);
    by1=8;
    bx2=random(10);
    by2=2;
    primeiro=true;
    pulando=false;
    nbarras=0;
    delay(2000);
}

void verificaPerdeu(){
    if(py==11){
        restart();
    }
}

void setup(){
    pinMode(LEFT, INPUT_PULLUP);
    pinMode(RIGHT, INPUT_PULLUP);
    pinMode(INTERRUPT, INPUT_PULLUP);
    randomSeed(analogRead(0));
	strip.begin();
	strip.show();
}



void loop(){
    strip.clear();
    
    if(primeiro) pulando=true;
    else verificaPerdeu();
    movimentaPersonagemX();
    pulaCai();
    desenhaBarras();
    desenhaPersonagem();

    strip.show();
	delay((150 - nbarras*3)>40 ? 150 - nbarras*3 : 40);
}
