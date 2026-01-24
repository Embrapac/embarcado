# 📊 Repositório para agrupamento do projeto do sistema embarcado da Embrapac:
Parte integrante do fluxo de trabalho do projeto final da residência do CPQD.

## Dispositivo em uso: 

PIC32MK MCACURIOSITYPRODEVELOPMENTBOARD


| **Descrição** | **Imagem** |
| :--- | :--- |
| **Vista baixa da PCB** | ![Vista baixa da PCB:](https://github.com/Embrapac/embarcado/blob/main/images/1.pic_pcb_view.jpeg) |
| **Divisão interna de blocos do CI** | ![Divisão interna de blocos do CI:](https://github.com/Embrapac/embarcado/blob/main/images/2.representacao_de_topo.png) |
| **Interface dedicada Arduino** | ![Interface dedicada Arduino:](https://github.com/Embrapac/embarcado/blob/main/images/3.interface_arduino.png) |
| **Interface de teste** | ![Interface de teste:](https://github.com/Embrapac/embarcado/blob/main/images/4.pinos_blink_control.png) |
| **Pinos dedicados ao sistema de controle de motores 1** | ![Lista da interface de controle de motores 1](https://github.com/Embrapac/embarcado/blob/main/images/5.lista_da_interface_de_controle_de_motores_1.png) |
| **Pinos dedicados ao sistema de controle de motores 2** | ![Lista da interface de controle de motores 2](https://github.com/Embrapac/embarcado/blob/main/images/6.lista_da_interface_de_controle_de_motores_2.png) |
| **Pinos dedicados para as entradas do banco 1** | ![Pinos dedicados para as entradas do banco 1:](https://github.com/Embrapac/embarcado/blob/main/images/7.lista_de_pinos_entrada_1.png) |
| **Pinos dedicados para as entradas do banco 2** | ![Pinos dedicados para as entradas do banco 2:](https://github.com/Embrapac/embarcado/blob/main/images/8.lista_de_pinos_entrada_2.png) |


## Ferramenta necessária para desenvolvimento no PIC: 
https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide

### Passos para o desenvolvimento:


1. Acesse a aba **File**, no canto superior esquerdo.
2. Em seguida aparecerão dois campos principais: **Categories** e **Projects**.  
   - Em *Categories*, escolha **Microchip Embedded**.  
   - Em *Projects*, selecione **Application Project(s)**.
3. Clique em **Next** no canto inferior do sub-menu.
4. Em **Family**, escolha a família correspondente ao seu dispositivo.  
   No nosso caso: **32-BIT MCUs (PIC32)**.
5. Em **Device**, selecione **PIC32MK0128MCA028**.
6. Em **Tool**, escolha **Simulator**, pois não estamos usando o hardware físico.
7. Clique em **Next** e selecione o compilador **XC32**.
8. Em **Project Name**, use **embrapac**.  
   No campo de localização do projeto, use o diretório onde clonou, em nosso caso:  
   ```
   /home/matheus/Área de trabalho/projeto/embarcado
   ```
9. Após finalizar, o programa criará a pasta **embrapac.X**, onde ficarão todos os arquivos do projeto.

## Comparativo simples de um circuito blink no Arduino vs PIC:

O que é um circuito Blink? Um circuito de teste simples, onde é alocado um led de uma cor qualquer, habitualmente o vermelho, de modo que o mesmo, alterne entre os estados ligado/desligado, a cada ciclo de tempo pré-determinado, em ambos os casos, usaremos um ciclo de 1s para cada comutação.

Além disso, é necessário utilizar um resistor para controle do potencial que alimenta o LED, este dispositivo pode ser dimensionado através desta fórmula:

## Cálculo do resistor para o LED:
<br>

$$
R(\Omega) = \frac{V_{pp} - V_{led}}{I_{led}}
$$ 

<br>

$$
P(W) = U*i
$$

<br>
O led em carga plena utiliza 1.8~2V, afim de cálculo, considerarei 1.9V, e sua corrente é de 20mA (0.02A)

### Para o Arduino a tensão de trabalho é de 5V:

<br>

$$
R(\Omega) = \frac{5 - 1.9}{0.02} = \frac{3.1}{0.02} = 155\Omega
$$

<br>

$$
P(W) = U*i = 3.1 * 0.02 = 0.062W
$$

<br>
Não existe resistor comercial com essa medida, porém usaremos o de 160, que é um valor próximo, para essa potência o valor minimo é um de 1/8W (0.125W)

<br>

### Para o PIC32 a tensão de trabalho é de 3.3V

<br>

$$
R(\Omega) = \frac{3.3 - 1.9}{0.02} = \frac{1.4}{0.02} = 70\Omega
$$

<br>

$$
P(W) = U*i = 1.4 * 0.02 = 0.028W
$$

<br>

Não existe resistor comercial com essa medida, porém usaremos o de 75, que é um valor próximo, para essa potência o valor minimo é um de 1/16W (0.0625W)

---

Para essa aplicação será necessário utilizar um pino que tenha capacidade produzir um sinal digital, para cada tomando o cuidado de não usar pinos reservados.

Para o Arduino será utilizado o pino D3.

e


# PAREI AQUI, DAQUI PRA BAIXO ESTÁ EM ELABORAÇÃO
Para o PIC será utilizado o pino RA10.

No Arduino:
```bash
void setup() {
    // define o pino D3 como saída
    pinMode(D3, OUTPUT);
}

void loop() {
    // liga o LED:
    digitalWrite(D3, HIGH); 

    // espera 1 segundo:
    delay(1000);   

    // desliga o LED:  
    digitalWrite(D3, LOW); 

    // espera 1 segundo: 
    delay(1000);            
}
```

No PIC32:
<br>
Aqui a situação é um pouquinho mais complexa e iremos detalhar abaixo:

Da forma que o Arduino trabalha seus cálculos são feitos por algoritmos internos de calibração de clock, o que infere que os sistemas não sejam tão precisos, no PIC conseguimos quantificar os ciclos de máquina manualmente, o que garante uma precisão milimétrica do sistema, para isso precisamos definir as constantes de nosso sistema, de acordo com o datasheet do fabricante o sistema é alimenato pelo cristal de clock Y300 MCHP CLOCK OSCILLATOR SINGLE 12MHz, partindo disso, para definir esse clock usaremos esse conjunto de instruções:

```
include <xc.h>
#include <sys/attribs.h>

// CONFIGURAÇÃO DO CLOCK PARA CRISTAL DE 12MHz
// Oscilador: System PLL
#pragma config FNOSC = SPLL       

// Cristal Externo de Alta Velocidade
#pragma config POSCMOD = HS      

// PLL usa o Oscilador Primário (Cristal) 
#pragma config FPLLICLK = POSC  

// Cria uma parametro para simplificar o processo de chamada de clock:
#define SYS_FREQ 120000000L
```
Caso deseje habilitar o cão de guarda do PIC definir como ON:
```
#pragma config FWDTEN = OFF       // Watchdog desligado
```

Nas definições inicias de pinos temos mais parametros a definir que no Arduino, o que nos fornece mais controle da operação:

```bash
// Configurar Pino RB14 para uso no LED:

void init_gpio(void)
{    
    // RB7 como saída
    TRISBbits.TRISB14 = 0;   

    // LED inicialmente desligado
    LATBbits.LATB14 = 0;     
}
```

Para simplificar a chamada de delay vamos criar uma função auxiliar:
```bash
void delay_ms(unsigned int ms)
{
    // Timer1 roda no PBCLK (ex.: 80 MHz)
    T1CON = 0;           // Para Timer1
    T1CONbits.TCKPS = 3; // Prescaler 1:256

    // PBCLK = 12 MHz → T1 tick = 12MHz/256 = 312.500 Hz
    // 1 ms = 312.5 contagens
    PR1 = 312;           // ~1 ms
    T1CONbits.ON = 1;

    for (unsigned int i = 0; i < ms; i++)
    {
        TMR1 = 0;
        while (!IFS0bits.T1IF); // Espera overflow
        IFS0bits.T1IF = 0;      // Limpa flag
    }
}
```



```bash
#include <xc.h>


int main(void)
{
    // define o pino RA0 como saída
    TRISAbits.TRISA10 = 0;

    while (1)
    {
        // liga o LED:
        LATAbits.LATA0 = 1;  
        for (volatile int i = 0; i < 500000; i++);
        
        // desliga o LED:
        LATAbits.LATE0 = 0;  
        for (volatile int i = 0; i < 500000; i++);
    }
}

