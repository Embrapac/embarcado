#### Para dúvidas, comentários ou sugestões, fiquem à vontade. Estou à disposição por meio da minha página:

[Matheus Grossi](https://www.linkedin.com/in/matheus-grossi/)

# Projeto de Aquisição ADC com Sensor Sharp GP2Y0A02YK0F - PIC32MK

## Descrição

Este projeto implementa uma aplicação embarcada para o **PIC32MK0128MCA048** que realiza a leitura do sensor analógico de distância **Sharp GP2Y0A02YK0F** por meio do canal **AN0** e utiliza o valor convertido para:

![GP2Y0A02YK0F](https://raw.githubusercontent.com/MattGrossi12/PIC32MK-analog_read_example/main/images/sensor_model.png)

- calcular a tensão equivalente na entrada do ADC;
- estimar a distância em centímetros;
- classificar a leitura em faixas de tensão;
- acionar três LEDs de indicação conforme a região de operação configurada no firmware.

A aplicação foi organizada de forma modular, separando a configuração de periféricos, o tratamento de interrupções, a aquisição ADC, a lógica de saídas e a configuração de pinos.

---

## Sensor analógico utilizado

O dispositivo analógico em uso é o **Sharp GP2Y0A02YK0F**, um sensor de medição de distância por triangulação com **saída analógica**. Segundo o datasheet:

- **faixa de medição do sensor:** **20 a 150 cm**;
- **alimentação recomendada:** **4,5 V a 5,5 V**;
- **corrente típica:** **33 mA**;
- **tipo de saída:** tensão analógica em **VO**;
- **pinagem:** `1 = VO`, `2 = GND`, `3 = VCC`.

Também é recomendado pelo fabricante o uso de **capacitor de bypass de 10 µF ou mais** entre **VCC** e **GND**, montado próximo ao sensor.

---

## Objetivo funcional

O firmware coleta amostras do canal analógico **AN0**, converte o valor digital em tensão e estima a distância a partir da curva polinomial atualmente implementada no código:

$$
f(V) = 1.720473 \cdot V^3 + 4.15228 \cdot V^2 - 59.8489 \cdot V + 117.313
$$

![Curva característica do sensor](https://raw.githubusercontent.com/MattGrossi12/PIC32MK-analog_read_example/main/images/sensor_curve.jpeg)



Com base no resultado:

1. a tensão calculada é armazenada em variável global de monitoramento;
2. a distância estimada é atualizada;
3. se a distância calculada estiver fora da janela configurada no firmware, todas as saídas são desligadas;
4. caso contrário, a leitura é classificada por faixa de tensão e apenas um LED permanece ativo.

---

## Hardware alvo

### Microcontrolador
- **PIC32MK0128MCA048**

### Sensor
- **Sharp GP2Y0A02YK0F**
- **Saída analógica do sensor:** conectada ao canal **AN0 / RA0**
- **Alimentação do sensor:** **5 V**
- **Capacitor recomendado próximo ao sensor:** **10 µF** entre **VCC** e **GND**

### LEDs de indicação
- **LED vermelho:** **RB13**
- **LED verde:** **RB15**
- **LED amarelo:** **RA1**

---

## Observações elétricas importantes

Como o firmware converte a leitura do ADC usando:

- `ADC_REFERENCE_VOLTAGE = 3.3f`
- `ADC_MAX_COUNTS = 4095.0f`

A tensão medida em `AN0` é interpretada com referência de **3,3 V** e resolução de **12 bits**.

Isso é compatível com a leitura do sinal analógico do sensor, desde que a interface elétrica entre o **VO** do GP2Y0A02YK0F e o pino do microcontrolador já tenha sido validada em hardware. A alimentação do sensor deve permanecer em **5 V**, conforme o datasheet.

---

## Visão geral da arquitetura

O fluxo da aplicação é o seguinte:

1. `main.c` inicializa pinos, clock, Timer2 e ADC.
2. O **Timer2** gera eventos periódicos.
3. A interrupção do Timer2 solicita uma nova conversão ADC por software.
4. Ao final da sequência de conversão (**EOS - End Of Scan**), a ISR do ADC:
   - lê `ADCDATA0`;
   - atualiza o contador de amostras;
   - sinaliza que há uma nova amostra disponível.
5. O laço principal detecta a flag de nova amostra e executa `analog_process_sample()`.
6. O processamento calcula tensão, distância e atualiza os LEDs.

Essa divisão mantém as ISRs curtas e desloca o processamento principal para fora do contexto de interrupção.

---

## Estrutura dos arquivos

### `defs.h`
Centraliza:

- *configuration bits* do PIC32MK;
- inclusões globais (`xc.h`, `sys/attribs.h`, `stdint.h`);
- constantes de conversão ADC;
- coeficientes da curva de distância;
- janela válida atualmente usada pelo firmware;
- protótipos de inicialização de clock e timer.

Constantes relevantes do código:

- `ADC_REFERENCE_VOLTAGE = 3.3f`
- `ADC_MAX_COUNTS = 4095.0f`
- `DIST_MIN_VALID_CM = 10.0f`
- `DIST_MAX_VALID_CM = 80.0f`

### `adc_1.h` / `adc_1.c`
Responsáveis pela configuração e pelo processamento do ADC.

Principais elementos:

- canal utilizado: `AN0`;
- resolução configurada em **12 bits**;
- leitura da última amostra via `g_adc0_last_value`;
- cálculo de:
  - `g_adc_voltage`
  - `g_distance_cm`
  - `g_voltage_zone`

A função `analog_process_sample()`:

- converte a leitura bruta em tensão;
- calcula a distância por curva polinomial;
- desliga todas as saídas se a distância calculada estiver fora da janela válida configurada;
- delega a indicação visual para `Output_UpdateFromVoltage()`.

### `out_control.h` / `out_control.c`
Implementam a lógica de saída dos LEDs.

## Demonstração

### Leitura baixa ~20cm |--| 40cm

![Leitura em baixa distância](https://raw.githubusercontent.com/MattGrossi12/PIC32MK-analog_read_example/main/images/low_dist.png)

### Leitura média ~40cm |--| ~60cm

![Leitura em distância média](https://raw.githubusercontent.com/MattGrossi12/PIC32MK-analog_read_example/main/images/med_dist.png)

### Leitura alta ~60cm |-| ~80cm

![Leitura em alta distância](https://raw.githubusercontent.com/MattGrossi12/PIC32MK-analog_read_example/main/images/high_dist.png)

Faixas de tensão configuradas:

- **Verde:** `0.95 V` a `1.50 V`
- **Amarelo:** `1.55 V` a `2.10 V`
- **Vermelho:** `2.15 V` a `2.50 V`

Existe uma **banda morta** entre as faixas para reduzir comutação espúria por ruído próximo aos limiares.

### `pins.h` / `pins.c`
Definem e inicializam os pinos usados pela aplicação.

- configuram **RA0** como entrada analógica;
- configuram os LEDs como saída digital;
- garantem o desligamento inicial de todas as saídas.

### `isr.c`
Contém as rotinas de interrupção:

- **Timer2 ISR**: inicia uma nova conversão ADC por software;
- **ADC EOS ISR**: captura o resultado da conversão e sinaliza disponibilidade de nova amostra.

### `main.c`
Ponto de entrada da aplicação.

Responsabilidades:

- habilitar os *configuration bits* por meio de `_CONFIG_BITS_SOURCE`;
- inicializar periféricos;
- habilitar interrupções multi-vetoriais;
- executar o laço principal que processa amostras novas.

---

## Decisão de saída

O processamento segue esta lógica:

- calcula a tensão equivalente;
- estima a distância;
- se a distância estiver fora da janela válida do firmware, todos os LEDs são desligados;
- se a distância estiver dentro da janela válida, a tensão é comparada com as faixas configuradas e um único LED é ativado.

### Classificação por tensão

| Faixa | Intervalo de tensão | LED ativo |
|---|---:|---|
| Verde | 0.95 V a 1.50 V | Verde |
| Amarela | 1.55 V a 2.10 V | Amarelo |
| Vermelha | 2.15 V a 2.50 V | Vermelho |

Valores entre faixas ou fora desses limites também desligam todas as saídas.

---

## Observações sobre o GP2Y0A02YK0F

### Comportamento do componente

O gráfico característico do datasheet mostra que a relação entre **distância** e **tensão de saída** é claramente **não linear**. No gráfico do datasheet, a curva cresce até a região de aproximadamente **15 a 20 cm**, e o fabricante especifica a operação nominal a partir de **20 cm**. Portanto, leituras abaixo dessa faixa não devem ser tratadas como medição garantida pelo sensor. O datasheet define o sensor como um medidor analógico de **20 a 150 cm**.

### Tempo de atualização do sensor

O diagrama temporal do datasheet informa:

- primeira saída estável após até **5 ms** de energização;
- período típico de atualização da medição de aproximadamente **38,3 ms ± 9,6 ms**.

---

