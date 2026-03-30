#### Para dúvidas, comentários ou sugestões fiquem à vontade, estarei à disposição através de minha página: 

[Matheus Grossi](https://www.linkedin.com/in/matheus-grossi/)

# Projeto de Blink Simples - PIC32MK

## Descrição
Este projeto é um exemplo prático de inicialização, configuração de hardware e alternância de estado de um pino (Blink) em um microcontrolador da família PIC32MK. O código configura o pino **RB10** como saída e intercala os níveis lógicos alto e baixo a cada 500 milissegundos, utilizando o compilador XC32.

**Autor:** Matheus Grossi  
**Data:** 13 de Fevereiro de 2026

## Hardware Alvo
* **Microcontrolador:** PIC32MK0128MCA048
* **Pino de Saída (LED/Sinal):** RB10 
* **Frequência de Clock de Sistema (`SYSCLK`):** 12 MHz (12000000 Hz)

## Circuito rodando em bancada:

<div align="center">
 
| **Ação** |
| :--- |
| Estado desligado |
|![Estado desligado](https://github.com/MattGrossi12/hello_world_pic32/blob/main/blink_state_0.jpeg)|
| Estado ligado |
|![Estado ligado](https://github.com/MattGrossi12/hello_world_pic32/blob/main/blink_state_1.jpeg)|

<div align="justify">
 
## Estrutura do Projeto

* `defs.h`: Arquivo de cabeçalho que armazena os *Configuration Bits* (`#pragma config`) do PIC32.
* `main.c`: Arquivo principal contendo a lógica do sistema. 
  * Ativa as configurações de hardware declarando a macro `_CONFIG_BITS_SOURCE` antes de incluir o arquivo `defs.h`.
  * Desabilita a interface JTAG em tempo de execução (`CFGCONbits.JTAGEN = 0;`) para evitar que ela interfira nas portas de I/O.
  * Executa o loop infinito com a lógica de piscar o LED.

## Detalhes de Implementação

### Função de Atraso Exata (`delay_ms`)
A função de atraso tira proveito do *Core Timer* embutido no núcleo MIPS (Coprocessador 0). Como o registrador de contagem do CP0 (`_CP0_GET_COUNT()`) é incrementado a uma taxa de `SYSCLK / 2`, a função calcula exatamente a quantidade de *ticks* necessários para atingir os milissegundos desejados. Isso cria um *delay* bloqueante preciso sem a necessidade de configurar *timers* periféricos adicionais.

### Manipulação de Registradores via Campos de Bits (Bitfields)
O código realiza a manipulação de hardware utilizando as estruturas de campos de bits predefinidas na biblioteca `<xc.h>`:
* `TRISBbits.TRISB10 = 0;`: Configura o pino RB10 como saída digital.
* `LATBbits.LATB10`: Acessa o registrador *Latch* para definir o estado de saída da porta.

### Lógica do Loop Principal
O loop `while(1)` dita os estados lógicos do pino de forma explícita e sequencial, garantindo o efeito visual de *Blink*:
1. Força o nível lógico baixo (`LATBbits.LATB10 = 0;`).
2. Aguarda 500 ms através da função `delay_ms`.
3. Força o nível lógico alto (`LATBbits.LATB10 = 1;`).
4. Aguarda mais 500 ms.

## Como Utilizar e Compilar

1. Crie um novo projeto no MPLAB X IDE.
2. Adicione os arquivos `main.c` e `defs.h` aos *Source Files* e *Header Files* do seu projeto, respectivamente.
3. Certifique-se de que o compilador **Microchip XC32** está selecionado nas propriedades.
4. Caso a sua placa utilize uma frequência diferente, atualize o valor da macro `SYSCLK_HZ` no arquivo `main.c` para garantir que o temporizador funcione corretamente.
5. Compile e grave o *firmware* no microcontrolador.
