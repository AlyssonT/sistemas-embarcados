#include "fx.cpp"
#include <iostream>

#define LEFT 5
#define RIGHT 4
#define LED_PIN 1
#define LED_COUNT 144
#define ROW 12
#define COL 12
#define INTERRUPT 2

volatile int button_pressed = 0;
int offset = 4;
int velX = random(2);
int velY = 2;
int PX = random(12)-1;
int PY = 3;
int nBlocos = 36;
int pausa = 0;

bool blocos[3][12] = {
    {true, true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true, true},
};

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int converteXY(int x, int y){
    return y%2 ? COL*y+(COL-x-1) : x+COL*y;
}

void set_button_pressed(void) {
	button_pressed = 1;
}

void desenhaBase(){
    for(int i=0;i<4;i++){
        strip.setPixelColor(converteXY(i+offset,11), 0, 255, 0);
    }
}

void desenhaBlocos(){
    for(int i=0;i<3;i++){
        for(int j=0;j<12;j++){
            if(blocos[i][j]){
                strip.setPixelColor(converteXY(j,i), 255, 255, 0);
            }
        }
    }
}

void desenhaProjetil(){
    strip.setPixelColor(converteXY(PX,PY), 0, 0, 255);
}

void andaBase(){
    if(digitalRead(LEFT)==HIGH ){
        offset = offset>0 ? offset-1 : offset;
    }
    else if(digitalRead(RIGHT)==HIGH){
        offset = offset<8 ? offset+1 : offset;
    }
}

void movimenta(){
    if(velX == 1){
        if(PX > 0){
            PX--;
        }
        else{
            PX++;
            velX = 2;
        }
    }
    else{
        if(PX < 11){
            PX++;
        }
        else{
            PX--;
            velX = 1;
        }
    }

    if(velY == 1){
        if(PY > 0){
            PY--;
        }
        else{
            PY++;
            velY = 2;
        }
    }
    else{
        if(PY < 11){
            PY++;
        }
        else{
            PY--;
            velY = 1;
        }
    }
}

void restart(){
    for(int i=0;i<3;i++){
        for(int j=0;j<12;j++){
            blocos[i][j]=true;
        }
    }
    nBlocos = 36;
    pausa=0;
    PY=3;
    PX=random(12)-1;
    delay(2000);
}

void verificaColisao(){
    int proxX = velX==1 ? PX-1 : PX+1;
    int proxY = velY==1 ? PY-1 : PY+1;

    if(PX==11){
        proxX-=2;
    }
    else if(PX==0){
        proxX+=2;
    }
    if(strip.getPixelColor(converteXY(proxX,proxY)) || blocos[proxY][PX]){
        if(velX == 1){
            if(velY == 1){
                velY = 2;
                if(blocos[proxY][PX]){
                   blocos[proxY][PX]=false;
                }
                else{
                    blocos[proxY][proxX]=false;
                }
                nBlocos--;
            }
            else{
                velY = 1;
            }
        }
        else{
            if(velY == 1){
                velY = 2;
                if(blocos[proxY][PX]){
                   blocos[proxY][PX]=false;
                }
                else{
                    blocos[proxY][proxX]=false;
                }
                nBlocos--;
            }
            else{
                velY = 1;
            }
        }
    }
}

void setup(){
    pinMode(LEFT, INPUT);
    pinMode(RIGHT, INPUT);
    pinMode(INTERRUPT, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(INTERRUPT), set_button_pressed, RISING);
    randomSeed(analogRead(0));
	strip.begin();
	strip.show();
}

void loop(){
    strip.clear();
    if(PY==11) restart();
    if(!nBlocos) restart();

    andaBase();
    if(pausa>10) movimenta();
    desenhaBase();
    desenhaBlocos();
    verificaColisao();
    desenhaProjetil();

    if(pausa<=10) pausa++;
    strip.show();
	delay(100 + nBlocos*3);
}
