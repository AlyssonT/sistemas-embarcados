#include <Adafruit_NeoPixel.h>

#define SHOOT D7
#define LEFT D5
#define RIGHT D6
#define LED_PIN (D3)
#define LED_COUNT 144
#define ROW 12
#define COL 12
#define INTERRUPT 2

//volatile int button_pressed = 0;

int offsetNave=ROW/3;
int offsetAliens=1;
int pnx,pny,pax,pay;
int nAliens = 20;

bool naveAtirou=false;
bool alienAtirou=false;

bool aliens[4][5] = {
    {true, true, true, true, true},
    {true, true, true, true, true},
    {true, true, true, true, true},
    {true, true, true, true, true}
};

int alturaColunaAliens[5] = {4, 4, 4, 4, 4};

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int converteXY(int x, int y){
    return y%2 ? COL*y+(COL-x-1) : x+COL*y;
}

/*void set_button_pressed(void) {
	button_pressed = 1;
}*/


void setup(){
    pinMode(LEFT, INPUT_PULLUP);
    pinMode(RIGHT, INPUT_PULLUP);
    pinMode(SHOOT, INPUT_PULLUP);
    pinMode(INTERRUPT, INPUT_PULLUP);
	//attachInterrupt(digitalPinToInterrupt(INTERRUPT), set_button_pressed, RISING);
    randomSeed(analogRead(0));
	strip.begin();
	strip.show();
}

void desenhaNave(){
    for(int i=0;i<3;i++){
        strip.setPixelColor(converteXY(offsetNave+i,ROW-1),150,0,150);
    }
    strip.setPixelColor(converteXY(offsetNave+1,ROW-2),150,0,150);
}

void desenhaAliens(){
    for(int i=0;i<4;i++){
        for(int j=0;j<5;j++){
            if(aliens[i][j]){
                strip.setPixelColor(converteXY(j*2+offsetAliens,i),0,150,0);
            }
        }
    }
}

void desenhaProjetilNave(){
    strip.setPixelColor(converteXY(pnx,pny),150,75,0);
}

void desenhaProjetilAliens(){
    strip.setPixelColor(converteXY(pax,pay),150,150,0);
}

void movimentaNave(){
    if(digitalRead(LEFT)==HIGH ){
        offsetNave = offsetNave>0 ? offsetNave-1 : offsetNave;
    }
    else if(digitalRead(RIGHT)==HIGH){
        offsetNave = offsetNave<9 ? offsetNave+1 : offsetNave;
    }
}

void movimentaAliens(){
    int mov = random(30);
    if(mov==5){
        offsetAliens = offsetAliens>1 ? offsetAliens - 1 : offsetAliens;
    }
    else if(mov==6){
        offsetAliens = offsetAliens<2 ? offsetAliens + 1 : offsetAliens;
    }
}

void naveAtira(){
    if(digitalRead(SHOOT)==HIGH){
        pnx=offsetNave+1;
        pny=10;
        naveAtirou = true;
    }
}

void movimentaProjetilNave(){
    pny = pny>0 ? pny-1 : pny;
}

void trataColisaoProjetilNave(){
    int xAliens = pnx/2-offsetAliens+1;
    if(pny<4 && ((pnx+offsetAliens)%2==0) && (aliens[pny][xAliens])){
        aliens[pny][xAliens]=false;
        nAliens--;
        alturaColunaAliens[xAliens]--;
        naveAtirou=false;
    }
    else if(pny==0){
        naveAtirou=false;
    }
    
}

void restart(){
    for(int i=0;i<4;i++){
        for(int j=0;j<5;j++){
            aliens[i][j]=true;
        }
    }

    for(int i=0;i<5;i++){
        alturaColunaAliens[i]=4;
    }

    nAliens = 20;
    naveAtirou=false;
    alienAtirou=false;
    delay(2000);
}

void alienAtira(){
    int coluna = random(5);
    if(alturaColunaAliens[coluna]>0) alienAtirou=true;

    pax = coluna*2+offsetAliens;
    pay = alturaColunaAliens[coluna]-1;
}

void movimentaProjetilAliens(){
    pay = pay<11 ? pay+1 : pay;
}

void trataColisaoProjetilAliens(){
    if(pay==11){
        if(pax>=offsetNave && pax<offsetNave+3 && pay==11) {
            restart();
            return;
        }
        alienAtirou=false;
    }
    if(pax==offsetNave+1 && pay==10){
        restart();
        return;
    }
}

void loop(){
    strip.clear();
    
    movimentaNave();
    movimentaAliens();
    if(!naveAtirou) naveAtira();
    if(naveAtirou){
        movimentaProjetilNave();
        trataColisaoProjetilNave();
        desenhaProjetilNave();
    }
    if(!alienAtirou) alienAtira();
    if(alienAtirou){
        movimentaProjetilAliens();
        trataColisaoProjetilAliens();
        desenhaProjetilAliens();
    }
    desenhaNave();
    desenhaAliens();

    if(nAliens==0) restart();

    strip.show();
	delay(100 + nAliens*3);
}
