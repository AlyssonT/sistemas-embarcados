#include <Adafruit_NeoPixel.h>

#define LED_PIN (D3)
#define BUTTON D5
#define LED_COUNT 144
#define ROW 12
#define COL 12
#define INTERRUPT 2
#define TEMPOQUEDA 1

int birdX = 2;
int birdY = 5;
int colidiu = 0;
int pulou = 0;
int tempoQueda = TEMPOQUEDA;

//volatile int button_pressed = 0;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int converteXY(int x, int y){
    return !(y%2)? x+COL*y : COL*y+(COL-x-1);
}

void desenhaCano(int x, int y, int tam){
    int proxX,antX;
    for(int j=0;j<tam;j++){
        proxX=x-1;
        antX=x+1;
        if(antX<COL) strip.setPixelColor(converteXY(antX,y+j), 0, 0, 0);
        if(proxX>=0) strip.setPixelColor(converteXY(proxX,y+j), 255, 0, 0);
        if(x<COL) strip.setPixelColor(converteXY(x,y+j), 255, 0, 0);
    }
}

void desenhaCanos(int i, int variacao){
    int tam=4;

    desenhaCano(i,0,tam+variacao);
    desenhaCano(i,8+variacao,8+tam);
}

void trataQueda(){
    if(pulou) return;
    if(!tempoQueda && birdY<ROW-1){
        strip.setPixelColor(converteXY(birdX,birdY), 0, 0, 0);
        birdY++;
        tempoQueda = TEMPOQUEDA;
    }
    strip.setPixelColor(converteXY(birdX,birdY), 0, 255, 0);
    if(birdY<ROW-1) tempoQueda--;
}

void verificaColisao(){
    int cont=0;
    if(strip.getPixelColor(converteXY(birdX+1,birdY))) cont++;
    if(strip.getPixelColor(converteXY(birdX-1,birdY))) cont++; 
    if(strip.getPixelColor(converteXY(birdX,birdY+1))) cont++;
    if(strip.getPixelColor(converteXY(birdX,birdY-1))) cont++;

    if(cont>1){
        delay(2000);
        birdX = 2;
        birdY = 5;
        colidiu = 1;
    }
}

/*void set_button_pressed(void) {
	button_pressed = 1;
}*/

int variaCanos(){
    int direcao=random(2);
    int quantoDesloca=random(3);
    return direcao? quantoDesloca : -quantoDesloca;
}

void pula(){
    delay(0);
    if(digitalRead(BUTTON)==HIGH && birdY>0){
        strip.setPixelColor(converteXY(birdX,birdY), 0, 0, 0);
        birdY--;
        strip.setPixelColor(converteXY(birdX,birdY), 0, 255, 0);
        pulou=1;
    }
    //button_pressed = 0;
}

void setup(){
   Serial.begin(9600);
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(INTERRUPT, INPUT_PULLUP);
	//attachInterrupt(digitalPinToInterrupt(INTERRUPT), set_button_pressed, RISING);
    randomSeed(analogRead(0));
	strip.begin();
	strip.show();
}
int velocidade=0;
void loop(){
    strip.clear();
    int variacao=variaCanos();
    for(int i=COL;i>=0;i--){
        desenhaCanos(i,variacao);
        pula();
        if(!pulou){
            trataQueda();
        }
        verificaColisao();
        if(colidiu){
            colidiu=0;
            velocidade=0;
            break;
        }
        pulou=0;
        strip.show();
		delay(200-velocidade);
    if(velocidade<=100) velocidade++;
    }
}
