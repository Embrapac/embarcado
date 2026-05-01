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

Da forma que o Arduino trabalha seus cálculos são feitos por algoritmos internos de calibração de clock, o que infere que os sistemas não sejam tão precisos, no PIC conseguimos quantificar os ciclos de máquina manualmente, o que garante uma precisão milimétrica do sistema, para isso precisamos definir as constantes de nosso sistema, de acordo com o datasheet do fabricante o sistema é alimenato pelo cristal de clock Y300 MCHP CLOCK OSCILLATOR SINGLE 12MHz, partindo disso, para definir esse clock usaremos esse conjunto de instruções em nosso header defs.h:

```bash
#ifndef DEFS_H
#define DEFS_H

// Colocar config bits APENAS quando _CONFIG_BITS_SOURCE estiver definido
#ifdef _CONFIG_BITS_SOURCE

// PIC32MK0128MCA048 Configuration Bit Settings
// 'C' source line config statements

// DEVCFG3
#pragma config USERID   = 0xFFFF
#pragma config PWMLOCK  = OFF
#pragma config PGL1WAY  = ON
#pragma config PMDL1WAY = ON
#pragma config IOL1WAY  = ON

// DEVCFG2
#pragma config FPLLIDIV = DIV_8
#pragma config FPLLRNG  = RANGE_54_64_MHZ
#pragma config FPLLICLK = PLL_FRC
#pragma config FPLLMULT = MUL_128
#pragma config FPLLODIV = DIV_32
#pragma config BORSEL   = HIGH

// DEVCFG1
#pragma config FNOSC      = FRC
#pragma config DMTINTV    = WIN_127_128
#pragma config FSOSCEN    = ON
#pragma config IESO       = ON
#pragma config POSCMOD    = OFF
#pragma config OSCIOFNC   = OFF
#pragma config FCKSM      = CSECME
#pragma config WDTPS      = PS1048576
#pragma config WDTSPGM    = STOP
#pragma config WINDIS     = NORMAL
#pragma config FWDTEN     = OFF
#pragma config FWDTWINSZ  = WINSZ_25
#pragma config DMTCNT     = DMT31
#pragma config FDMTEN     = OFF

// DEVCFG0
#pragma config DEBUG      = OFF
#pragma config JTAGEN     = OFF
#pragma config ICESEL     = ICS_PGx1
#pragma config TRCEN      = ON
#pragma config BOOTISA    = MIPS32
#pragma config FECCCON    = ECC_DECC_DISABLE_ECCON_WRITABLE
#pragma config FSLEEP     = OFF
#pragma config DBGPER     = PG_ALL
#pragma config SMCLR      = MCLR_NORM
#pragma config SOSCGAIN   = G3
#pragma config SOSCBOOST  = ON
#pragma config POSCGAIN   = G3
#pragma config POSCBOOST  = ON
#pragma config POSCFGAIN  = G3
#pragma config POSCAGCDLY = AGCRNG_x_25ms
#pragma config POSCAGCRNG = ONE_X
#pragma config POSCAGC    = Automatic
#pragma config EJTAGBEN   = NORMAL

// DEVCP
#pragma config CP = OFF

// SEQ
#pragma config TSEQ = 0xFFFF
#pragma config CSEQ = 0xFFFF

#endif // _CONFIG_BITS_SOURCE

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#endif // DEFS_H
```

Com essa definições, criamos então nosso arquivo main.c:

```bash
/* * File:   main.c
 * Author: Matheus Grossi
 * Created on 13 de Fevereiro de 2026, 14:11
 */

#define _CONFIG_BITS_SOURCE 

#include "defs.h"

#include <xc.h>
#include <stdint.h>
#include <sys/attribs.h>

#define SYSCLK_HZ 12000000UL   // ajuste para o clock REAL do seu projeto

static void delay_ms(uint32_t ms)
{
    // CP0 Count geralmente incrementa a SYSCLK/2
    const uint32_t ticks_per_ms = (SYSCLK_HZ / 2U) / 1000U;
    uint32_t start = _CP0_GET_COUNT();
    uint32_t wait  = ticks_per_ms * ms;

    while ((uint32_t)(_CP0_GET_COUNT() - start) < wait) { }
}

int main(void)
{
    // garante JTAG desligado em runtime também
    CFGCONbits.JTAGEN = 0;

    // RB10 como saída usando struct
    TRISBbits.TRISB10 = 0;

    // inicia em LOW
    LATBbits.LATB10 = 0;

    while (1)
    {
        //Força nível lógico baixo:
        LATBbits.LATB10 = 0;
        
        //Tempo de espera:
        delay_ms(500);
        
        //Força nível lógico alto:
        LATBbits.LATB10 = 1;
        
        //Tempo de espera:
        delay_ms(500);
    }
}
```

Este pequeno exemplifica as grandes diferenças estruturais entre um projeto baseado em Arduino e um em PIC32, dadas as considerações iniciais, vamos falar agora sobre o projeto de fato.


# Projeto de Medição, Classificação e Controle com PIC32MK + UART

Abaixo iremos descrever o sistema de medição/classificação de caixas com controle local e remoto via UART feito para a aplicação da Embrapac.

O projeto integra:

- leitura analógica de sensor de distância/tensão;
- classificação de caixa por faixa de tensão;
- sinalização local por LEDs;
- controle físico por botões;
- comando remoto via UART;
- transmissão do estado do sistema conforme protocolo binário;
- acionamento digital da esteira;
- LEDs de debug para atividade UART.

---

## 1. Plataforma alvo

| Item | Valor |
|---|---|
| MCU | PIC32MK0128MCA048 |
| Família | PIC32MK |
| Toolchain esperado | MPLAB X + XC32 |
| Clock de referência | 12 MHz |
| UART principal | UART1 |
| ADC usado | AN0 / RA0 |

---

## 2. Visão geral do sistema

O firmware atua como controlador local entre sensores, botões, painel de LEDs, esteira e sistema externo de supervisão.

Fluxo geral:

1. O usuário liga/desliga o sistema por botão físico ou comando remoto UART.
2. Quando ligado, o sistema habilita a medição analógica.
3. A leitura do ADC é convertida para tensão e distância estimada.
4. A classificação local aciona o LED correspondente à caixa detectada.
5. A UART transmite um byte de protocolo sempre que o estado final do protocolo muda.
6. O pino RC6 acompanha o estado ligado e atua como sinal de acionamento da esteira.

---

## 3. Organização dos arquivos

| Arquivo | Função |
|---|---|
| `main.c` | Loop principal, polling UART, processamento de amostras e envio do protocolo |
| `defs.h` | Configurações globais, clock, constantes de ADC, estados e config bits |
| `pins.h` / `pins.c` | Mapeamento e inicialização dos GPIOs |
| `init.h` / `init.c` | Inicialização de clock, Timer2 e delay baseado no core timer |
| `adc_1.h` / `adc_1.c` | Inicialização e processamento do ADC AN0 |
| `out_control.h` / `out_control.c` | Classificação por tensão e acionamento dos LEDs de caixa |
| `on_off_button.h` / `on_off_button.c` | Máquina de estados do botão liga/desliga e comando remoto |
| `emergency_button.h` / `emergency_button.c` | Tratamento do botão de emergência |
| `uart.h` / `uart.c` | Inicialização UART1, TX, RX, comando remoto e LEDs de debug |
| `uart_status.h` / `uart_status.c` | Montagem do byte de protocolo transmitido pela UART |

---

## 4. Tabela de I/Os em uso

### 4.1 Entradas e sensor

| Pino MCU | Direção | Função |
|---|---|---|
| RA0 / AN0 | Entrada analógica | Sensor de medição de distância/tensão |
| RB1 | Entrada digital | Botão físico liga/desliga |
| RB2 | Entrada digital | Botão físico de emergência |
| RC1 | Entrada UART | UART1 RX |

### 4.2 Saídas UART e debug

| Pino MCU | Direção | Função |
|---|---|---|
| RC0 | Saída UART | UART1 TX |
| RC2 | Saída digital | LED debug de transmissão UART |
| RA8 | Saída digital | LED debug de recepção UART |

### 4.3 LEDs de estado do sistema

| Pino MCU | Direção | Função |
|---|---|---|
| RC11 | Saída digital | LED de sistema ligado |
| RA11 | Saída digital | LED de sistema desligado |
| RA12 | Saída digital | LED de emergência |

### 4.4 Medição e classificação de caixa

| Pino MCU | Direção | Função |
|---|---|---|
| RA1 | Saída digital | LED caixa P / LED branco |
| RB3 | Saída digital | LED caixa M / LED azul |
| RB9 | Saída digital | LED caixa G / LED vermelho |

### 4.5 Acionamento

| Pino MCU | Direção | Função |
|---|---|---|
| RC6 | Saída digital | Sinal de acionamento da esteira |

---

## 5. Estados do sistema

O projeto usa três estados principais:

| Estado | Descrição |
|---|---|
| `STATE_OFF` | Sistema desligado, medição desabilitada |
| `STATE_ON` | Sistema ligado, medição habilitada |
| `STATE_EMERGENCY` | Emergência ativa, medição desabilitada |

### 5.1 Comportamento por estado

| Estado | LED ON | LED OFF | LED EMERGENCY | Esteira RC6 | Medição |
|---|---:|---:|---:|---:|---:|
| `STATE_OFF` | 0 | 1 | 0 | 0 | Desabilitada |
| `STATE_ON` | 1 | 0 | 0 | 1 | Habilitada |
| `STATE_EMERGENCY` | 0 | 1 | Piscante | 0 | Desabilitada |

---

## 6. Controle local e remoto

### 6.1 Botão físico liga/desliga

O botão em RB1 alterna o estado operacional:

- `STATE_OFF` → `STATE_ON`
- `STATE_ON` → `STATE_OFF`
- `STATE_EMERGENCY` → `STATE_ON`

A lógica inclui debounce por software.

### 6.2 Botão físico de emergência

O botão em RB2 só ativa emergência quando o sistema está em `STATE_ON`.

Ao entrar em emergência:

- a medição é desabilitada;
- a esteira é desligada;
- o LED de emergência pisca;
- o byte de protocolo passa a indicar emergência.

### 6.3 Comando remoto UART

O comando remoto é recebido pela UART RX.

| Comando recebido | Valor binário | Valor hexadecimal | Função |
|---|---:|---:|---|
| Toggle remoto | `00100000` | `0x20` | Alterna o estado de operação |

Esse comando simula o botão físico liga/desliga.

---

## 7. UART

### 7.1 Configuração

| Parâmetro | Valor |
|---|---:|
| UART | UART1 |
| Baud rate | 9600 bps |
| TX | RC0 |
| RX | RC1 |
| Formato | 8 bits, sem paridade, 1 stop bit |
| BRGH | 1 |

### 7.2 LEDs de debug UART

| Evento | Pino | Comportamento |
|---|---|---|
| TX | RC2 | Acende por 1 s após transmissão |
| RX | RA8 | Acende por 1 s após recepção |

O tempo dos LEDs é mantido pela rotina `uart1_led_service_1ms()`, chamada a partir de `delay_ms()`.

---

## 8. Protocolo UART de transmissão

O sistema transmite um único byte binário de estado.

A ordem lógica da tabela do protocolo é empacotada em ordem visual:

| Campo lógico | Bit físico transmitido | Função |
|---|---:|---|
| B0 | bit 7 | Estado ligado/desligado |
| B1 | bit 6 | Emergência |
| B2 | bit 5 | Comando remoto; fixo em 0 no TX do PIC |
| B3 | bit 4 | Reservado, fixo em 0 |
| B4 | bit 3 | Reservado, fixo em 0 |
| B5 | bit 2 | Reservado, fixo em 0 |
| B6 | bit 1 | Campo de caixa |
| B7 | bit 0 | Campo de caixa |

### 8.1 Campo B0

| Estado | B0 |
|---|---:|
| Sistema ligado | 1 |
| Sistema desligado ou emergência | 0 |

### 8.2 Campo B1

| Estado | B1 |
|---|---:|
| Emergência ativa | 1 |
| Demais estados | 0 |

### 8.3 Campo B2

No TX do PIC, B2 permanece fixo em 0.

O comando remoto é recebido pelo RX como `00100000` (`0x20`) e não é retransmitido como estado ativo permanente.

### 8.4 Campo de caixa B6:B7

Mapeamento final confirmado em bancada:

| Caixa | Campo final B6:B7 | Observação |
|---|---:|---|
| Nenhuma | `00` | Sem classificação ativa |
| Caixa P | `01` | LED branco |
| Caixa M | `10` | LED azul |
| Caixa G | `11` | LED vermelho |

### 8.5 Exemplos de bytes transmitidos

| Situação | Byte binário | Hex |
|---|---:|---:|
| Desligado, sem caixa | `00000000` | `0x00` |
| Ligado, sem caixa | `10000000` | `0x80` |
| Emergência, sem caixa | `01000000` | `0x40` |
| Ligado, caixa P | `10000001` | `0x81` |
| Ligado, caixa M | `10000010` | `0x82` |
| Ligado, caixa G | `10000011` | `0x83` |

### 8.6 Política de envio

O firmware não transmite continuamente.

A transmissão ocorre apenas quando o byte final do protocolo muda:

```c
current_status = uart_build_status_byte();

if (current_status != last_status)
{
    uart1_send_byte(current_status);
    last_status = current_status;
}
```

Isso evita tráfego redundante e impede que o LED TX permaneça continuamente aceso por retransmissão desnecessária.

---

## 9. Medição analógica

### 9.1 ADC

| Item | Valor |
|---|---:|
| Canal usado | AN0 |
| Pino | RA0 |
| Referência | AVDD / AVSS |
| Resolução configurada | 12 bits |
| Valor máximo | 4095 |
| Tensão de referência lógica | 3.3 V |

A conversão de ADC para tensão é feita por:

```c
v = (adc_value * ADC_REFERENCE_VOLTAGE) / ADC_MAX_COUNTS;
```

### 9.2 Cálculo de distância

O projeto mantém a curva polinomial do sensor:

```c
distance_cm =
    (DIST_POLY_A * v * v * v) +
    (DIST_POLY_B * v * v) +
    (DIST_POLY_C * v) +
    DIST_POLY_D;
```

Constantes atuais:

| Constante | Valor |
|---|---:|
| `DIST_POLY_A` | `1.720473f` |
| `DIST_POLY_B` | `4.15228f` |
| `DIST_POLY_C` | `-59.8489f` |
| `DIST_POLY_D` | `117.313f` |

### 9.3 Faixa válida de medição

| Parâmetro | Valor |
|---|---:|
| `DIST_MIN_VALID_CM` | `10.0f` |
| `DIST_MAX_VALID_CM` | `80.0f` |

Quando a distância fica fora dessa faixa:

- os LEDs de caixa são apagados;
- `g_voltage_zone` retorna para 0;
- o código de caixa UART retorna para `00`.

---

## 10. Classificação de caixa

A classificação atualmente é feita por tensão em `Output_UpdateFromVoltage()`.

### 10.1 Faixas configuradas no firmware atual

| Caixa | Faixa de tensão | Código interno |
|---|---|---:|
| P | `V < 1.00 V` | `UART_BOX_CODE_P` |
| M | `1.00 V <= V < 1.05 V` | `UART_BOX_CODE_M` |
| G | `V >= 1.05 V` | `UART_BOX_CODE_G` |

### 10.2 LEDs de classificação

| Caixa | LED | Pino | Código UART |
|---|---|---|---:|
| P | Branco | RA1 | `01` |
| M | Azul | RB3 | `10` |
| G | Vermelho | RB9 | `11` |

### 10.3 Função central

A função que decide a classificação é:

```c
void Output_UpdateFromVoltage(float voltage);
```

Essa função:

1. escolhe a zona da caixa;
2. aciona o LED correspondente;
3. atualiza o código de caixa usado pelo protocolo UART.

---

## 11. Timer e periodicidade

O Timer2 é usado como base de amostragem do sistema de medição.

| Parâmetro | Valor |
|---|---:|
| Timer | Timer2 |
| Prescaler | 1:256 |
| Período de medição | 1000 ms |
| Interrupção | Timer2 ISR |

O Timer2 só fica ligado quando a medição está habilitada.

---

## 12. Interrupções

### 12.1 Timer2

Responsável por disparar a conversão do ADC ou sinalizar o ciclo de amostragem.

### 12.2 ADC

O ADC usa interrupção de fim de sequência para atualizar a amostra e marcar nova leitura disponível.

O loop principal processa a amostra quando:

```c
analog_measurement_is_enabled() != 0u && g_adc0_new_sample != 0u
```

---

## 13. Inicialização do sistema

A função `init()` realiza:

1. desabilitação do JTAG;
2. configuração dos pinos analógicos/digitais;
3. configuração dos botões;
4. configuração dos LEDs de estado, caixa, UART e esteira;
5. inicialização do clock lógico;
6. inicialização do Timer2;
7. inicialização do ADC;
8. inicialização da UART1;
9. inicialização do protocolo UART;
10. inicialização da máquina de estados;
11. habilitação de interrupções multi-vetor.

Após a inicialização, o sistema começa em `STATE_OFF`.

---

## 14. Fluxo principal do firmware

O loop principal executa continuamente:

1. leitura/polling da UART;
2. aplicação de comando remoto, se recebido;
3. atualização da máquina de estados;
4. processamento de nova amostra ADC, se disponível;
5. montagem do byte de protocolo;
6. envio UART apenas se o byte mudou;
7. delay de 1 ms com serviço dos LEDs UART.

Resumo simplificado:

```c
while (1)
{
    rx_events = uart1_rx_poll();

    if ((rx_events & UART_RX_EVENT_REMOTE_TOGGLE) != 0u)
    {
        on_off_button_request_remote_toggle();
    }

    operation();

    if ((analog_measurement_is_enabled() != 0u) &&
        (g_adc0_new_sample != 0u))
    {
        g_adc0_new_sample = 0u;
        analog_process_sample();
    }

    current_status = uart_build_status_byte();

    if (current_status != last_status)
    {
        uart1_send_byte(current_status);
        last_status = current_status;
    }

    delay_ms(1u);
}
```

---

## 15. Observações de integração

- A UART permanece ativa mesmo com o sistema desligado.
- O comando remoto por RX funciona independentemente do estado atual.
- A medição analógica só opera quando o sistema está em `STATE_ON`.
- A esteira em RC6 só é acionada em `STATE_ON`.
- `RC6` não é LED; é saída de acionamento.
- `RC2` e `RA8` são apenas LEDs de debug de atividade UART.
- O protocolo TX é binário; em terminal ASCII ele pode aparecer como caracteres não imprimíveis.
- Para validar corretamente o protocolo, use visualização hexadecimal ou lógica/terminal que mostre bytes em HEX.

---

## 17. Checklist de validação em bancada

### 17.1 Estado e controle

- [ ] Ao ligar por botão físico, `RC11 = 1`, `RA11 = 0`, `RC6 = 1`.
- [ ] Ao desligar por botão físico, `RC11 = 0`, `RA11 = 1`, `RC6 = 0`.
- [ ] Ao enviar `0x20` via UART RX, o sistema alterna ON/OFF.
- [ ] Em emergência, `RA12` pisca e `RC6 = 0`.

### 17.2 UART

- [ ] RX acende `RA8` por 1 s ao receber dados.
- [ ] TX acende `RC2` por 1 s ao enviar dados.
- [ ] O byte só é transmitido quando há mudança real de estado/protocolo.
- [ ] O comando remoto `0x20` não causa envio contínuo.

### 17.3 Classificação

- [ ] Caixa P acende LED branco em RA1.
- [ ] Caixa M acende LED azul em RB3.
- [ ] Caixa G acende LED vermelho em RB9.
- [ ] Caixa P transmite `10000001` quando o sistema está ligado.
- [ ] Caixa M transmite `10000010` quando o sistema está ligado.
- [ ] Caixa G transmite `10000011` quando o sistema está ligado.

---

## 19. Referência rápida

### 19.1 Comando RX

| Ação | Binário | Hex |
|---|---:|---:|
| Alternar ON/OFF remoto | `00100000` | `0x20` |

### 19.2 Bytes TX principais

| Situação | Binário | Hex |
|---|---:|---:|
| Ligado sem caixa | `10000000` | `0x80` |
| Ligado com caixa P | `10000001` | `0x81` |
| Ligado com caixa M | `10000010` | `0x82` |
| Ligado com caixa G | `10000011` | `0x83` |
| Emergência | `01000000` | `0x40` |
| Desligado | `00000000` | `0x00` |

### 19.3 Pinos principais

| Função | Pino |
|---|---|
| Sensor analógico | RA0 / AN0 |
| UART TX | RC0 |
| UART RX | RC1 |
| Botão ON/OFF | RB1 |
| Botão emergência | RB2 |
| Esteira | RC6 |
| LED debug TX | RC2 |
| LED debug RX | RA8 |
| LED caixa P | RA1 |
| LED caixa M | RB3 |
| LED caixa G | RB9 |

---

## 20. Notas finais

Este firmware foi estruturado para manter o comportamento funcional do sistema de medição original e adicionar a comunicação UART sem remover funcionalidades existentes.

A regra central do projeto é:

> A UART transmite somente quando o estado consolidado do protocolo muda.

Isso reduz tráfego desnecessário e torna os LEDs de debug coerentes com eventos reais de comunicação.
