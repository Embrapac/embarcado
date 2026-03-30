#### Para dúvidas, comentários ou sugestões, fique à vontade para entrar em contato através da minha página:

[Matheus Grossi](https://www.linkedin.com/in/matheus-grossi/)

# Controle básico de saídas com botões - PIC32MK

## Descrição
Este projeto implementa um firmware simples para o **PIC32MK0128MCA048**, desenvolvido com o compilador **XC32**, para controle direto de dois LEDs a partir de dois botões físicos.

O projeto está organizado em três arquivos principais:

- **`defs.h`**: define a frequência de clock do sistema, declara `delay_ms()` e concentra os *Configuration Bits*.
- **`delay.c`**: implementa a rotina de atraso em milissegundos com base no **Core Timer**.
- **`main.c`**: realiza a configuração dos pinos e contém a lógica principal de operação.

**Autor:** Matheus Grossi  
**Base do código:** 13 de Fevereiro de 2026

## Hardware alvo
- **Microcontrolador:** PIC32MK0128MCA048
- **Botão em RB1**
- **Botão em RB2**
- **LED verde em RB10**
- **LED vermelho em RB13**
- **Clock de sistema (`SYSCLK_HZ`):** 12 MHz

## Mapeamento atual de pinos

| Sinal | Pino | Direção | Observação |
| :--- | :---: | :---: | :--- |
| Botão 1 | RB1 | Entrada | Leitura por `PORTBbits.RB1` |
| Botão 2 | RB2 | Entrada | Leitura por `PORTBbits.RB2` |
| LED verde | RB10 | Saída | Controle por `LATBbits.LATB10` |
| LED vermelho | RB13 | Saída | Controle por `LATBbits.LATB13` |

## Comportamento atual do firmware
O firmware inicializa o hardware, desabilita o **JTAG** em tempo de execução e configura:

- **RB1** e **RB2** como entradas digitais;
- **RB10** e **RB13** como saídas digitais;
- **RB1** e **RB2** com modo analógico desabilitado;
- estado inicial com **LED verde apagado** e **LED vermelho aceso**.

Após a inicialização, o programa executa continuamente a função `operation()` dentro do laço principal.

### Ação de cada botão
A lógica implementada hoje é a seguinte:

- se **RB2** estiver em nível lógico alto, a função `mode_off()` é executada;
- caso contrário, se **RB1** estiver em nível lógico alto, a função `mode_on()` é executada.

### Efeito visual das rotinas

| Rotina | Condição de entrada | LED verde (RB10) | LED vermelho (RB13) |
| :--- | :--- | :---: | :---: |
| `mode_off()` | `RB2 = 1` | Apagado | Aceso |
| `mode_on()` | `RB1 = 1` | Aceso | Apagado |
| Inicialização | após `init()` | Apagado | Aceso |

### Circuito em operação:

| **Ação** |
| :--- |
| Desligado |
|![Desligado](https://github.com/Embrapac/embarcado/blob/button_device/dev_wip/PIC32MK-on_off_button.X/images/mode_off.png)|
| Ligado |
|![Ligado](https://github.com/Embrapac/embarcado/blob/button_device/dev_wip/PIC32MK-on_off_button.X/images/mode_on.png)|

## Fluxo de operação
1. O microcontrolador inicia com os pinos configurados.
2. O LED vermelho permanece ligado por padrão.
3. Ao pressionar o botão ligado a **RB2**, o sistema coloca a saída no padrão de `mode_off()`.
4. Ao pressionar o botão ligado a **RB1**, o sistema coloca a saída no padrão de `mode_on()`.
5. Quando nenhum botão está pressionado, o firmware não altera as saídas, mantendo o último estado aplicado.

## Estrutura do software

### `defs.h`
Este arquivo centraliza definições globais do projeto:

- `#define SYSCLK_HZ 12000000UL`
- declaração da função `delay_ms(uint32_t ms);`
- *Configuration Bits* protegidos pela macro `_CONFIG_BITS_SOURCE`

Isso permite manter a configuração do dispositivo em um único local e evitar repetição entre arquivos do projeto.

### `delay.c`
A função `delay_ms()` utiliza o registrador `_CP0_GET_COUNT()` do núcleo MIPS. Como o contador incrementa tipicamente em **`SYSCLK / 2`**, o código converte o tempo solicitado em *ticks* e aguarda até o tempo expirar.

Essa abordagem é suficiente para atrasos curtos por software em aplicações simples.

### `main.c`
O arquivo principal contém:

- os mapeamentos simbólicos dos botões e LEDs por macros;
- a rotina `init()` para configuração de hardware;
- as rotinas `mode_off()` e `mode_on()`;
- a rotina `operation()`, responsável pela leitura dos botões;
- o laço principal infinito em `main()`.

## Compilação e uso
1. Crie um projeto para o **PIC32MK0128MCA048** no **MPLAB X IDE**.
2. Adicione `main.c` e `delay.c` em **Source Files**.
3. Adicione `defs.h` em **Header Files**.
4. Certifique-se de selecionar o compilador **XC32**.
5. Mantenha `#define _CONFIG_BITS_SOURCE` no topo de `main.c`, para que os *Configuration Bits* definidos em `defs.h` sejam compilados neste arquivo.
6. Compile o projeto e grave o firmware no microcontrolador.
7. Conecte os botões e LEDs conforme o mapeamento documentado acima.
