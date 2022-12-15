#include "fx.cpp"
#include <iostream>

int estadoEscolha=1;
int estado=0;

#define SHOOT 7
#define LEFT 5
#define RIGHT 4
#define BACK 6
#define LED_PIN 1
#define LED_COUNT 144
#define ROW 12
#define COL 12
#define INTERRUPT 2
#define TEMPOQUEDA 1

volatile int button_pressed = 0;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int converteXY(int x, int y){
    return y%2 ? COL*y+(COL-x-1) : x+COL*y;
}

void set_button_pressed(void) {
	button_pressed = 1;
}

// VARIAVEIS SPACE INVADERS

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

void restartSI(){
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
            restartSI();
            return;
        }
        alienAtirou=false;
    }
    if(pax==offsetNave+1 && pay==10){
        restartSI();
        return;
    }
}

void loopSI(){
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

    if(nAliens==0) restartSI();

    strip.show();
	delay(100 + nAliens*3);
    if(digitalRead(BACK)==HIGH) {estado=0;delay(1000);}
}

// VARIAVEIS FLAPPY

int birdX = 2;
int birdY = 5;
int colidiu = 0;
int pulou = 0;
int tempoQueda = TEMPOQUEDA;

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

void verificaColisaoFB(){
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
int variaCanos(){
    int direcao=random(2);
    int quantoDesloca=random(4);
    return direcao? quantoDesloca : -quantoDesloca;
}

void pula(){
    delay(0);
    if(digitalRead(LEFT)==HIGH && birdY>0){
        strip.setPixelColor(converteXY(birdX,birdY), 0, 0, 0);
        birdY--;
        strip.setPixelColor(converteXY(birdX,birdY), 0, 255, 0);
        pulou=1;
    }
}

void loopFB(){
    strip.clear();
    int variacao=variaCanos();
    for(int i=COL;i>=0;i--){
        desenhaCanos(i,variacao);
        pula();
        if(!pulou){
            trataQueda();
        }
        verificaColisaoFB();
        if(colidiu){
            colidiu=0;
            break;
        }
        pulou=0;
        strip.show();
		delay(200);
        if(digitalRead(BACK)==HIGH) {estado=0;delay(1000);}
    }
}

// VARIAVEIS DOODLE JUMP

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

void restartDJ(){
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
        restartDJ();
    }
}

void loopDJ(){
    strip.clear();
    
    if(primeiro) pulando=true;
    else verificaPerdeu();
    movimentaPersonagemX();
    pulaCai();
    desenhaBarras();
    desenhaPersonagem();

    strip.show();
	delay((150 - nbarras*3)>40 ? 150 - nbarras*3 : 40);
    if(digitalRead(BACK)==HIGH) {estado=0;delay(1000);}
}

// VARIAVEIS BREAKOUT

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

void restartBreakout(){
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

void loopBreakout(){
    strip.clear();
    if(PY==11) restartBreakout();
    if(!nBlocos) restartBreakout();

    andaBase();
    if(pausa>10) movimenta();
    desenhaBase();
    desenhaBlocos();
    verificaColisao();
    desenhaProjetil();

    if(pausa<=10) pausa++;
    strip.show();
	delay(100 + nBlocos*3);
    if(digitalRead(BACK)==HIGH) {estado=0;delay(1000);}
}

//-----------------------------------**********---------------------------------------------

void desenhaFlappy(){
    desenhaCanos(9,-1);
    strip.setPixelColor(converteXY(2,6),0,255,0);
}

void desenhaBreakout(){
    for(int i=0;i<4;i++){
        strip.setPixelColor(converteXY(i+3,11), 0, 255, 0);
    }
    for(int i=0;i<3;i++){
        for(int j=0;j<12;j++){
            strip.setPixelColor(converteXY(j,i), 255, 255, 0);
        }
    }
    strip.setPixelColor(converteXY(6,6), 0,0,255);
}

void desenhaDJ(){
    for(int i=0;i<3;i++){
        strip.setPixelColor(converteXY(3+i,7),0,255,0);
        strip.setPixelColor(converteXY(7+i,3),0,255,0);
    }
    strip.setPixelColor(converteXY(7,11),255,0,0);
    strip.setPixelColor(converteXY(7,10),255,0,0);
}

void desenhaSI(){
    for(int i=0;i<3;i++){
        strip.setPixelColor(converteXY(4+i,ROW-1),150,0,150);
    }
    strip.setPixelColor(converteXY(4+1,ROW-2),150,0,150);

    for(int i=0;i<4;i++){
        for(int j=0;j<5;j++){
            strip.setPixelColor(converteXY(j*2+1,i),0,150,0);
        }
    }
}

void loopEscolha(){
    strip.clear();

    if(estadoEscolha==1){
        desenhaFlappy();
        if(digitalRead(BACK)==HIGH) {estado=1;delay(200);}
        else if(digitalRead(LEFT)==HIGH) {estadoEscolha=4;delay(200);}
        else if(digitalRead(RIGHT)==HIGH) {estadoEscolha=2;delay(200);}
    }
    else if(estadoEscolha==2){
        desenhaBreakout();
        if(digitalRead(BACK)==HIGH) {estado=2;delay(200);}
        else if(digitalRead(LEFT)==HIGH) {estadoEscolha=1;delay(200);}
        else if(digitalRead(RIGHT)==HIGH) {estadoEscolha=3;delay(200);}
    }
    else if(estadoEscolha==3){
        desenhaDJ();
        if(digitalRead(BACK)==HIGH) {estado=3;delay(200);}
        else if(digitalRead(LEFT)==HIGH) {estadoEscolha=2;delay(200);}
        else if(digitalRead(RIGHT)==HIGH) {estadoEscolha=4;delay(200);}
    }
    else if(estadoEscolha==4){
        desenhaSI();
        if(digitalRead(BACK)==HIGH) {estado=4;delay(200);}
        else if(digitalRead(LEFT)==HIGH) {estadoEscolha=3;delay(200);}
        else if(digitalRead(RIGHT)==HIGH) {estadoEscolha=1;delay(200);}
    }

    strip.show();
    delay(200);
}


void setup(){
    pinMode(LEFT, INPUT);
    pinMode(RIGHT, INPUT);
    pinMode(SHOOT, INPUT);
    pinMode(INTERRUPT, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(INTERRUPT), set_button_pressed, RISING);
    randomSeed(analogRead(0));
	strip.begin();
	strip.show();
}

void loop(){
    if(estado==0){
        loopEscolha();
    }
    else if(estado==1){
        loopFB();
    }
    else if(estado==2){
        loopBreakout();
    }
    else if(estado==3){
        loopDJ();
    }
    else if(estado==4){
        loopSI();
    }
}