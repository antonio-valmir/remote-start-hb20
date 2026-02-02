#include <avr/wdt.h>


#include <stdint.h>

/* Guarda o status do reset para análise posterior, se necessário */
uint8_t mcusr_mirror __attribute__ ((section (".noinit")));

/* Função que roda imediatamente na inicialização (seção .init3) */
void get_mcusr(void) __attribute__((naked)) __attribute__((section(".init3")));
void get_mcusr(void)
{
  mcusr_mirror = MCUSR;
  MCUSR = 0;
  wdt_disable();
}

#include <EEPROM.h>
//#include <RCSwitch.h>



//  "ATUALIZAÇÃO" 25/01/2026
//RESOLVER BUGS E JANELAS DE TEMPO EM MIILIS() PURO
//********************************************************************************
//MÓDULO JÁ EM FUNCIONAMENTO COM WDT ON ATVADO POR FUSE'DATA:25/01/2026. BLINDAGEM ANTITRAVAMENTOS
// ESCRITO 0 NO BIT 4 DO HFUSE, ATIVANDO WDT ON.
// COM ESSE RECUSRO ATIVADO, SE HOUVER ALGUM TRAVAMENTO DO SISTEMA,
//WDT RESTAURA TUDO E DEVOLVE A ESTABILIDADE DO SISTEMA
// FUSE BOD ATIVADO EM 2.7V

// RODAR EM CLOCK 500khz, 1mhz, 2mhz...
/***************************************************************
 * PROJETO: Partida Remota Integrada – Hyundai HB20
 *
 * AUTOR:
 * Antonio Valmir da Silva Moraes
 *
 * INÍCIO DE OPERAÇÃO:
 * 18/12/2025
 *
 * HISTÓRICO DO PROJETO:
 * Este projeto nasceu da necessidade de implementar uma
 * partida remota funcional utilizando o telecomando original
 * do veículo, sem módulos comerciais prontos.
 *
 * No início, a proposta era simples, porém durante o
 * desenvolvimento surgiram desafios importantes relacionados
 * ao BCM, bloqueios do telecomando, lógica de ignição,
 * segurança do neutro e controle correto do arranque.
 *
 * Após diversos testes em bancada e no veículo real,
 * a lógica foi sendo refinada até atingir um comportamento
 * estável, previsível e seguro no uso diário.
 *
 * PROPOSTA DO SISTEMA:
 * - Permitir partida remota utilizando o telecomando original
 * - Garantir segurança elétrica e lógica do veículo
 * - Respeitar estados de neutro, embreagem e ignição
 * - Evitar consumo excessivo da bateria
 * - Não interferir no funcionamento original do carro
 * - Permitir feedback sonoro (bipe) configurável pelo usuário
 * - Manter lógica simples, robusta e confiável
 *
 * OBSERVAÇÃO IMPORTANTE:
 * Este projeto encontra-se FINALIZADO e VALIDADO EM USO REAL.
 * Alterações só devem ser feitas caso seja identificado
 * algum comportamento fora da lógica prevista.
 * 
 * CÓDIOG ORIGINAL
 *
 ***************************************************************/


#define SENS_TRAVA 9
#define SINAL_RESPOSTA A5
#define BIPE_BUZINA 11
#define SENS_CONTATO 10
#define SENS_DESTRAVA A4
#define SENS_SETA 7
#define SENS_NEUTRO 6
#define ARRANQUE 12
#define CONTATO 8
#define EMBREAGEM 13
#define CONTATO1 4


uint8_t EST_SENS_TRAVA = 0;
uint8_t ESTANT_SENS_TRAVA = 0;

uint8_t EST_SENS_TRAVA1 = 0;
uint8_t ESTANT_SENS_TRAVA1 = 0;


long LIGAR_DESLIGAR_BIPE = 0;
uint8_t AT_LIGAR_DESLIGAR_BIPE = 0;




unsigned long ATUALIZACAO = 0;

uint8_t EST_SENS_DESTRAVA = 0;
uint8_t ESTANT_SENS_DESTRAVA = 0;

uint8_t EST_SENS_DESTRAVA1 = 0;
uint8_t ESTANT_SENS_DESTRAVA1 = 0;

uint8_t EST_SENS_DESTRAVA2 = 0;
uint8_t ESTANT_SENS_DESTRAVA2 = 0;

uint8_t ESTANT_SENS_NEUTRO = 0;


uint8_t SETA_PENDENTE_DESTRAVA = 0;
uint8_t SETA_PENDENTE_DESTRAVA1 = 0;
uint8_t SETA_PENDENTE_TRAVA = 0;

uint8_t EST_SENS_SETA = 0;
uint8_t ESTANT_SENS_SETA = 0;
uint8_t EST_SENS_SETA3 = 0;
uint8_t ESTANT_SENS_SETA3 = 0;

uint8_t EST_SENS_SETA4 = 0;
uint8_t ESTANT_SENS_SETA4 = 0;

uint8_t SETA_PENDENTE_DESTRAVA3 = 0;
uint8_t  FLAG_SENS_SETA_DT_ATRASO =0;

uint8_t SETA_PENDENTE_TRAVA3 = 0;
uint8_t  FLAG_SENS_SETA_TR_ATRASO =0;

uint8_t AUX_BOT_IN_DT = 0;
uint8_t BOT_IN_DT = 0;
uint8_t AUX_BOT_IN_TR = 0;
uint8_t BOT_IN_TR = 0;


//uint8_t G_SENS_ST= 0;
static unsigned long TEMPO_ZERA_G_SEN_DT = 0;
static unsigned long TEMPO_ZERA_G_SEN_TR = 0;
                          
uint8_t EST_SENS_SETA1 = 0;
uint8_t ESTANT_SENS_SETA1 = 0;

uint8_t EST_SENS_SETA2 = 0;
uint8_t ESTANT_SENS_SETA2 = 0;

uint8_t EST_SENS_CONTATO = 0;
uint8_t ESTANT_CONTATO = 0;

uint8_t FLAG_TRAVA_SETA = 0;
unsigned int TIMER_TRAVA_SETA = 0;
static unsigned long TEMPO_TIMER_TRAVA_SETA = 0;



uint8_t FLAG_DESTRAVA_SETA = 0;
static unsigned long TEMPO_TIMER_DESTRAVA_SETA = 0;
unsigned int TIMER_DESTRAVA_SETA = 0;

uint8_t FLAG_DESTRAVA_SETA1 = 0;
static unsigned long TEMPO_TIMER_DESTRAVA_SETA1 = 0;
unsigned int TIMER_DESTRAVA_SETA1 = 0;


uint8_t SIN_DESTRAVA = 0;
uint8_t SIN_TRAVA = 0;
uint8_t SIN_SETA = 0;
unsigned long TEMPO_ARRANQUE = 0;
static unsigned long TEMPO_ESPERA_PARTIDA = 0;
unsigned long ESPERA_PARTIDA = 0;
int LIGAR_ESPERA_PARTIDA = 0;
uint8_t PAUSA_PARTIDA = 0;
uint8_t REST_LIGAR_ESPERA_PARTIDA = 0;
static unsigned long TEMPO_SINAL_RESPOSTA = 0;
uint8_t ATIVA_SINAL_RESPOSTA = 0;

uint8_t ESTADO_CONTATO = 0;
uint8_t AUXIAR_MODO_CHAVE = 0;
uint8_t ESTADO_ARRANQUE = 0;
uint8_t BLOQUEIO_ARRANQUE = 1;
uint8_t AUX_DESLIGAR =0;
//boolean  BLQ_BCM  = 0;
uint8_t INT_EMBREAGEM = 0;

uint8_t TRAVA_PARTIDA = 1;
uint8_t ATIVA_ARRANQUE = 0;
uint8_t LIBERA_PARTIDA = 0;
uint8_t TRAVA_PARTIDA1 = 1;


static unsigned long TEMPO_MODO_CHAVE = 0;

uint8_t ATIVA_MODO_CHAVE = 0;
uint8_t   MODO_CHAVE = 0;

unsigned long ZERA_SIN_DESTRAVA  = 0;
 static unsigned long TEMPO_SIN_DESTAVA = 0; 
unsigned long ZERA_SIN_TRAVA = 0;
static unsigned long TEMPO_SIN_TAVA = 0;

unsigned long ZERA_ATIVA_DES_BIPE = 0;
static unsigned long TEMPO_VELOC_BIPE = 0;  
  
static unsigned long TEMPO_FREQ_BIPE = 0;
static unsigned long TEMPO_ZERA_ATIVA_DES_BIPE= 0;
unsigned long ZERA_LIBERA_LIGAR_DESLIGAR_BIPE= 0;

uint8_t LIBERA_LIGAR_DESLIGAR_BIPE = 0;


uint8_t XDT = 0;
uint8_t XTR = 0;
uint8_t XLI = 0;
uint8_t XTB = 0;
uint8_t XTB1 = 0;
uint8_t XTST = 0;
uint8_t XTS_TR = 0;
uint8_t XBIPAR = 0;
uint8_t XTRF = 0;
static unsigned long ZERA_G_SEN_T = 0;


uint8_t ATIVA_CONTA_CLIQUE = 0;



uint8_t LIBERA_IGNICAO = 0;
static unsigned long TEMPO_ZERA_LIBERA_IGNICAO = 0;
unsigned long ZERA_LIBERA_IGNICAO = 0;
unsigned long BIPAR = 0;
unsigned long QUANT_BIPAR = 0;
uint8_t ATIVA_BIPE = 0;
uint16_t VELOCIDADE_BIPE = 100;
//uint8_t LIGAR_BIPE = 0;
uint8_t LIGA_DESL_BUZINA = 0;
uint8_t NUM_BIPE = 0;


//int ANVIAR_RF = 0;
long CONTA_CLIQUE = 0;



uint8_t MARCHA_DETECT = 0;
static unsigned long TEMPO_ZERA_LIBERA_LIGAR_DESLIGAR_BIPE = 0;
unsigned long CORTE_BIPE = 0;
unsigned long DELAY_PARTIDA= 0;
unsigned long DEBOUNCE_SENSOR = 0;
unsigned long DEBOUNCE_SENSOR_SETA = 0;
unsigned long TEMPO_JANELAS = 0;


//RCSwitch mySwitch = RCSwitch();


void setup() {
  clock_prescale_set(clock_div_4); // 8 MHz / 4 = 2 MHz
  MCUSR = 0;      // limpa flags de reset
  wdt_disable();
 

   // TRANSMISSOR NO PINO 2 
  //mySwitch.enableTransmit(2);
  
AT_LIGAR_DESLIGAR_BIPE = EEPROM.read(5);

pinMode(  ARRANQUE ,OUTPUT);
pinMode( CONTATO,OUTPUT);
pinMode( SENS_TRAVA,INPUT_PULLUP);
pinMode( SENS_DESTRAVA ,INPUT_PULLUP);
pinMode( SENS_CONTATO, INPUT_PULLUP);
pinMode( SENS_NEUTRO, INPUT_PULLUP);
pinMode( SENS_SETA, INPUT_PULLUP);
pinMode( EMBREAGEM ,OUTPUT);
pinMode(BIPE_BUZINA  ,OUTPUT);
pinMode(SINAL_RESPOSTA  ,OUTPUT);


pinMode( CONTATO1 ,OUTPUT);


  digitalWrite(CONTATO,LOW);
  digitalWrite(CONTATO1,LOW);
  digitalWrite(EMBREAGEM,LOW);
  digitalWrite(ARRANQUE,LOW);
  digitalWrite(  BIPE_BUZINA ,LOW);
 
  wdt_enable(WDTO_2S);

//Serial.begin(19200);

}

void loop() {
  wdt_reset();
if (ESTADO_CONTATO == LOW) {
    digitalWrite(CONTATO1, LOW);
}

 JANELAS_TEMPORAIS();
 BIPE();
 EVENTOS_LIGAR_DESLIGAR_IGNICAO();
 PARITIDA();
 ALTERNAR_MODO_CHAVE();
 RESPOSTS_SIN_TRAVA_DESTRAVA();
 BOT_EXTRA_PROG();
 TRAVA_IN_MARCHA();

 

//if(millis()- DEBOUNCE_SENSOR_SETA>=10){

  // DEBOUNCE_SENSOR_SETA = millis(); 
   // BOT_EXTRA_PROG();
  
//}


if(millis()- DEBOUNCE_SENSOR >=50){

DEBOUNCE_SENSOR = millis();

SENSORES();
SENSOR_SETA();
}
  /*
  if(millis()- ATUALIZACAO >=1000){


     ATUALIZACAO = millis();
 
 

 
//Serial.print("  BLOQUEIO_ARRANQUE  =");
//Serial.println (   BLOQUEIO_ARRANQUE  );

//Serial.print("LIGAR_DESLIGAR_BIPE  =");
//Serial.println ( LIGAR_DESLIGAR_BIPE );
//Serial.print(" AUX_BOT_IN_DT =");
 //Serial.println (AUX_BOT_IN_DT );
 //Serial.print("BOT_IN_DT=");
// Serial.println ( BOT_IN_DT );
 

 

//Serial.print("LIBERA_LIGAR_DESLIGAR_BIPE =");
//Serial.println (  LIBERA_LIGAR_DESLIGAR_BIPE);

//Serial.print("SENS_SETA=");
 //Serial.println ( digitalRead(SENS_SETA));
 
 
  

 //Serial.print("G_SENS_ST=");
 //Serial.println (G_SENS_ST);

 //Serial.print("SIN_DESTRAVA=");
 //Serial.println (SIN_DESTRAVA);
 //Serial.print("LIGAR_DESLIGAR_BIPE  =");
 //Serial.println ( LIGAR_DESLIGAR_BIPE );
 
 //Serial.print(" ESPERA_PARTIDA=");
 //Serial.println ( ESPERA_PARTIDA);
 //Serial.print("ZERA_SIN_DESTRAVA  =");
 //Serial.println (ZERA_SIN_DESTRAVA );
 //Serial.print(" XDT =");
 //Serial.println ( XDT );
// Serial.print("LIBERA_IGNICAO=");
 //Serial.println (LIBERA_IGNICAO);
 
/*
 Serial.print("SENS_NEUTRO=");
 Serial.println ( digitalRead(SENS_NEUTRO ) );

 Serial.print("SENS_CONTATO=");
 Serial.println ( digitalRead(SENS_CONTATO ) );
 
 
 Serial.print("SENS_TRAVA=");
 Serial.println (digitalRead(SENS_TRAVA) );
 
 Serial.print("SENS_DESTRAVA=");
 Serial.println ( digitalRead(SENS_DESTRAVA) );

 Serial.print("SENS_SETA=");
 Serial.println ( digitalRead(SENS_SETA) );
 
 Serial.print("CONTATO=");
 Serial.println (ESTADO_CONTATO);
 
 Serial.print("CONTATO1=");
 Serial.println ( digitalRead(CONTATO1 ) );

 Serial.print("ARRANQUE=");
 Serial.println ( digitalRead(ARRANQUE) ); 
  

   
 //Serial.print("AT_LIGAR_DESLIGAR_BIPE= ");
// Serial.println (AT_LIGAR_DESLIGAR_BIPE);

 


     }
*/
}

void PARITIDA(){
static unsigned long T_TEMPO_ARRANQUE  = 0;
if( LIBERA_PARTIDA  == 1 && TRAVA_PARTIDA == 0 && PAUSA_PARTIDA == 1 && ESTADO_CONTATO  == HIGH && digitalRead(SENS_DESTRAVA) == LOW ){// MOMENTO DO DISPARO DO ARRANQUE
if(SIN_TRAVA  == 1 ){
  
 T_TEMPO_ARRANQUE = millis();
 ATIVA_ARRANQUE = 1;
 TRAVA_PARTIDA1 = 0;
 BLOQUEIO_ARRANQUE = 0;

 
}
  }

  if(digitalRead(SENS_DESTRAVA)==HIGH && ESTADO_CONTATO == HIGH){

     REST_LIGAR_ESPERA_PARTIDA = 0;
     LIGAR_ESPERA_PARTIDA =1;
     PAUSA_PARTIDA = 1;
    
  }

  if(TRAVA_PARTIDA1 == 0 &&  BLOQUEIO_ARRANQUE == 0){

 //============================================================
// ROTINA DE CONTROLE DO ARRANQUE – TEMPO REAL
//============================================================

static unsigned long T_TEMPO_ARRANQUE = 0;

//------------------------------------------------------------
// Inicia o ciclo do arranque quando o disparo é autorizado
//------------------------------------------------------------
if (ATIVA_ARRANQUE == 1 && TRAVA_PARTIDA == 0 && T_TEMPO_ARRANQUE == 0) {
    T_TEMPO_ARRANQUE = millis();   // INICIA O CICLO DO ARRANQUE
}

//------------------------------------------------------------
// Aguarda 30 ms antes de energizar o ARRANQUE
//------------------------------------------------------------
if (ATIVA_ARRANQUE == 1 && TRAVA_PARTIDA == 0 && ESTADO_ARRANQUE == LOW && millis() - T_TEMPO_ARRANQUE >= 300) {

    digitalWrite(ARRANQUE, HIGH);
    ESTADO_ARRANQUE = HIGH;

    if (ESTADO_CONTATO == HIGH) {
        digitalWrite(CONTATO1, LOW);
    }

    digitalWrite(EMBREAGEM, HIGH);
}

//------------------------------------------------------------

//------------------------------------------------------------
if (ATIVA_ARRANQUE == 1 && ESTADO_ARRANQUE == HIGH && millis() - T_TEMPO_ARRANQUE >= 10000) {

    digitalWrite(ARRANQUE, LOW);

    if (ESTADO_CONTATO == HIGH) {
        digitalWrite(CONTATO1, HIGH);
    }

    digitalWrite(EMBREAGEM, LOW);

    ESTADO_ARRANQUE = LOW;
    if (AT_LIGAR_DESLIGAR_BIPE == 1) {
    CHAMA_BIPE(1, 50);
}

    LIBERA_PARTIDA = 0;
    TRAVA_PARTIDA = 1;
    TRAVA_PARTIDA1 = 1;
    BLOQUEIO_ARRANQUE = 1;
    ATIVA_ARRANQUE = 0;
    SIN_DESTRAVA = 0;

    T_TEMPO_ARRANQUE = 0;   // LIBERA PARA O PRÓXIMO CICLO
}

   }

  
      }
 void SENSORES(){ 
// --- LEITURA BT TRAVA / DESTRAVA ---


//===========================>SENSOR DESTRAVA PARA LIGAR<=====================================================================================
//============================================================================================================================================
     EST_SENS_DESTRAVA = digitalRead(SENS_DESTRAVA);
    if(EST_SENS_DESTRAVA == HIGH && ESTANT_SENS_DESTRAVA == LOW && ESTADO_CONTATO  == LOW && LIBERA_IGNICAO ==1){//COMANDO DESTRAVA PARA LIGAR
   
   FLAG_DESTRAVA_SETA = 1;
   TEMPO_TIMER_DESTRAVA_SETA = millis();
   TIMER_DESTRAVA_SETA = 0;
   
   //  CAPTURA IMEDIATA DA SETA
  ESTANT_SENS_SETA1 = digitalRead(SENS_SETA);

  //  SE A SETA JÁ ESTIVER BAIXA, MARCA EVENTO PENDENTE
  if (ESTANT_SENS_SETA1 == LOW) {
    SETA_PENDENTE_DESTRAVA = 1;
  }
    
 
  
  }
  
  ESTANT_SENS_DESTRAVA = EST_SENS_DESTRAVA ;

  if(SIN_DESTRAVA>2){

  SIN_DESTRAVA =0;
 }


 
//========================>SENSOR DESTRAVA PARA DESLIGAR<==============================================================================================
//==================================================================================================================================
  EST_SENS_DESTRAVA1 = digitalRead(SENS_DESTRAVA);

  if(EST_SENS_DESTRAVA1 == HIGH && ESTANT_SENS_DESTRAVA1 == LOW &&  ESTADO_CONTATO  == HIGH){//COMANDO DESTRAVA PARA DESLIGAR
    
    TIMER_DESTRAVA_SETA1 = 0;
    FLAG_DESTRAVA_SETA1 = 1;
    TEMPO_TIMER_DESTRAVA_SETA1= millis();

    ESTANT_SENS_SETA2 = digitalRead(SENS_SETA);

  //  SE A SETA JÁ ESTIVER BAIXA, MARCA EVENTO PENDENTE
  if (ESTANT_SENS_SETA2 == LOW) {
    SETA_PENDENTE_DESTRAVA1 = 1;
  }
    
 
       } 
   
  ESTANT_SENS_DESTRAVA1 = EST_SENS_DESTRAVA1 ;

 

 

  //==============>SENSOR TRAVA<=================================================================
 //===================================================================================================
  EST_SENS_TRAVA = digitalRead(SENS_TRAVA);

  if(digitalRead(SENS_DESTRAVA) == LOW ){//EVITAR PARTIDA ENTRAR NA ROTINA DE LIGAR CONTATO
  if(EST_SENS_TRAVA == HIGH && ESTANT_SENS_TRAVA == LOW && SIN_SETA == 0){
   
   FLAG_TRAVA_SETA = 1;
   //TEMPO_TIMER_TRAVA_SETA = millis();
 
   TIMER_TRAVA_SETA = 0;
  //  captura imediata da seta
  ESTANT_SENS_SETA = digitalRead(SENS_SETA);

  //  se a seta já estiver LOW, marca pendente
  if (ESTANT_SENS_SETA == LOW) {
    SETA_PENDENTE_TRAVA = 1;
  }
   
      }

           }

  if(SIN_TRAVA>1){

 SIN_TRAVA =0;
}

 if(digitalRead(SENS_CONTATO)== LOW &&  ESTADO_CONTATO  == HIGH){

  TRAVA_PARTIDA = 0;
  ATIVA_MODO_CHAVE = 0;
 }


  EST_SENS_CONTATO = digitalRead(SENS_CONTATO);
 
  if( EST_SENS_CONTATO== HIGH && ESTANT_CONTATO ==LOW ){

 
  TRAVA_PARTIDA = 1;
 if( ESTADO_CONTATO  == HIGH){ 
  ATIVA_MODO_CHAVE = 1;
  AUXIAR_MODO_CHAVE = 1;
  TEMPO_MODO_CHAVE = millis();
  MODO_CHAVE = 0;
 }
  }
  ESTANT_CONTATO =  EST_SENS_CONTATO;

    //==============>SENSOR SETA SINCRONIZADO COM TRAVA<=================================================================
 //===================================================================================================



//=========================================================================================================
//============================================================================================================= 
 
}

void SENSOR_SETA(){


  EST_SENS_SETA = digitalRead(SENS_SETA);


  if ( FLAG_TRAVA_SETA == 1 &&( (EST_SENS_SETA == LOW && ESTANT_SENS_SETA == HIGH)||(SETA_PENDENTE_TRAVA == 1))){ // borda normal


  //  consome evento
  SETA_PENDENTE_TRAVA = 0;
  FLAG_TRAVA_SETA = 0;

  XTR = 1;
  TEMPO_SIN_TAVA = millis();   
  SIN_TRAVA++;


  ZERA_SIN_TRAVA = 0;
  ZERA_SIN_DESTRAVA = 0;
  SIN_DESTRAVA = 0;

  if (ESTADO_CONTATO == LOW) {
    LIBERA_IGNICAO = 1;
    XLI = 1;
    TEMPO_ZERA_LIBERA_IGNICAO = millis();
  }

  if (ESTADO_CONTATO == HIGH) {
    LIBERA_PARTIDA = 1;
  }
}
ESTANT_SENS_SETA = EST_SENS_SETA;



EST_SENS_SETA1 = digitalRead(SENS_SETA);

if (  FLAG_DESTRAVA_SETA == 1 && ( (EST_SENS_SETA == LOW && ESTANT_SENS_SETA == HIGH) ||(SETA_PENDENTE_DESTRAVA== 1))){  // borda normal
                                   
    // CONSOME EVENTO
  MARCHA_DETECT = 1;
  SETA_PENDENTE_DESTRAVA = 0;
  FLAG_DESTRAVA_SETA = 0;

  XDT = 1;
 // TEMPO_SIN_DESTAVA = millis();  
  SIN_DESTRAVA++;
  ZERA_SIN_DESTRAVA = 0;
  //LIBERA_LIGAR_DESLIGAR_BIPE = 1;
  MODO_CHAVE = 0;
  ATIVA_MODO_CHAVE = 0;
  TRAVA_PARTIDA1 = 1;
  ATIVA_SINAL_RESPOSTA = 1;
  LIGAR_ESPERA_PARTIDA = 1;
  TEMPO_ESPERA_PARTIDA = millis();//INICIA JANELA TEMPORAL
  PAUSA_PARTIDA = 1;
  AUX_DESLIGAR = 0;
  ESPERA_PARTIDA = 0;
}

/*
if(FLAG_DESTRAVA_SETA == 1 && EST_SENS_SETA1 ==LOW && ESTANT_SENS_SETA1 ==HIGH){
    XDT = 1;
    SIN_DESTRAVA ++;
    ZERA_SIN_DESTRAVA = 0;
    MODO_CHAVE = 0;
    ATIVA_MODO_CHAVE = 0;
    TRAVA_PARTIDA1 = 1;
    ATIVA_SINAL_RESPOSTA = 1;
    LIGAR_ESPERA_PARTIDA = 1; //ATIVAR TEMPOSIZADOR PARA CARRO ACEITAR PARTIDA
    PAUSA_PARTIDA = 1;
    AUX_DESLIGAR = 0;   
    ESPERA_PARTIDA = 0;

}
*/
ESTANT_SENS_SETA1 = EST_SENS_SETA1;

 //===========================================================================================
  //==============================================================================================.
  EST_SENS_SETA2 = digitalRead(SENS_SETA);
  if (FLAG_DESTRAVA_SETA1 ==1 &&(( EST_SENS_SETA2 ==LOW && ESTANT_SENS_SETA2 ==HIGH)||(SETA_PENDENTE_DESTRAVA1 == 1))){
    
    SETA_PENDENTE_DESTRAVA1=0;
    FLAG_DESTRAVA_SETA1=0;
    
    XDT = 1;
    //TEMPO_SIN_DESTAVA = millis();
    SIN_DESTRAVA ++;
     ZERA_SIN_DESTRAVA = 0;
    MODO_CHAVE = 0;
    ATIVA_MODO_CHAVE = 0;
    AUX_DESLIGAR = 1;
  }
  ESTANT_SENS_SETA2 = EST_SENS_SETA2 ;

 

}


void JANELAS_TEMPORAIS(){
 
//======>JANELA DE TEMPO SENSOR DESTRAVA<===============================
//====================================================================
 /*  
   if(XDT == 1){
   if(millis()-TEMPO_SIN_DESTAVA >1000){
    TEMPO_SIN_DESTAVA = millis();
   
    ZERA_SIN_DESTRAVA ++;
  }}
  
  if( ZERA_SIN_DESTRAVA>= 4){

 ZERA_SIN_DESTRAVA = 0;
 ZERA_SIN_TRAVA = 0;
    XDT = 0;
     SIN_DESTRAVA = 0;
     //LIBERA_PARTIDA  = 0;
     LIBERA_LIGAR_DESLIGAR_BIPE = 0;
     NUM_BIPE = 0;
      TEMPO_SIN_DESTAVA = 0;
  }
*/
// Garanta que a variável seja unsigned long

if (XDT == 1 && TEMPO_SIN_DESTAVA == 0) {
    TEMPO_SIN_DESTAVA = millis();
}
if (XDT == 1 && millis() - TEMPO_SIN_DESTAVA >= 6000) {

    ZERA_SIN_DESTRAVA = 0;
    ZERA_SIN_TRAVA = 0;
    XDT = 0;
    SIN_DESTRAVA = 0;
    LIBERA_LIGAR_DESLIGAR_BIPE = 0;
    NUM_BIPE = 0;

    TEMPO_SIN_DESTAVA = 0;
}


//========>JANELA DE TEMPO SENSOR TRAVA<=============================================================
//==================================================================================================
  // AT_IGAR_DESLIGAR_BIPE_DEST = 0;


/*
if ( XTR == 1) {   
//SIN_TRAVA ++;
if(millis()-TEMPO_SIN_TAVA >1000){
TEMPO_SIN_TAVA = millis();   
ZERA_SIN_TRAVA++; 

 }}
 if(ZERA_SIN_TRAVA>=1){// TEMPO DE ATIVIDADE DO SENSOR TRAVA
 SIN_TRAVA = 0;
 XTR = 0;
  ZERA_SIN_TRAVA = 0;
  TEMPO_SIN_TAVA = 0;
 }
 */

 if ( XTR == 1 && TEMPO_SIN_TAVA == 0){


  TEMPO_SIN_TAVA = millis();
 }

  if ( XTR == 1 && millis()-TEMPO_SIN_TAVA > 1500){

     SIN_TRAVA = 0;
 XTR = 0;
  ZERA_SIN_TRAVA = 0;
  TEMPO_SIN_TAVA = 0;
  }
 
//==============================================================================
//================================================================================
/*
if(XTB == 1){

  if(millis()-TEMPO_ZERA_ATIVA_DES_BIPE >=1){
    TEMPO_ZERA_ATIVA_DES_BIPE = millis();
    ZERA_ATIVA_DES_BIPE ++;
  }

}
if(ZERA_ATIVA_DES_BIPE >1000){

  ZERA_ATIVA_DES_BIPE = 0;
  XTB =0;

  LIGAR_DESLIGAR_BIPE = 0;
}
*/
if (XTB == 1 && TEMPO_ZERA_ATIVA_DES_BIPE == 0) {
    TEMPO_ZERA_ATIVA_DES_BIPE = millis();
}

if (XTB == 1 && millis() - TEMPO_ZERA_ATIVA_DES_BIPE >=10000) {
    XTB = 0;
    LIGAR_DESLIGAR_BIPE = 0;
    TEMPO_ZERA_ATIVA_DES_BIPE = 0;
   
    BOT_IN_TR = 0;
    AUX_BOT_IN_TR = 0;
    FLAG_SENS_SETA_TR_ATRASO =0;
    SETA_PENDENTE_TRAVA3 = 0; 
}

//============================================================================
//============================================================================
/*
if( XTB1 ==1){
  if(millis()-TEMPO_ZERA_LIBERA_LIGAR_DESLIGAR_BIPE >=1){
  TEMPO_ZERA_LIBERA_LIGAR_DESLIGAR_BIPE= millis();
  ZERA_LIBERA_LIGAR_DESLIGAR_BIPE++;
}
}
 if(ZERA_LIBERA_LIGAR_DESLIGAR_BIPE>800){
  XTB1 = 0;
  ZERA_LIBERA_LIGAR_DESLIGAR_BIPE = 0;
  LIBERA_LIGAR_DESLIGAR_BIPE = 0;
  G_SENS_ST = 0;
 }
if(XTST == 1 && TEMPO_ZERA_G_SEN_T == 0){
TEMPO_ZERA_G_SEN_T = millis();
  
}

*/
if (XTB1 == 1 && TEMPO_ZERA_LIBERA_LIGAR_DESLIGAR_BIPE == 0) {
    TEMPO_ZERA_LIBERA_LIGAR_DESLIGAR_BIPE = millis();
}

if (XTB1 == 1 && millis() - TEMPO_ZERA_LIBERA_LIGAR_DESLIGAR_BIPE >= 10000) {
    XTB1 = 0;
    LIBERA_LIGAR_DESLIGAR_BIPE = 0;
    BOT_IN_DT =0;
    AUX_BOT_IN_DT= 0;
    SETA_PENDENTE_DESTRAVA3 =0;
    FLAG_SENS_SETA_DT_ATRASO =0;
   
    TEMPO_ZERA_LIBERA_LIGAR_DESLIGAR_BIPE = 0;
}


 
//============================================================================================
//============================================================================================
 
/*
if(  FLAG_TRAVA_SETA == 1){

  if(millis()-TEMPO_TIMER_TRAVA_SETA>1){
 TEMPO_TIMER_TRAVA_SETA = millis();
 
 TIMER_TRAVA_SETA ++;
  
 }}
 if( TIMER_TRAVA_SETA >550){

   TIMER_TRAVA_SETA = 0;
   FLAG_TRAVA_SETA= 0;
   SIN_SETA = 0;
   
   
 }
 if (FLAG_TRAVA_SETA == 0) {
  SETA_PENDENTE_TRAVA = 0;
}
*/
// Inicia a janela quando o evento é armado
if (FLAG_TRAVA_SETA == 1 && TEMPO_TIMER_TRAVA_SETA == 0) {
    TEMPO_TIMER_TRAVA_SETA = millis();
   
}

// Timeout de 550 ms para a seta da trava
if (FLAG_TRAVA_SETA == 1 && millis() - TEMPO_TIMER_TRAVA_SETA >=550) {

    FLAG_TRAVA_SETA = 0;      // encerra a janela
    SIN_SETA = 0;             // invalida o pulso
    SETA_PENDENTE_TRAVA = 0;  // limpa evento pendente

    TEMPO_TIMER_TRAVA_SETA = 0; // libera para próxima execução
     
}

// Proteção adicional: se desarmou, zera o pendente
if (FLAG_TRAVA_SETA == 0) {
    SETA_PENDENTE_TRAVA = 0;
}


//===========================================================================================
//============================================================================================

if (FLAG_DESTRAVA_SETA == 1 && TEMPO_TIMER_DESTRAVA_SETA == 0) {
    TEMPO_TIMER_DESTRAVA_SETA = millis();
}
if (FLAG_DESTRAVA_SETA == 1 && millis() - TEMPO_TIMER_DESTRAVA_SETA >= 550
) {
    FLAG_DESTRAVA_SETA = 0;
    TIMER_DESTRAVA_SETA = 0;
    TEMPO_TIMER_DESTRAVA_SETA = 0;
}



// TERCEIRO PONTO — LIMPEZA OBRIGATÓRIA
if (FLAG_DESTRAVA_SETA == 0) {
  SETA_PENDENTE_DESTRAVA = 0;
}

//=================================================================================================

 

if (FLAG_DESTRAVA_SETA1 == 1 && TEMPO_TIMER_DESTRAVA_SETA1 == 0) {
    TEMPO_TIMER_DESTRAVA_SETA1 = millis();   // INICIA JANELA TEMPORAL
}
if (FLAG_DESTRAVA_SETA1 == 1 && millis() - TEMPO_TIMER_DESTRAVA_SETA1 >= 550) {

    FLAG_DESTRAVA_SETA1 = 0;       // encerra a validação
    TIMER_DESTRAVA_SETA1 = 0;      // limpa contador antigo
    TEMPO_TIMER_DESTRAVA_SETA1 = 0; // libera para próxima execução
}

if (FLAG_DESTRAVA_SETA1 == 0) {
  SETA_PENDENTE_DESTRAVA1 = 0;
}
 
 //===============================================================================================
 //==============================================================================================
//=========>JANELA DE TEMPO DE LEBRAÇÃO DE IGNIÇÃO PARA LIGAR O CARRO REMOTAMENTE<=============
//=============================================================================================

//=========>JANELA DE TEMPO DE LIBERAÇÃO DE IGNIÇÃO<==============================
//=================================================================================

if (XLI == 1 && TEMPO_ZERA_LIBERA_IGNICAO == 0) {
    TEMPO_ZERA_LIBERA_IGNICAO = millis();  // INICIA JANELA TEMPORAL
}

if (XLI == 1 && millis() - TEMPO_ZERA_LIBERA_IGNICAO >= 10000) { 
    // TEMPO VÁLIDO PARA APERTAR AS DUAS VEZES EM DESTRAVA PARA LIGAR O CARRO

    LIBERA_IGNICAO = 0;             // ESTOURANDO ESSE TEMPO, OS COMANDOS DE DESTRAVA SE TORNAM INVÁLIDOS PARA LIGAR O CARRO.
    XLI = 0;
    TEMPO_ZERA_LIBERA_IGNICAO = 0;  // LIBERA PARA PRÓXIMA EXECUÇÃO
}

 //=====JANELA DE TEMPO DE VALIDADE DE PERIODO DE DÁ PARTIDA<=================================================
 //==============================================================================================================================
 


if (ESTADO_CONTATO == HIGH && LIGAR_ESPERA_PARTIDA == 1 && TEMPO_ESPERA_PARTIDA == 0) {
    TEMPO_ESPERA_PARTIDA = millis();   // INICIA JANELA TEMPORAL DE ESPERA
}

if (ESTADO_CONTATO == HIGH && LIGAR_ESPERA_PARTIDA == 1 && millis() - TEMPO_ESPERA_PARTIDA >= 13000) {

    ESPERA_PARTIDA = 0;                 // encerra a contagem de espera
    LIGAR_ESPERA_PARTIDA = 0;           // bloqueia novos disparos de partida
    REST_LIGAR_ESPERA_PARTIDA = 1;      // indica que a janela expirou
    PAUSA_PARTIDA = 0;

    TEMPO_ESPERA_PARTIDA = 0;           // libera para a próxima execução
}

 if(REST_LIGAR_ESPERA_PARTIDA == 0){// REATIVA SISTEMA PARA NOVO COMANDO DE PARTIDA CASO TEMPO INICIAL TENHA ESTOURADO
  LIGAR_ESPERA_PARTIDA  = 1;

//================================================================================================================================= 
//================================================================================================================================= 
 }


//=============================================================================================================
//=====================================================================================================================



}

void EVENTOS_LIGAR_DESLIGAR_IGNICAO(){
//====>EVENTO DE LIGAR O CONTATO<==============================================================================
//=============================================================================================================
  
  if(SIN_DESTRAVA  == 2 &&  ESTADO_CONTATO == LOW  && LIBERA_IGNICAO  == 1 ){//CONDIÇÃO PARA LIGAR CONTATO
   BLOQUEIO_ARRANQUE = 1;
   digitalWrite(EMBREAGEM,LOW);
  //IGN_ON_OFF = 1;
  digitalWrite(CONTATO,HIGH);
   ESTADO_CONTATO = HIGH;
  //BLQ_BCM = 1;
  digitalWrite(CONTATO1,LOW);
  
  LIBERA_PARTIDA  = 0;
  
  TRAVA_PARTIDA = 1;  
  TEMPO_ARRANQUE = 0;
  TRAVA_PARTIDA1 = 1;

if(  AT_LIGAR_DESLIGAR_BIPE == 1){  
    
      CHAMA_BIPE(1, 80);
 }
   digitalWrite(ARRANQUE, LOW );
    LIGAR_ESPERA_PARTIDA = 1;
//SIN_SETA = 0;

  
}
//====>EVENTO DE DESLIGAR O CONTATO<==============================================================================
//=============================================================================================================

   if(SIN_DESTRAVA  == 2 &&  ESTADO_CONTATO == HIGH &&  AUX_DESLIGAR  == 1){//CONDIÇÃO PARA DESLIGAR CONTATO
 
 BLOQUEIO_ARRANQUE = 1;
 
 if (ESTADO_CONTATO == LOW) {
    digitalWrite(CONTATO1, LOW);
}
    LIBERA_PARTIDA = 0;
    TRAVA_PARTIDA = 1;
    TRAVA_PARTIDA1 = 1;
    ATIVA_ARRANQUE = 0;
    ESTADO_ARRANQUE = LOW;
    TEMPO_ARRANQUE = 0;
    ESPERA_PARTIDA = 0;
    LIGAR_ESPERA_PARTIDA = 0;
    PAUSA_PARTIDA = 0;
    SIN_TRAVA = 0;
    SIN_DESTRAVA = 0;

    ATIVA_BIPE = 0;
    QUANT_BIPAR = 0;
    LIGA_DESL_BUZINA = 0;
    digitalWrite(BIPE_BUZINA, LOW);

 //INT_EMBREAGEM = 0;
 digitalWrite(EMBREAGEM,LOW);
  //IGN_ON_OFF = 0;
  digitalWrite(CONTATO,LOW);
   ESTADO_CONTATO = LOW;
   LIBERA_PARTIDA  = 0;
  SIN_TRAVA = 0;
 // BLQ_BCM = 0;
  digitalWrite(CONTATO1, LOW);
 LIBERA_PARTIDA  = 0;
 TRAVA_PARTIDA = 1;  
 TEMPO_ARRANQUE = 0;
 digitalWrite(ARRANQUE, LOW );
  LIGAR_DESLIGAR_BIPE = 0;
  ATIVA_BIPE = 0;
  
 ESPERA_PARTIDA = 0;
// LIGAR_ESPERA_PARTIDA = 0;
 REST_LIGAR_ESPERA_PARTIDA = 0;
 //PAUSA_PARTIDA = 0;
  
}
  
}


void BIPE() {


  
    // LIGA/DESLIGA O BIPE AO LIGAR A IGNÇÃO DO CARRO PELO CONTROLE
    if (LIGAR_DESLIGAR_BIPE >=3) {
      LIGAR_DESLIGAR_BIPE = 0;
       TEMPO_FREQ_BIPE = millis();
        AT_LIGAR_DESLIGAR_BIPE = !AT_LIGAR_DESLIGAR_BIPE;
        EEPROM.update(5, AT_LIGAR_DESLIGAR_BIPE);      
        
 
    if (AT_LIGAR_DESLIGAR_BIPE == 1) {
        CHAMA_BIPE(1, 80);
    } else {
        CHAMA_BIPE(2, 80);
    }
      }
        

   
  // inicia o bipe

 

     // inicia o bipe
if (ATIVA_BIPE == 1 && TEMPO_FREQ_BIPE == 0) {
    TEMPO_FREQ_BIPE = millis();
    LIGA_DESL_BUZINA = 0;
    digitalWrite(BIPE_BUZINA, LOW);
}

// alterna buzzer
if (ATIVA_BIPE == 1 && millis() - TEMPO_FREQ_BIPE >= VELOCIDADE_BIPE) {

    TEMPO_FREQ_BIPE = millis();

    LIGA_DESL_BUZINA = !LIGA_DESL_BUZINA;
    digitalWrite(BIPE_BUZINA, LIGA_DESL_BUZINA);

    // terminou um bipe completo
    if (LIGA_DESL_BUZINA == 0) {
        if (NUM_BIPE > 0) NUM_BIPE--;

        if (NUM_BIPE == 0) {
            ATIVA_BIPE = 0;
            TEMPO_FREQ_BIPE = 0;
            VELOCIDADE_BIPE = 100;
            digitalWrite(BIPE_BUZINA, LOW);
        }
    }
 }

// fora do ciclo
if (ATIVA_BIPE == 0) {
    VELOCIDADE_BIPE = 100;
}

   }

void ALTERNAR_MODO_CHAVE(){

 //============================================================
// TRANSIÇÃO DO MODO REMOTO PARA O MODO CHAVE
//============================================================

// Inicia a contagem quando a chave é rodada no contato
if (ATIVA_MODO_CHAVE == 1 && ESTADO_CONTATO == HIGH && AUXIAR_MODO_CHAVE == 1 && TEMPO_MODO_CHAVE == 0) {
    TEMPO_MODO_CHAVE = millis();   // INICIA CONTAGEM PARA TRANSIÇÃO DE MODO
}

// Aguarda 2 segundos para liberar o modo chave
if (ATIVA_MODO_CHAVE == 1 && ESTADO_CONTATO == HIGH && AUXIAR_MODO_CHAVE == 1 && millis() - TEMPO_MODO_CHAVE >= 2500) {

    // DESLIGA TODOS OS RELÉS, LEVA O CARRO PARA POSIÇÃO ORIGINAL AO RODAR A CHAVE NO CONTATO,
    // APROVEITA A PARTIDA JÁ ACIONADA E CONTINUA COM O CARRO LIGADO COM A CHAVE ORIGINAL EM 2S.

    MODO_CHAVE = 0;
    ATIVA_MODO_CHAVE = 0;
    AUXIAR_MODO_CHAVE = 0;

    digitalWrite(EMBREAGEM, LOW);
    digitalWrite(CONTATO, LOW);
    ESTADO_CONTATO = LOW;

    SIN_TRAVA = 0;
    digitalWrite(CONTATO1, LOW);

    TEMPO_ARRANQUE = 0;
    LIBERA_PARTIDA = 0;
    TRAVA_PARTIDA = 1;
    ESPERA_PARTIDA = 0;
    LIGAR_ESPERA_PARTIDA = 0;
    REST_LIGAR_ESPERA_PARTIDA = 0;
    PAUSA_PARTIDA = 0;

    TEMPO_MODO_CHAVE = 0;   // LIBERA PARA A PRÓXIMA EXECUÇÃO
}
 
}
 void RESPOSTS_SIN_TRAVA_DESTRAVA(){

   static unsigned long T_TEMPO_SINAL_RESPOSTA = 0;

 //============================================================
// RESPOSTA SONORA / VISUAL DO CONTROLE (SINAL_RESPOSTA)
//============================================================

// Detecta evento de trava ou destrava
if ((digitalRead(SENS_DESTRAVA) == HIGH || digitalRead(SENS_TRAVA) == HIGH) && ATIVA_SINAL_RESPOSTA == 0) {
    ATIVA_SINAL_RESPOSTA = 1;
    T_TEMPO_SINAL_RESPOSTA = millis();  // INICIA PULSO DE RESPOSTA
}

// Mantém o sinal ativo por 100 ms
if (ATIVA_SINAL_RESPOSTA == 1 && millis() - T_TEMPO_SINAL_RESPOSTA >= 100) {
    ATIVA_SINAL_RESPOSTA = 0;
    digitalWrite(SINAL_RESPOSTA, LOW);
    T_TEMPO_SINAL_RESPOSTA = 0;
}

// Enquanto o pulso estiver ativo
if (ATIVA_SINAL_RESPOSTA == 1) {
    digitalWrite(SINAL_RESPOSTA, HIGH);
}

   }
   

 void TRAVA_IN_MARCHA() {

    uint8_t MARCHA_DETECTADA = digitalRead(SENS_NEUTRO);

    if (MARCHA_DETECTADA == HIGH && ESTADO_CONTATO == LOW && SIN_DESTRAVA == 1 && MARCHA_DETECT == 1 && ATIVA_BIPE == 0) {//SEGURANÇA DO SISTEMA EM MARCHA IMPEDIR LIGAR

         CHAMA_BIPE(3, 100);

        digitalWrite(EMBREAGEM, LOW);
        digitalWrite(CONTATO, LOW);
        ESTADO_CONTATO = LOW;
        digitalWrite(CONTATO1, LOW);
        digitalWrite(ARRANQUE, LOW);

        SIN_DESTRAVA = 0;
        MARCHA_DETECT = 0;

        // =====================================================
// ABORTA / INVALIDA COMPLETAMENTE O ARRANQUE
// (ex: carro em marcha, erro crítico, cancelamento)
// =====================================================

// 1Trava global do arranque
BLOQUEIO_ARRANQUE = 1;

// Cancela qualquer execução de arranque em andamento
ATIVA_ARRANQUE  = 0;
ESTADO_ARRANQUE = LOW;

// Remove autorização de partida
LIBERA_PARTIDA = 0;

// 4Travas de ciclo (evita reentrada)
TRAVA_PARTIDA  = 1;
TRAVA_PARTIDA1 = 1;

// 5Cancela janelas de espera da partida
LIGAR_ESPERA_PARTIDA       = 0;
PAUSA_PARTIDA              = 0;
ESPERA_PARTIDA             = 0;
REST_LIGAR_ESPERA_PARTIDA  = 0;

// Zera tempo do arranque
TEMPO_ARRANQUE = 0;

    }
}
  void BOT_EXTRA_PROG(){    
  static unsigned long TEMPO_LIGAR_DESLIGAR_BIPE = 0;
  static unsigned long TEMPO_CONTA_CLIQUE = 0;  
   EST_SENS_TRAVA1 =digitalRead(SENS_TRAVA);
   EST_SENS_DESTRAVA2 = digitalRead(SENS_DESTRAVA);
   EST_SENS_SETA3 =digitalRead(SENS_SETA);
   EST_SENS_SETA4 =digitalRead(SENS_SETA);
   

if (EST_SENS_TRAVA1 == LOW && ESTANT_SENS_TRAVA1 == HIGH && ESTADO_CONTATO ==LOW && EST_SENS_CONTATO == LOW ) {
if(LIBERA_LIGAR_DESLIGAR_BIPE == 1 ){
 XTB = 1;
 XTS_TR = 1;
 BOT_IN_TR = 0;
 FLAG_SENS_SETA_TR_ATRASO =1;
 TEMPO_ZERA_G_SEN_TR = millis(); 
 
TEMPO_ZERA_ATIVA_DES_BIPE = millis();


//if( EST_SENS_SETA4 == LOW){

//  SETA_PENDENTE_TRAVA3 = 1; 
   
//}
  
  ZERA_ATIVA_DES_BIPE = 0;
 
}
  }

ESTANT_SENS_TRAVA1 = EST_SENS_TRAVA1;

if(XTS_TR== 1 && millis()-TEMPO_ZERA_G_SEN_TR>700){// PARA SINCRONIZAR COM SETA
XTS_TR = 0;
AUX_BOT_IN_TR = 1; 
  
}

if (BOT_IN_TR == 0 && AUX_BOT_IN_TR == 1) {     // detecta o clique

    if (millis() - TEMPO_LIGAR_DESLIGAR_BIPE >= 180) {
        TEMPO_LIGAR_DESLIGAR_BIPE = millis();
        LIGAR_DESLIGAR_BIPE++;

        if (LIGAR_DESLIGAR_BIPE >3) {
            LIGAR_DESLIGAR_BIPE = 0;
       }

        AUX_BOT_IN_TR = 0;   // trava aqui até o botão soltar
    }
}

if (FLAG_SENS_SETA_TR_ATRASO == 1 && EST_SENS_SETA4 == LOW && ESTANT_SENS_SETA4 == HIGH) {
 
BOT_IN_TR = 1;

}
ESTANT_SENS_SETA4 = EST_SENS_SETA4;


//====================================================================================
//======================================================================================


if(EST_SENS_DESTRAVA2 == LOW && ESTANT_SENS_DESTRAVA2 == HIGH  && ESTADO_CONTATO ==LOW &&  EST_SENS_CONTATO == LOW){
   
 FLAG_SENS_SETA_TR_ATRASO = 0;
//SETA_PENDENTE_TRAVA3 = 0;
XTS_TR = 0;

  
  FLAG_SENS_SETA_DT_ATRASO  = 1;
  
  BOT_IN_DT = 0; 
  XTST = 1;
  TEMPO_ZERA_G_SEN_DT = millis();
  TEMPO_ZERA_LIBERA_LIGAR_DESLIGAR_BIPE= millis();
 
   //EST_SENS_SETA3 = LOW;   
   
  //if( EST_SENS_SETA3 ==LOW){

  //SETA_PENDENTE_DESTRAVA3 = 1;
  //}
  
}

if(XTST ==1 && millis()-TEMPO_ZERA_G_SEN_DT>700){//ATRASO PARA SINCRONIZAR TEMPO DE SETA
   
  XTST = 0;
  
  XTB1 = 1;
 
   if(BOT_IN_DT == 0 ){//SEPARA O EVENTO DE BOT DESTRVA COM SETA, DE BOT DESTRAVA SEM SETA
                                        //O EVENTO VERDADEIRO É BOT DESTRAVA SEM SETA
   
  LIBERA_LIGAR_DESLIGAR_BIPE = 1;
  TEMPO_LIGAR_DESLIGAR_BIPE = 0;
  //ZERA_LIBERA_LIGAR_DESLIGAR_BIPE = 0;
 
 
   } 
}

ESTANT_SENS_DESTRAVA2 = EST_SENS_DESTRAVA2;

 if (FLAG_SENS_SETA_DT_ATRASO == 1 && EST_SENS_SETA3 == LOW && ESTANT_SENS_SETA3 == HIGH) {//SPARA BOT DESTRAVA DA SETA
     
BOT_IN_DT = 1;
      
    }
ESTANT_SENS_SETA3 = EST_SENS_SETA3;    
    }
    void CHAMA_BIPE(uint8_t QTD, uint16_t VEL) {
    ATIVA_BIPE = 0;           // força reset do estado
    LIGA_DESL_BUZINA = 0;     // garante buzzer desligado
    NUM_BIPE = QTD;          // quantidade de bipes
    VELOCIDADE_BIPE = VEL;   // velocidade (ms)
    TEMPO_FREQ_BIPE = millis();
    // reinicia temporização
    ATIVA_BIPE = 1;          // arma novo ciclo
}


    //void RF_OUT_CANAL(){

//uint8_t SETA_PENDENTE_TRAVA3 = 0;
//uint8_t  FLAG_SENS_SETA_TR_ATRASO =0;
 //if(ANVIAR_RF == 2){
 
// mySwitch.send(148547365 ,28);
   // ANVIAR_RF = 0;
   // ATIVA_CONTA_CLIQUE = 0;
  //CONTA_CLIQUE = 0;
 // }}
