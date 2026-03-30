#### Para dúvidas, comentários ou sugestões fiquem à vontade, estarei à disposição através de minha página: 

[Matheus Grossi](https://www.linkedin.com/in/matheus-grossi/)

# Projeto de Controle de Estados com Botões - PIC32MK

## Descrição
Este projeto é um exemplo prático de inicialização, configuração de hardware e controle de estados em um microcontrolador da família PIC32MK. O firmware implementa uma lógica de operação com três estados principais: **desligado**, **ligado** e **emergência**, utilizando botões físicos e sinalização visual por LEDs.

O sistema foi desenvolvido com o compilador **XC32** e utiliza uma abordagem baseada em **máquina de estados**, permitindo controle consistente do comportamento do circuito através dos botões de **liga/desliga** e **emergência**.

**Autor:** Matheus Grossi  
**Data:** 13 de Fevereiro de 2026

## Hardware Alvo
* **Microcontrolador:** PIC32MK0128MCA048
* **Botão de Liga/Desliga:** RB1
* **Botão de Emergência:** RB2
* **LED Verde:** RB10
* **LED Vermelho:** RB13
* **LED Amarelo:** RB4
* **Frequência de Clock de Sistema (`SYSCLK`):** 12 MHz (12000000 Hz)

## Lógica de Operação do Sistema

O firmware trabalha com três estados distintos:

### 1. Estado Desligado
Neste estado, o sistema permanece inativo e a sinalização visual indica condição de repouso.

* **LED verde:** apagado
* **LED vermelho:** aceso
* **LED amarelo:** apagado

Ao pressionar o botão de **liga/desliga**, o sistema passa para o estado **ligado**.

### 2. Estado Ligado
Neste estado, o sistema entra em operação normal.

* **LED verde:** aceso
* **LED vermelho:** apagado
* **LED amarelo:** apagado

Ao pressionar novamente o botão de **liga/desliga**, o sistema retorna ao estado **desligado**.

### 3. Estado de Emergência
O estado de emergência só pode ser acionado se o sistema já estiver previamente no estado **ligado**.

Quando o botão de **emergência** é pressionado:

* a operação normal é interrompida;
* o **LED vermelho** permanece aceso;
* o **LED amarelo** passa a piscar;
* o sistema permanece travado em emergência até que o botão de **liga/desliga** seja pressionado novamente.

Ao pressionar o botão de **liga/desliga** durante a emergência, o sistema reinicia e retorna ao estado **ligado**.

## Comportamento dos Botões

O projeto considera botões com lógica de acionamento por **pull-down** em bancada, ou seja:

* **nível lógico 0**: botão solto
* **nível lógico 1**: botão pressionado

A leitura dos botões é feita por **detecção de borda de subida**, evitando múltiplos acionamentos indevidos enquanto o botão permanece pressionado. Além disso, foi incorporado um pequeno tratamento de **debounce por software** para maior estabilidade na leitura.

## Estrutura do Projeto

O projeto foi organizado de forma objetiva e funcional, mantendo os elementos centrais separados conforme sua responsabilidade:

* **`defs.h`**: Centraliza os *Configuration Bits* (`#pragma config`), a frequência de clock do sistema e o protótipo da função de atraso.
* **`delay.c`**: Implementa a função `delay_ms`, utilizada para temporizações por software com base no Core Timer.
* **`main.c`**: Contém a configuração dos pinos, a definição da máquina de estados, as rotinas de operação e a lógica principal do sistema.

## Detalhes de Implementação

### Função de Atraso Exata (`delay_ms`)
A função de atraso utiliza o **Core Timer** do núcleo MIPS (Coprocessador 0). O registrador `_CP0_GET_COUNT()` incrementa tipicamente a uma taxa de `SYSCLK / 2`. A partir disso, a função calcula a quantidade de *ticks* necessária para representar o tempo em milissegundos desejado, permitindo atrasos por software com boa precisão.

### Inicialização e Configuração
Durante a inicialização, são realizadas as seguintes etapas:

* desativação do **JTAG** em tempo de execução;
* configuração de **RB1** e **RB2** como entradas digitais;
* configuração de **RB4**, **RB10** e **RB13** como saídas digitais;
* inicialização do sistema no estado **desligado**.

### Máquina de Estados
A lógica principal foi implementada com três estados:

* `STATE_OFF`
* `STATE_ON`
* `STATE_EMERGENCY`

Essa abordagem permite tratar de forma clara e escalável o comportamento do sistema, facilitando futuras expansões, como inserção de novos modos de operação, temporizações adicionais ou tratamento por interrupções.

### Tratamento do Modo de Emergência
O modo de emergência foi implementado de forma a não depender apenas do nível lógico momentâneo do botão. Uma vez acionado, o estado é mantido internamente pelo firmware até que o botão de **liga/desliga** seja novamente pressionado, caracterizando um comportamento de travamento controlado do sistema.

## Tabela de Estados Observados em Bancada

<div align="center">

| **Estado do Sistema** | **LED Verde** | **LED Vermelho** | **LED Amarelo** |
| :--- | :---: | :---: | :---: |
| Desligado | Apagado | Aceso | Apagado |
| Ligado | Aceso | Apagado | Apagado |
| Emergência | Apagado | Aceso | Piscando |

</div>

## Como Utilizar e Compilar

1. Crie um novo projeto no **MPLAB X IDE**.
2. Adicione os arquivos `main.c` e `delay.c` à pasta *Source Files*.
3. Adicione o arquivo `defs.h` à pasta *Header Files*.
4. Certifique-se de que o compilador **Microchip XC32** está selecionado nas propriedades do projeto.
5. A macro `_CONFIG_BITS_SOURCE` deve permanecer definida no topo do `main.c` para que as configurações presentes em `defs.h` sejam aplicadas corretamente.
6. Compile o projeto e grave o firmware no microcontrolador.
7. Conecte os botões e LEDs conforme os pinos definidos e valide os estados em bancada.
