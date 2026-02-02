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

## 2. Comparativo simples de um circuito blink no Arduino vs PIC:
<div align="justify">
O que é um circuito Blink? Um circuito de teste simples, onde é alocado um led de uma cor qualquer, habitualmente o vermelho, de modo que o mesmo, alterne entre os estados ligado/desligado, a cada ciclo de tempo pré-determinado, em ambos os casos, usaremos um ciclo de 1s para cada comutação.

Além disso, é necessário utilizar um resistor para controle do potencial que alimenta o LED, este dispositivo pode ser dimensionado através desta fórmula:

Para essa aplicação será necessário utilizar um pino que tenha capacidade produzir um sinal digital, tomando o cuidado de não usar pinos reservados, no Arduino será o usado o pino D3, e no PIC será usado o RA10.

---

#### 2.1 Cálculo do resistor para os LED:

<div align="justify">
Serão utilizados dois leds neste contexto, o amarelo que simula uma iluminação de área externa controlado pelo relé, enquanto o led vermelho representa a luz de alerta, acionada diretamente pelo gpio, as fórmulas abaixo representam o modo de cálculo para dimensionamento:

<br>
<br>
2.1.1. Resistência mínima segura:

$$
R(\Omega) = \frac{V_{pp} - V_{led}}{I_{led}}
$$ 

<br>


2.1.2. Dissipação no resistor:

$$
P(W) = U*i
$$

2.1.3. Potência mínima do resistor:
<br>
*Fs é um fator de segurança, idealmente adoto 50%*
<br>

$$
Pmin = P(W) * (1+Fs)
$$
<br>
<br>

---

<div align="center">

| **Tabela de caracteristicas técnicas dos LEDs** |
| :---: |
![tabela_led](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/tabela_led.png)
| **Tabela de resistores comerciais** |
![tabela_resistores](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/tabela_resistores.png)
| **Diagrama de potências de resistores comerciais** |
![tabela_resistores](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/pot_resistores.png)

<br>

<div align="justify">

### 2.2 Dimensionamento dos resistores para os LEDs:

### 2.2.1 Para o Arduino:

Neste a tensão de trabalho do GPIO é de 5V.

Este em carga carga plena utiliza 1.7V, e sua corrente é de 10mA (0.01A).



<br>

$$
R(\Omega) = \frac{5 - 1.7}{0.01} = \frac{3.3}{0.01} = 330\Omega
$$

<br>

$$
P(W) = U*i = 3.3 * 0.01 = 0.033W
$$

<br>

$$
Pmin = U*i = 0.033W * (1+0.50)=0.0495W
$$
<br>

<br>
Usaremos um resistor comercial de 330R, e para essa potência o valor minimo arredonando para cima será um de 1/16W (0.0625W).

<br>

### 2.2.2 Para o PIC32:

Neste a tensão de trabalho do GPIO é de 3.3V.

Este em carga carga plena utiliza 1.7V, e sua corrente é de 10mA (0.01A).



<br>

$$
R(\Omega) = \frac{3.3 - 1.7}{0.01} = \frac{1.6}{0.01} = 160\Omega
$$

<br>

$$
P(W) = U*i = 1.6 * 0.01 = 0.016W
$$

<br>

$$
Pmin = U*i = 0.016W * (1+0.50)=0.024W
$$
<br>

<br>
Usaremos um resistor comercial de 160R, e para essa potência o valor minimo arredonando para cima será um de 1/16W (0.0625W).

---
## 2.3 Arquitetura dos firmwares
### 2.3.1 Para o Arduino:

No ambiente do arduino os clocks já são previamente configuardos a nível de firmware da bios do SoC, fazendo com que o usuário não necesiste se preocupar com essas questões, restando assim duas funções básicas a programar, o "setup" que é chamada apenas uma vez e especificamente no ato da inicialização, como pode ser visto abaixo:

```bash
void setup() {
    // define o pino D3 como saída
    pinMode(D3, OUTPUT);
}
```

Ea 2º função mínima obrigatória é a de "loop", o qual após setup assume o comando e é repetida infinitamente, como pode ser vista abaixo:


```bash
void loop() {
    // liga o LED:
    digitalWrite(D3, 1); 

    // espera 1 segundo:
    delay(1000);   

    // desliga o LED:  
    digitalWrite(D3, 0); 

    // espera 1 segundo: 
    delay(1000);            
}
```

### 2.3.2 No PIC32:
<br>
Aqui a situação é um pouquinho mais complexa e iremos detalhar abaixo:

Da forma que o Arduino trabalha seus cálculos são feitos por algoritmos internos de calibração de clock, o que infere que os sistemas não sejam tão precisos, no PIC conseguimos quantificar os ciclos de máquina manualmente, o que garante uma precisão milimétrica do sistema, para isso precisamos definir as constantes de nosso sistema, de acordo com o datasheet do fabricante o sistema é alimenato pelo cristal de clock Y300 MCHP CLOCK OSCILLATOR SINGLE 12MHz, partindo disso, para definir esse clock usaremos esse conjunto de instruções:

```bash
//Includes:
#include <xc.h>

//Definições Pragma (Instruções passadas ao compilador):
//--------------------------------------------------------
// Liga/desliga o sistema de cão de guarda de temporização do MCU
#pragma config FWDTEN   = OFF    // Turn off watchdog timer

//Habilita o uso 
// Desabilita o debugger em background
#pragma config DEBUG    = OFF         

// Desabilita a interface JTAG
#pragma config JTAGEN   = OFF         

// Seleciona canal de comunicação ICD/ICE PGx1
#pragma config ICESEL   = ICS_PGx1    

// Desabilita o módulo de rastreamento Trace
#pragma config TRCEN    = OFF         

// Boot em ISA MIPS32
#pragma config BOOTISA  = MIPS32      

// ECC de flash desabilitado
#pragma config FECCCON  = OFF_UNLOCKED 

// Flash não entra em modo sleep
#pragma config FSLEEP   = OFF         

// Permite acesso de debug a todos os periféricos
#pragma config DBGPER   = PG_ALL      


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

