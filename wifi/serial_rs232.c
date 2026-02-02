/*
*
 *                               Comunicando via WIFI com modulo ESP8566
 *
 * Compilador : MPlabXC8
 * Microcontrolador: 32MK
 * Autor: aguivone
 * Versão: 1
 * Data :  02/02/2026
 * Atualizações :  19/02/2016 - Atualização do código para suporte ao PIC 32MK
 */
#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --------------------------------------------------------------------------
// DEFINIÇÕES DE LED — PIC32
// --------------------------------------------------------------------------
#define LED_CONEX   LATBbits.LATB3
#define LED_WEB     LATBbits.LATB5
#define LED_STATUS  LATBbits.LATB0

// --------------------------------------------------------------------------
// CONSTANTES DO SISTEMA
// --------------------------------------------------------------------------
#define TAM_MAX            40
#define NUM_MAX_TENTATIVA   3
#define TEMPO_RX          100
#define FIM_CONF         'x'
#define TIMEOUT_CONF      40

// --------------------------------------------------------------------------
// CONFIGURATION BITS (PIC32MK) — valores seguros e funcionais
// --------------------------------------------------------------------------

// CLOCK
#pragma config FNOSC    = SPLL      // Oscilador: System PLL
#pragma config POSCMOD  = HS        // Oscilador externo em modo HS
#pragma config FSOSCEN  = OFF       // Oscilador secundário desligado

// PLL (ajuste comum para 12 MHz de entrada)
#pragma config FPLLIDIV = DIV_3     // 12MHz /3 = 4 MHz
#pragma config FPLLMULT = MUL_50    // 4 MHz × 50 = 200 MHz
#pragma config FPLLODIV = DIV_2     // 200 MHz /2 = 100 MHz (F_SYS)

// PERIFÉRICOS
#pragma config FPBDIV   = DIV_1     // PBCLK = SYSCLK
#pragma config FWDTEN   = OFF       // Watchdog desligado
#pragma config FDMTEN   = OFF       // Deadman Timer off

// DEBUG E PROTEÇÃO
#pragma config DEBUG    = OFF
#pragma config ICESEL   = ICS_PGx2
#pragma config JTAGEN   = OFF
#pragma config CP       = OFF
#pragma config BWP      = OFF

//variaveis de tempo
int  giTempo_us=0;
long glTempo_ms=0;
long glTempo_led=0;
long glTempo_rx=0;

//variaveis da serial
char gcCaracter=0;
unsigned char gucStatus_serial='N';         //N = sem dados,R= recebendo dados , F=fim de recepção
unsigned char gucEstado_de_envio='0';       //0 = sem dados,V=dados validados , N=dados não validado
unsigned long gulTam_recebido=0;
unsigned char gucBuffer_serial[TAM_MAX];    //só pra testar

//variaveis da maquina de estado
unsigned char gucStado_do_modulo='A';       //inicia em A
unsigned long  gulTempo_de_espera=0;
unsigned long  gulTimeout_de_envio=0;
unsigned int  guiTentativa_envio=0;



//*******************************************************************************/
//===================================================================================
//Função:    _limpa_buffer
//Parâmetro: não tem.
//Retorno:   não tem.
//Descrição: usada para limpar o buffer de recpção.
//===================================================================================
void limpa_buffer()
{
    long tamanho = 0;
    while(tamanho<TAM_MAX)
    {
        gucBuffer_serial[tamanho]=0X00; 
        tamanho++;
    }    
}
//*******************************************************************************/
//===================================================================================
//Função:    _trata_int_conf
//Parâmetro: não tem.
//Retorno:   não tem.
//Descrição: usada para pegar as respostas das configurações.
//===================================================================================

void trata_int_conf()
{
            if(gucStatus_serial == 'R')
            {//primeiro dado
              gucBuffer_serial[gulTam_recebido] = gcCaracter; 
              gulTam_recebido++;
              if(gulTam_recebido>(TAM_MAX-1))
              { 
                  gucStatus_serial = 'N';//sobrescreve o vetor
              }
            }
            if(gucStatus_serial == 'N')
            {//primeiro dado
              gucBuffer_serial[0] = gcCaracter; 
              gulTam_recebido =1;
              gucStatus_serial = 'R';
            } 
}
//*******************************************************************************/
//===================================================================================
//Função:    trata_int_dados
//Parâmetro: não tem.
//Retorno:   não tem.
//Descrição: usada para pegar os dados(após a configuração).
//===================================================================================
void trata_int_dados()
{
     if(gcCaracter == '\n')//caracter de retorno de carro "LR" (depois vejo, talvez verificar se tem o \n))
            {//final da frase
             if((gucBuffer_serial[gulTam_recebido-2]=='D')||(gucBuffer_serial[gulTam_recebido-2]=='T'))
                {//verifica se está abrindo ou fechando o socket(comunicação)                 
                 if((gucBuffer_serial[gulTam_recebido-5]=='N')&&(gucBuffer_serial[gulTam_recebido-4]=='E')&&(gucBuffer_serial[gulTam_recebido-3]=='C'))
                    {//abrindo conexão recebe "CONNECT"
                       LED_CONEX = 1;
                       gulTam_recebido=0;//zera variavel de tamanho
                       gucStatus_serial = 'N';//prepara para pegar novos dados
                    }
                  if((gucBuffer_serial[gulTam_recebido-5]=='O')&&(gucBuffer_serial[gulTam_recebido-4]=='S')&&(gucBuffer_serial[gulTam_recebido-3]=='E'))
                    {//fechando conexão recebe "CLOSED"
                       LED_CONEX = 0;
                       gulTam_recebido=0;//zera variavel de tamanho
                       gucStatus_serial = 'N';//prepara para pegar novos dados
                    }                   
                }
             
            }
            if(gucStatus_serial == 'R')
            {//está recebendo dados
              gucBuffer_serial[gulTam_recebido] = gcCaracter; 
              gulTam_recebido++;
              if(gulTam_recebido>TAM_MAX-2)
              {//gera aviso de estouro pois o buffer de recpção não consegue armazenar o pacote todo.
                  gucStatus_serial = 'E';
              }
            }
            if(gucStatus_serial == 'N')
            {//primeiro caracter capturado 
              gucBuffer_serial[0] = gcCaracter; 
              gulTam_recebido =1;
              gucStatus_serial = 'R';
            } 
}
//*******************************************************************************/
//===================================================================================
//Função:    _interrupcoes
//Parâmetro: não tem.
//Retorno:   não tem.
//Descrição: usada para tratar as interrupções geradas pelo microcontrolador.
//===================================================================================
void interrupt interrupcoes(void)//vetor de interrupção
 {
   if(T0IF)
    {//rotinas de tempo(15us))
        giTempo_us++;
        TMR0 = 254;//reinicia timer com o valor calculado
        T0IF = 0;
        if(giTempo_us>66)//1ms
        {//executa a cada 1ms
            giTempo_us = 0;
            glTempo_ms++;
            if(glTempo_rx > 0)
            {
              glTempo_rx--;
            }
            if(gulTempo_de_espera > 0)
            {
              gulTempo_de_espera--;
            }
            if(glTempo_led>0)
            {
              glTempo_led--;  
            }
            if(glTempo_ms>1000)
            {//executa a cada 1s
               glTempo_ms=0;
               gulTimeout_de_envio++;
            }
        }
        //62.5us + os delay da rotinas  = 75us
    }
   if(RCIF)//verifica interrupção da serial
    {//interrupção da serial
        gcCaracter = RCREG;
        if(gucStado_do_modulo == FIM_CONF+1)
            {//Modulo já foi configurado
               trata_int_dados();
            }
        else
        {
           trata_int_conf();
        }
      glTempo_rx = TEMPO_RX;  
      RCIF =0;//limpa flag de interrupçao
    }
   
}
//===================================================================================
//Função:    _inicializa_CPU
//Parâmetro: não tem.
//Retorno:   não tem.
//Descrição: configura o microcontrolador.
//===================================================================================
void inicializa_CPU(void)
{
    // -------------------------------------------------------
    // 1. Configuração dos pinos (RC0/RC1)
    // -------------------------------------------------------
    ANSELCbits.ANSELC0 = 0;   // RC0 digital
    ANSELCbits.ANSELC1 = 0;   // RC1 digital

    TRISCbits.TRISC0 = 0;     // TX2 = saída
    TRISCbits.TRISC1 = 1;     // RX2 = entrada

    LATC = 0;                 // zerar saídas do PORTC

    // -------------------------------------------------------
    // 2. Peripheral Pin Select – associa UART2 aos pinos RC0/RC1
    // -------------------------------------------------------
    RPC0R = 0b0001;           // RC0 = TX2  (saída)
    U2RXR = 0b0010;           // RC1 = RX2  (entrada)

    // -------------------------------------------------------
    // 3. Inicialização da UART2 a 115200 baud
    // -------------------------------------------------------
    U2MODE = 0;               // modo padrão
    U2MODEbits.BRGH = 1;      // alta velocidade (BRGH = 1)

    // BRG = (Fpb / (4 * Baud)) - 1   → supondo Fpb = 40 MHz
    U2BRG = 86;               // valor aproximado para 115200

    U2STA = 0;
    U2STAbits.UTXEN = 1;      // habilitar TX
    U2STAbits.URXEN = 1;      // habilitar RX

    U2MODEbits.ON = 1;        // liga UART2

    // -------------------------------------------------------
    // 4. Timer equivalente ao TIMER0 (usamos Timer2)
    // -------------------------------------------------------
    T2CON = 0;
    T2CONbits.TCKPS = 0b101;  // prescaler = 1:32
    PR2 = 24999;              // período
    TMR2 = 0;
    T2CONbits.ON = 1;

    IEC0bits.T2IE = 1;        // habilita interrupção do timer2
    IFS0bits.T2IF = 0;

    // -------------------------------------------------------
    // 5. Habilita interrupções globais
    // -------------------------------------------------------
    __builtin_enable_interrupts();

    // -------------------------------------------------------
    // 6. LEDs da placa
    // (RA10 = LED1 conforme manual)
    // -------------------------------------------------------
    ANSELAbits.ANSELA10 = 0;
    TRISAbits.TRISA10 = 0;
    LATA10 = 0;               // LED apagado
}
//===================================================================================
//Função:    _Verifica_resposta
//Parâmetro: char prox,char anterior,unsigned char procurar[].
//Retorno:   não tem.
//Descrição: verifica se o comando foi aceito.
//===================================================================================
void Verifica_resposta(char prox,char anterior,unsigned char procurar[])
{    
    if(glTempo_rx == 0)//espera receber o comando
            {
                if(BuscaPalavra(gucBuffer_serial,procurar) != 0XFFFF)
                {  
                  gucStado_do_modulo = prox;//vai para proximo passo
                  guiTentativa_envio = 0;
                }
               else
                {
                  if(guiTentativa_envio >= NUM_MAX_TENTATIVA)
                   {///registra falha na inicialização
                    gulTimeout_de_envio = 250;//Deu erro na configuração
                   } 
                  else
                  {
                   gucStado_do_modulo = anterior;//passo anterior
                  }
                }
            }
}
//*******************************************************************************/
//===================================================================================
//Função:    Envia_comando
//Parâmetro: unsigned char dados[],char mod,int multi_tempo.
//Retorno:   não tem.
//Descrição: usada para enviar comandos.
//===================================================================================
void Envia_comando(unsigned char dados[],char mod)
   {
        PEIE = 0;//desabilita interrupção de perifericos do pic        
        gulTimeout_de_envio=0;
        gucStado_do_modulo = mod;
        limpa_buffer();
        gucStatus_serial = 'N';//põe pra ir pro inicio do vetor
        gulTam_recebido =0;//zera o tamanho
        imprime_RS232(dados);        
        glTempo_rx = TEMPO_RX*3;//seta tempo de recepção de dados um pouco maior para primeira amostra  
        guiTentativa_envio++; 
        PEIE = 1;//habilita interrupção de perifericos do pic   
    }
//===================================================================================
//Função:    _Inicializa_ESP8266
//Parâmetros: não tem.
//Retorno:   não tem.
//Descrição: é uma maquina de estado que configura o modulo ESP8266.
//===================================================================================
//maquina de estado de inicialização do  modulo
void Inicializa_ESP8266(void)
{
    switch(gucStado_do_modulo)
   {
       case 'A'://teste inicial pra saber se o modulo está ativo
       { 
           Envia_comando("AT\r\n",'B');//comando , proximo passo
       }break;
       case 'B'://verifica resposta
       { 
            if(glTempo_rx == 0)//espera receber o comando
            {
                if(BuscaPalavra(gucBuffer_serial,"OK") != 0XFFFF)
                {  
                  gucStado_do_modulo = 'C';//vai para proximo passo
                  guiTentativa_envio = 0;
                }
               else
                {                    
                  gucStado_do_modulo = 'A';//passo anterior
                }
            }
       }break;       
       case 'C':
       { //configura o modo que o modulo vai funcionar, aqui deixei para funcionar como AP
        //   Envia_comando("AT+CWMODE=3\r\n",'D');//comando , proximo passo  - AP ou servidor
          Envia_comando("AT+CWMODE=2\r\n",'D');//comando , proximo passo - AP
        //   Envia_comando("AT+CWMODE=1\r\n",'D');//comando , proximo passo - servidor
       }break;
       case 'D'://verifica resposta
       { 
           Verifica_resposta('E','C',"OK");//colopra onde vai e qual é a palavra procurado
       }break;
       case 'E'://manda resetar o modulo 
       {    
           Envia_comando("AT+RST\r\n",'F');//comando , proximo passo  
       }break;
       case 'F'://verifica resposta
       {
           gulTimeout_de_envio=0;//zera variavel de contagem
           while(gulTimeout_de_envio < 15);//espera pelo menos 10segundos para resetar com segurança
           gulTimeout_de_envio=0;//zera variavel novamente
           Verifica_resposta('G','E',"ready");
       }break;
       case 'G':
       { //configura o modo que o modulo vai funcionar, aqui deixei para funcionar como AP
           Envia_comando("ATE0\r\n",'H');//pede pra não repetir o comando
       }break;
       case 'H'://verifica resposta
       { 
           Verifica_resposta('I','G',"OK");//colopra onde vai e qual é a palavra procurado
       }break;       
       case 'I':
       { 
           Envia_comando("AT+CIPMUX=1\r\n",'J');//comando , proximo passo//cofigura para conexões multiplas
         //  Envia_comando("AT+CIPMUX=0\r\n",'J');//comando , proximo passo//cofigura para conexao simples
       }break;
       case 'J'://verifica resposta
       { 
           Verifica_resposta('L','I',"OK");
       }break;       
       case 'L'://configura DHCP e modo de operação
       { //quando DHCP é ligado ele busca ip automaticamente
       //   Envia_comando("AT+CWDHCP=2,1\r\n",'M');//só aceitou o modo AP+STATION/dhcp ligado.(pega ip automatico))
       //  Envia_comando("AT+CWDHCP=2,0\r\n",'M');//AP+STATION/dhcp desligado.(para usar ip fixo))
             Envia_comando("AT+CWDHCP=0,0\r\n",'M');//modo AP(o sinal fica mais estavel)/DHCP desligado(para usar ip fixo) 
       }break;       
       case 'M'://verifica resposta
       { 
           Verifica_resposta('N','L',"OK");
       }break;       
       case 'N'://Nome do ponto de wifi,senha,canal(deixei o canal de freq.7)),tipo de segurança(0 = aberta))
       { //
          Envia_comando("AT+CWSAP=\"MICROCONTROLARES-C\",\"senha\",7,0\r\n",'O');
       }break;  
       case 'O'://verifica resposta
       { 
           Verifica_resposta('P','N',"OK");
       }break;
       case 'P':
       { //configura para criar servidor na porta 5000
           Envia_comando("AT+CIPSERVER=1,5000\r\n ",'Q'); 
        }break;
       case 'Q'://verifica resposta
       {             
           Verifica_resposta('R','P',"OK");
       }break;              
       case 'R':
       { //configura para criar servidor na porta 5000
           Envia_comando("AT+CIPAP=\"192.168.5.1\" \r\n",'S');//usa este IP para gateway e para o servidor 
        }break;
       case 'S'://verifica resposta
       {             
           Verifica_resposta('T','R',"OK");
       }break;       
       case 'T':
       { //seta time out
           Envia_comando("AT+CIPSTO=100\r\n",'U');//comando , proximo passo  
        }break;
       case 'U'://verifica resposta
       {             
                gucStado_do_modulo = FIM_CONF;//não precisa de confimação
       }break;
     
           
       
    } 
}
//===================================================================================
//Função:    _envia_pacote
//Parâmetros: long *tamanho,char *pacote[].
//Retorno:   não tem.
//Descrição: usado para o modo socket(sem usar uma pagina web, envia um pacote TCP).
//===================================================================================
void envia_pacote(long *tamanho,char *pacote[])
{
   char valor[]="0000";
   int pos = LongToChar(tamanho,valor);
   imprime_RS232("AT+CIPSEND=0,");//para conexão multipla informa o canal 
  // imprime_RS232("AT+CIPSEND=");//na conexoes simples não precisa informar o canal
        while(pos<4)
        {
            escreve_RS232(valor[pos]);  
            pos++;  
        }
   escreve_RS232(0X0D);
   escreve_RS232(0X0A);
    glTempo_rx = TEMPO_RX/2;
    while(glTempo_rx > 0);//espera receber a resposta do comando 
    pos=0;// 
        while(pacote[pos] != 0X00 )//envia os dados recebidos novamente
            { //imprime na porta TCP os dados recebidos(o tamanho max aqui será uns 30 caracteres))
               escreve_RS232(pacote[pos]);
               pos++;
            } 
    escreve_RS232(0X0D);
    escreve_RS232(0X0A);
}
//===================================================================================
//Função:    _envia_pagina
//Parâmetros: não tem.
//Retorno:   não tem.
//Descrição: Monta e envia a pagina HTML.
//===================================================================================
void envia_pagina()
{//lembre que \" = " logo conta como 1 caracter e não 2
   limpa_buffer();//solicita pra limpar o buffer serial 
   ///como vou conectar apenas 1 então o canal é 0 
   imprime_RS232("AT+CIPSEND=0,285");//para conexão multipla informa o canal (prepara envio e tamanho))
  // imprime_RS232("AT+CIPSEND=");//na conexoes simples não precisa informar o canal
    escreve_RS232(0X0D);
    escreve_RS232(0X0A);
    glTempo_rx = TEMPO_RX;
    while(glTempo_rx > 0);//espera receber a resposta do comando 
    imprime_RS232("<html><head><title>Teste</title></head><h1>PROGRAMA DE TESTE</h1>");
    imprime_RS232("<body><p><form method=\"GET\"></p>");
    imprime_RS232("<p><input type=\"radio\" name=\"Bot\" value=\"1\" CHECKED> Ligar led.</p>");
    imprime_RS232("<p><input type=\"radio\" name=\"Bot\" value=\"0\" > Desligar led.</p>");
    imprime_RS232("<p><input type=\"submit\"  value=\"Enviar\"></p></body></html>");
    escreve_RS232(0X0D);
    escreve_RS232(0X0A);
}
//////////////////////////////////////////////////////Rotina principal///////////////////////////////////////////////////////////////
//===================================================================================
//Função:    _main
//Parâmetros: não tem.
//Retorno:   não tem.
//Descrição: função principal.
//===================================================================================
///////////////http://microcontroladores-c.blogspot.com.br//////////////////////////////////////////
void main(void) 
{
    inicializa_CPU();
     for(;;)
    {        
        if(glTempo_led == 0)
        {//led de status            
            LED_STATUS = ~LED_STATUS;
            if(gucStado_do_modulo == FIM_CONF+1)
            {//Modulo pronto pra funcionar
              glTempo_led = 100;//tempo mais rapido  
            }
            else
            {
               glTempo_led = 500;//tempo normal 
               if(gulTimeout_de_envio>TIMEOUT_CONF)//se for maior que 30S o tempo de espera então indica falha
                {
                   LED_WEB = ~LED_WEB; //modulo não responde - falha
                }
            }
        }
        if(gucStado_do_modulo == FIM_CONF+1)
            {//Modulo pronto pra funcionar
                if((glTempo_rx == 0)&&(gulTam_recebido<8))//tamanho minimo em cada recpção de dados
                 {//descarta
                     gulTam_recebido=0;//zera variavel de tamanho
                       gucStatus_serial = 'N';//prepara para pegar novos dados
                 }
                if(gucStatus_serial == 'E')//espera terminar tempo de time out
                {//houve estouro do buffer logo a comunicação é via pagina HTTP 
                    if(BuscaPalavra(gucBuffer_serial,"GET") != 0XFFFF)//conexão por modo get
                        {//pagina web solicitada ou resposta recebida
                             if(BuscaPalavra(gucBuffer_serial,"GET / HTTP") != 0XFFFF)//solicitou pagina web
                                 envia_pagina();
                            if(BuscaPalavra(gucBuffer_serial,"GET /?Bot") != 0XFFFF)//resposta da pagina web 
                               {
                                  if(BuscaPalavra(gucBuffer_serial,"Bot=0") != 0XFFFF)//vê se é pra desligar led
                                    LED_WEB=0;
                                  if(BuscaPalavra(gucBuffer_serial,"Bot=1") != 0XFFFF)//vê se é pra desligar led
                                    LED_WEB=1;
                               }                                                                                      
                        }                    
                       gulTam_recebido=0;//zera variavel de tamanho
                       gucStatus_serial = 'N';//prepara para pegar novos dados 
                   
                }
                else
                {//é comunicação via socket
                    if((glTempo_rx == 0)&&(gulTam_recebido>8))//tamanho minimo
                    {//verifica se tem dados pra ler pois o tempo limite pra finalizar recpção já passou
                          if(BuscaPalavra(gucBuffer_serial,"+IPD") != 0XFFFF)
                           {//chegou dados validado
                               long auxiliar = BuscaPalavra(gucBuffer_serial,":");
                               long tamanho ;//tamanho dos dados
                               long pos_buffer=0;//tamanho dos dados
                               unsigned char gucBuffer_tratamento[TAM_MAX];
                               if(auxiliar != 0XFFFF)
                               {//pega o tamanho dos dados
                                   if(CharToInt(gucBuffer_serial[auxiliar-2])<10)
                                        {// o tamanho dos dados é maior que 10
                                             tamanho = (10*CharToInt(gucBuffer_serial[auxiliar-2]))+CharToInt(gucBuffer_serial[auxiliar-1]);
                                            if(CharToInt(gucBuffer_serial[auxiliar-3])<10)
                                             {
                                              tamanho = (100*CharToInt(gucBuffer_serial[auxiliar-3]))+tamanho; 
                                             }                                             
                                        }
                                        else
                                        {//o tamanho é menor que 10
                                          tamanho = CharToInt(gucBuffer_serial[auxiliar-1]);
                                        } 
                                       auxiliar++;//pra não pegar o ":"
                                       while( pos_buffer < tamanho )
                                       { //copia os dados para o vetor de tratamento
                                           gucBuffer_tratamento[pos_buffer] = gucBuffer_serial[auxiliar+pos_buffer];
                                           pos_buffer++;
                                       } 
                                       gucBuffer_tratamento[pos_buffer]=0X00;//caracter null 
                               }
                               //limpa_buffer();//solicita pra limpar o buffer serial   
                               envia_pacote(tamanho,gucBuffer_tratamento);
                           }                           
                          gulTam_recebido=0;//zera variavel de tamanho
                          gucStatus_serial = 'N';//prepara para pegar novos dados
                    }                 
                }        
            }
        else
        {//durante inicialização
            if(gucStado_do_modulo < FIM_CONF)
            {//inicializa o modulo
               if(gulTimeout_de_envio<TIMEOUT_CONF)//se for menor que 30S o tempo de espera terminar configuração           
                {
                    Inicializa_ESP8266();
                }
            }
            if(gucStado_do_modulo == FIM_CONF)
            {
             ///neste momento a WIFI já esta configurada e pronta pra receber dados   
              gucStado_do_modulo = FIM_CONF+1;
            }    
        }        
    }//loop infinito

}

O código do arquivo serial:

/*
 *                          FUNÇOES USADAS NA SERIAL 
 *
 * Compilador :      MPlabXC8
 * Microcontrolador: 16F877A
 * Autor:            Aguivone
 * Versão:           1.0.1
 * Descrição:   ->Assim como o printf tem a função de comunicar via rs232, mas com o diferencial de ter um menor processamento e gasto de memoria
 *              ->Mas exige um pouco mais de trabalho para imprimir caracteres numericos 
 * Data de criação:  08/09/2014.
 * Data de atualização : 29/01/2016 - melhorando o baud rate para altas velocidades
 */

#include <xc.h>
#include <string.h> //para usar funçoes de string deve se adicionar este header

//===================================================================================
//Função:    _inicializa_RS232
//Parâmetros: unsigned long ulVelocidade
//          : unsigned int uiModo
//Retorno:   não tem retorno.
//Descrição: usada para iniciar a porta serial com velocidades até 38400.
//===================================================================================
void inicializa_RS232(float fVelocidade,unsigned int uiModo,float fClock)
{//// por padrão é usado o modo 8 bits e sem paridade, mas se necessario ajuste aqui a configuração desejada.
    // verifique datasheet para ver a porcentagem de erro e se a velocidade é possivel para o cristal utilizado.
    RCSTA = 0X90;//habilita porta serial,recepção de 8 bit em modo continuo,assincrono.
    float valor;
        if(uiModo == 1)
        {//modo = 1 ,modo alta velocidade
         TXSTA = 0X24;//modo assincrono,trasmissao 8 bits.
        // valor =(((_XTAL_FREQ/ulVelocidade)-16)/16);//calculo do valor do gerador de baud rate
         valor =(((fClock/fVelocidade)-16)/16);//calculo do valor do gerador de baud rate
        }
        else
        {//modo = 0 ,modo baixa velocidade
         TXSTA = 0X20;//modo assincrono,trasmissao 8 bits.
         //valor =(((_XTAL_FREQ/ulVelocidade)-64)/64);//calculo do valor do gerador de baud rate
         valor =(((fClock/fVelocidade)-64)/64);//calculo do valor do gerador de baud rate
        }
    SPBRG =(int)valor;
   // PIE1 = 0X20;
    RCIE = 1;//habilita interrupção de recepção
    TXIE = 0;//deixa interrupção de transmissão desligado(pois corre se o risco de ter uma interrupção escrita e leitura ao mesmo tempo)
}
//===================================================================================
//Função:    _inicializa_RS232_HS
//Parâmetros: unsigned long ulVelocidade
//          : unsigned int uiModo
//Retorno:   não tem retorno.
//Descrição: usada para iniciar a porta serial com velocidades acima 38400.
//===================================================================================
void inicializa_RS232_HS(float fVelocidade,int iClock)//o clock em mhz
{//// por padrão é usado o modo 8 bits e sem paridade, mas se necessario ajuste aqui a configuração desejada.
    // verifique datasheet para ver a porcentagem de erro e se a velocidade é possivel para o cristal utilizado.
    RCSTA = 0X90;//habilita porta serial,recepção de 8 bit em modo continuo,assincrono.
    TXSTA = 0X24;//modo assincrono,trasmissao 8 bits.modo = 1 ,modo alta velocidade
    switch(iClock)//cadastrei somente os cristais mais usados
   {
       case 4:
       { 
           if(fVelocidade > 57600)
           {//é 115200
             SPBRG =1;  
           }
           else
           {//é 57600
            SPBRG =3;   
           }
       }break;
       case 10:
       { 
           if(fVelocidade > 57600)
           {//é 115200
             SPBRG =4;  
           }
           else
           {//é 57600
            SPBRG =10;   
           }
       }break;
       case 16:
       { 
           if(fVelocidade > 57600)
           {//é 115200 ou 250000
               if(fVelocidade == 115200)
               {//é 115200
                 SPBRG =8; 
               }
               else
               {//é 250000
                 SPBRG =3;  
               }
           }
           else
           {//é 57600
            SPBRG =16;   
           }
       }break;
       case 20:
       { 
           if(fVelocidade > 57600)
           {//é 115200,250000,625000 ou 1250000(1,25Mbs))
               if(fVelocidade == 115200)
               {//é 115200
                 SPBRG =10; 
               }
               if(fVelocidade == 250000)
               {
                 SPBRG =4; 
               }
               if(fVelocidade == 625000)
               {
                 SPBRG =1; 
               }
               if(fVelocidade == 1250000)
               {
                 SPBRG =0; 
               }  
           }
           else
           {//é 57600
            SPBRG =21;   
           }
       }break;
    }
   // PIE1 = 0X20;
    RCIE = 1;//habilita interrupção de recepção
    TXIE = 0;//deixa interrupção de transmissão desligado(pois corre se o risco de ter uma interrupção escrita e leitura ao mesmo tempo)
}
//===================================================================================
//Função:    _escreve_RS232
//Parâmetros: char cValor
//Retorno:   não tem retorno.
//Descrição: usada para escrever 1 caracter.
//===================================================================================
 void escreve_RS232(char cValor)
{
    TXIF = 0;//limpa flag que sinaliza envio completo.
    TXREG = cValor;
    while(TXIF ==0);//espera enviar caracter
}
//===================================================================================
//Função:    _imprime_RS232
//Parâmetros: const char ccFrase[]
//Retorno:   não tem retorno.
//Descrição: usada para escrever uma string.(maximo 255 caracteres)
//===================================================================================
 void imprime_RS232(const char *ccFrase[])
{
     unsigned char indice = 0;
     unsigned char tamanho = strlen(ccFrase);
      while(indice < tamanho ) ///veja que o programa pode travar se aqui não tiver as duas aspas
       {
          escreve_RS232(ccFrase[indice]);
           indice++;
       }
}
//===================================================================================
//Função:    _imprime_vetor_RS232
//Parâmetros: const char ccFrase[]
//Retorno:   não tem retorno.
//Descrição: usada para escrever uma vetor de caracteres.(maximo 255 caracteres))
//===================================================================================
 void imprime_vetor_RS232(unsigned char *ccFrase[])
{
     unsigned char indice = 0;
     unsigned char tamanho = strlen(ccFrase);
      while(indice < tamanho ) 
       {
           escreve_RS232(ccFrase[indice]);
           indice++;
       }
}
 //===================================================================================
//Função:    _BuscaPalavra
//Parâmetros: long ulQuant
//Retorno:   se = 0 não tem palavra igual, senão retorna a posição+1.
//Descrição: buscar uma palavra em um vetor de caracteres(máximo 65535 caracteres) e retorna primeira posição)
//===================================================================================
// As funçoes "strstr()" e "strspn()" não funcionaram bem por isto resolvi montar esta função
 //fica mais rapida
int BuscaPalavra(unsigned char *ccFrase[],unsigned char *palavra[])
{
     unsigned int indice = 0;
     unsigned int tamanho_palavra = strlen(palavra);
     unsigned int tamanho_vetor = strlen(ccFrase) - tamanho_palavra;//tamanho do vetor subtraido do tamanho da palavra
      while(indice <= tamanho_vetor) //tamanho do vetor subtraido do tamanho da palavra
       {
          if(palavra[0]==ccFrase[indice])
          {//se for parecido com a primeira letra da palavra então testa se é a palavra certa
              unsigned int tamanho_comparado=1;//inicia comparando próxima letra
              while(tamanho_comparado < tamanho_palavra) 
                 {
                      if(palavra[tamanho_comparado]!=ccFrase[indice+tamanho_comparado])
                       {
                         tamanho_comparado=tamanho_palavra+1;
                         indice = indice + tamanho_comparado;//já pula pra frente pois foi testado)
                       }
                    tamanho_comparado++;                      
                 }
              if(tamanho_comparado<tamanho_palavra+1)
              {
                return(indice); //retorna a posição da primeira ocorrencia da palavra 
              }
                 
          }
           indice++;
       }
     return(0XFFFF);//indica que não achou palavra igual
} 
 //===================================================================================
//Função:    _LongToChar
//Parâmetros: long ulQuant
//Retorno:   não tem retorno.
//Descrição: usada para escrever um numero(int ou long) pela serial
//===================================================================================
int LongToChar(long ulQuant,char *valor[])
{ 
    int ret=3;//indica apartir de quando pegar os dados para não imprimir 0 antes do numero
    if(ulQuant>=1000)//mas se quiser imprimir o zero basta usar o vetor inteiro
    {ret=0;}
    else
    {if(ulQuant>=100)
            {ret=1;}
            else
            {if(ulQuant>=10)
                {ret=2;}
            }
    }
            while(ulQuant>=1000)
            {
             ulQuant=ulQuant-1000;
             valor[0]++;
             }
            while(ulQuant>=100)
            {
             ulQuant=ulQuant-100;
             valor[1]++;
             }
             while(ulQuant>=10)
            {
             ulQuant=ulQuant-10;
             valor[2]++;
             }
            while(ulQuant>=1)
            {
             ulQuant=ulQuant-1;
             valor[3]++;
             }
    return(ret);
}
//===================================================================================
//Função:    _CharToInt
//Parâmetros: unsigned char convert_char
//Retorno:   retorna um numero.
//Descrição: usada para conveter um caracter em um numero
//===================================================================================
int CharToInt(unsigned char convert_char)
{
     switch(convert_char)
     {
         case '0':
         return(0);
         case '1':
         return(1);
         case '2':
         return(2);
         case '3':
         return(3);
         case '4':
         return(4);
         case '5':
         return(5);
         case '6':
         return(6);
         case '7':
         return(7);
         case '8':
         return(8);
         case '9':
         return(9);
         default:
             return(10);
     }
}
