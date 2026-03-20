# Projeto prático de nº2 do programa de especialização/residência em sistemas eletrônicos embarcados.
Organizado em uma parceria MCTI/Softex + CPQD + FIAP

---

#### Para dúvidas, comentários ou sugestões fiquem à vontade, estarei à disposição através de minha página: 

[Matheus Grossi](https://www.linkedin.com/in/matheus-grossi/)

Link do projeto no Wokwi:

[fiap_embedded_projects_2](https://wokwi.com/projects/454130082230099969)

Link do dashboard no Ubidots:

[Ubidots_dashboard](https://stem.ubidots.com/app/dashboards/69701418221d689174891929)

---
## Objetivo?
<div align="justify">
<br>
Ferramentas: Wokwi +
HiveMQ (ou outro broker MQTT) + Plataforma Ubidots. + Sistema desejado para Integração:
<br>
<br>
1 - Criar
device no Ubidots (2 a 4 variáveis de telemetria a critério do estudante) - Pode reutilizar a Atividade 1 mas tente ser criativo melhorando o projeto anterior.
<br>
<br>
2 - Configurar
dashboard com widgets para o device criado - DICA: Como o Dashboard tem limite de 10 Variaveis, pode usar variaveis de contexto para aumentar esse limite. Veja Aulas da Semana 6.
<br>
<br>
3 - No Wokwi, publicar telemetria
MQTT em algum Broker(MQTTx, HiveMQ, Mosquito, etc)
<br>
<br>
4 - No
Ubidots, configurar integração MQTT usando Token do device/variáveis para
consumir dados do Broker (via bridge, plugin, ou fluxo suportado pela
plataforma, pode ser criado um outro sistema com ESP32 como retransmissor como ex. da Semana 7).
<br>
<br>
5 - Validar se
os dados do device estão atualizando no dashboard do Ubidots - Gere um relatorio (pode ser capturado do monitor serial) da origem dos dados e comparar com o resultado final. Acrescente na documentação.
<br>
<br>
6 - Inserir print do dashboard atualizado na documentação
<br>
<br>
7- Criar uma documentação adequada mostrando um rascunho simples da arquitetura, objetivo do projeto, descrição geral do sistema, explicação de cada componente, seu fluxo de interação entre eles, descrição das variaveis de publicação e subscrição e conclusoes. Faça um documento formal com capa, titulo, indices, etc.
<br>
<br>
8 - Fazer um pequeno video de poucos minutos mostrando o funcionamento dos sistemas, pode usar captura de tela.

---
## Índice:

1. [Descrição do projeto](https://github.com/MattGrossi12/fiap_embedded_projects_2/tree/main?tab=readme-ov-file#1-descri%C3%A7%C3%A3o-do-projeto)
2. [Dimensionamento e lista de materiais](https://github.com/MattGrossi12/fiap_embedded_projects_2/tree/main?tab=readme-ov-file#2-dimensionamento-e-lista-de-materiais)
<br>2.1. [Cálculo do resistor para os LED](https://github.com/MattGrossi12/fiap_embedded_projects_2/tree/main?tab=readme-ov-file#21-c%C3%A1lculo-do-resistor-para-os-led)
<br>2.2. [Dimensionamento dos resistores para os LEDs](https://github.com/MattGrossi12/fiap_embedded_projects_2/tree/main?tab=readme-ov-file#22-dimensionamento-dos-resistores-para-os-leds)
3. [Lista de materiais utilizados nesta aplicação](https://github.com/MattGrossi12/fiap_embedded_projects_2/tree/main?tab=readme-ov-file#3-lista-de-materiais-utilizados-nesta-aplica%C3%A7%C3%A3o)
<br>3.1. [Critérios de projeto](https://github.com/MattGrossi12/fiap_embedded_projects_2/tree/main?tab=readme-ov-file#31-crit%C3%A9rios-de-projeto)
4. [Conexão ao canal de comunicação com o broker Mosquitto/EMQX](https://github.com/MattGrossi12/fiap_embedded_projects_2/tree/main?tab=readme-ov-file#4-conex%C3%A3o-ao-canal-de-comunica%C3%A7%C3%A3o-com-o-broker-mosquittoemqx)
5. [Arquitetura do firmware](https://github.com/MattGrossi12/fiap_embedded_projects_2/tree/main?tab=readme-ov-file#5-arquitetura-do-firmware)
6. [Interação ao Ubidots](https://github.com/MattGrossi12/fiap_embedded_projects_2/tree/main?tab=readme-ov-file#6-intera%C3%A7%C3%A3o-ao-ubidots)


---
## 1. Descrição do projeto:

Este trabalho se objetivou a criação de um sistema simples de automação residêncial utilizando conceitos de sistemas embarcados e IOT, o escopo de funções abrange:

> A medição de gases tóxicos/perigosos no ambiente residencial como gases combustíveis, incluindo GLP, propano, hidrogênio, metano e monóxido de carbono e emissão de alerta sonoro/visual em caso de detecção, além de aviso no dashboard.

> A detecção da luminosidade da área externa, integrado a uma automação simples para acionamento das luzes quando o ambiente estiver escuro.

> A medição de temperatura ambiente, que poderia posteriormente ser interligada a um sistema de climatização caso o usuário assim deseje.


### 2. Dimensionamento e lista de materiais: 


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

*Vale ressaltar que a tensão máxima de trabalho no gpio do ESP32-S3-DEVKITC-1-N8R8 é de 3.3V, portanto Vpp = 3.3V*

<div align="center">

| **Tabela de caracteristicas técnicas dos LEDs** |
| :---: |
![tabela_led](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/tabela_led.png)
| **Tabela de resistores comerciais** |
![tabela_resistores](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/tabela_resistores.png)
| **Diagrama de potências de resistores comerciais** |
![tabela_resistores](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/pot_resistores.png)

<div align="justify">

#### 2.2 Dimensionamento dos resistores para os LEDs:

O led vermelho em carga plena utiliza 1.7V, e sua corrente é de 10mA (0.01A), para tal:

<br>
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

<div align="justify">
Como vimos na tabela acima esse valor de resistor é uma medida comercial, no entanto, não existe resistor de 0.024W, o valor comercial mais próximo arredondando para cima é o de 1/16W (0.0625W).

---
O led amarelo em carga plena utiliza 1.7V, e sua corrente é de 20mA (0.02A)

<br>

$$
R(\Omega) = \frac{3.3 - 1.7}{0.02} = \frac{1.6}{0.02} = 80\Omega
$$

<br>

$$
P(W) = U*i = 1.6 * 0.02 = 0.032W
$$

<br>

$$
Pmin = U*i = 0.032W * (1+0.50)=0.048W
$$
<br>

<div align="justify">
Como vimos na tabela acima esse valor de resistor, não é uma medida comercial, portanto ao arredondar usaremos um de 82R, além disso, não existe resistor de 0.048W, o valor comercial mais próximo arredondando para cima é o de 1/16W (0.0625W).

## 3 Lista de materiais utilizados nesta aplicação:

 
<div align="center">

| **Item** | **Imagem** | **Link referêcia** |
| :--- | :---: | :---: |
| **ESP32-S3-DEVKITC-1-N8R8** | ![ESP32-S3-DEVKITC-1-N8R8](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/1.esp32.png) | [Link-1](https://www.digikey.com.br/en/products/detail/espressif-systems/ESP32-S3-DEVKITC-1-N8R8/15295894) |
| **Display LCD 20x4** | ![Display LCD 20x4](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/2.display.jpg) | [Link-2](https://www.usinainfo.com.br/display-arduino/display-lcd-20x4-com-fundo-azul-2727.html?srsltid=AfmBOoo_T0yx8DeO8kDobWg-LXnzvC8cdNzDUk0HhwmnHMre7m4wweJD) |
| **Sensor de temperatura NTC-10K** | ![Sensor de temperatura NTC-10K](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/3.sensor_de_temperatura.png) | [Link-3](https://www.arducore.com.br/modulo-sensor-de-temperatura-ntc?srsltid=AfmBOoq0kWVqQLXZ2cuCdpVPkEVKR5jUI-C0HkQP3z2ZFlozv6QAMyRD) |
| **Sensor de luminosidade (LDR)** | ![Sensor de luminosidade (LDR)](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/4.sensor_de_luminosidade.png) | [Link-4](https://www.usinainfo.com.br/sensor-de-luminosidade/modulo-sensor-de-luminosidade-ldr-hw-072-para-arduino-e-esp32-2539.html?srsltid=AfmBOoqh2a2FSQJ-WsQwSs6_H3h9eT0_MXp4nbUpsYRxL-ytK1wvdg2d) |
| **Sensor de gás** | ![Sensor de gás](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/5.sensor_de_gas.png) | [Link-5](https://www.eletrogate.com/sensor-de-gas-mq-6-glp-propano-e-iso-butano?srsltid=AfmBOor93c2GLfcym9Ea-exo15aaudNBmn3tdkNTRiq2efnVQ1H4DsF7) |
| **Módulo Relé** | ![Módulo Relé](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/6.modulo_rele.png) | [Link-6](https://www.autocorerobotica.com.br/modulo-rele-5v-1-canal-high-ou-low-trigger?srsltid=AfmBOooc7hmlZ66WW1IAvDfkgiAtKHC0fGlh_Dl073uQg76DFQskytyW) |
| **Buzzer** | ![Buzzer](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/7.modulo_buzzer.png) | [Link-7](https://38-3d.co.uk/products/active-buzzer-module-3-3v-5v?srsltid=AfmBOorTXmQ61TX4eDaEroojsn16cxy4yOKYKK0hJKutVKGQzq2nbawm) |
| **Led vermelho** | ![Led vermelho](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/8.led_vermelho.png) | [Link-8](https://www.mercadolivre.com.br/kit-1000-leds-difusos-lamp-5mm-vermelhos-para-projetos-eletrnicos/p/MLB43986681?pdp_filters=item_id%3AMLB5197656606&from=gshop&matt_tool=91562990&matt_internal_campaign_id=&matt_word=&matt_source=google&matt_campaign_id=22090193891&matt_ad_group_id=174661984004&matt_match_type=&matt_network=g&matt_device=c&matt_creative=727914181090&matt_keyword=&matt_ad_position=&matt_ad_type=pla&matt_merchant_id=735098660&matt_product_id=MLB43986681-product&matt_product_partition_id=2392713578861&matt_target_id=aud-1967156880386:pla-2392713578861&cq_src=google_ads&cq_cmp=22090193891&cq_net=g&cq_plt=gp&cq_med=pla&gad_source=1&gad_campaignid=22090193891&gbraid=0AAAAAD93qcB9rle_WedL6atW03-Klp6f_&gclid=Cj0KCQiA-NHLBhDSARIsAIhe9X0RUuj74fUpQcMN1MXpvSVynh04RoKwYq0xg9n8QLxBREtiGrHlpbEaAl_qEALw_wcB) |
| **Led amarelo** | ![Led amarelo](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/9.led_amarelo.png) | [Link-9](https://www.dualshop.com.br/led-5mm-amarelo-amarelo-standard-difuso-pth?srsltid=AfmBOop2r85YTH3l7Al_zvDkvm4G4wAGqd8xIjWce4ekCzflslonKj03) |
| **Resistor 160R 1/16W** | ![Resistor 160R 1/16W](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/10.res_160r.png) | [Link-10](https://curtocircuito.com.br/resistor-160r-1-4w-5.html?srsltid=AfmBOooummxSfqPPxtEGCP2oIGFGCHLwrRuSj9kSmuDmM7-aB_z1A_wM) |
| **Resistor 82R 1/16W** | ![Resistor 82R 1/16W](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/11.res_82r.png) | [Link-11](https://www.eletronicacuiaba.com/resistor-82r-1w?srsltid=AfmBOoolW35wzi-zIkhnYBgZPu3kQA6LoSaOEcVuWnducEBeAkPMe61_) |

<div align="justify">

### 3.1 Critérios de projeto:

<div align="justify">
A Imagem abaixo ilustra o mapa de I/Os do microcontrolador de nosso projeto:
<br>
<div align="center">

| **ESP32-S3-DEVKITC-1-N8R8** |
| :---: |
![ESP32-S3-DEVKITC-1-N8R8](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/esp_pinout.png)

<div align="justify">
A imagem nos demonstra que alguns pinos são para funções específicas do MCU, e devem ser usadas com cuidado.
Em nosso projeto, foi adotado o seguinte mapa de conexões:

<br>

<div align="center">

| **Pino** | **Função** |
| :---: | :--- |
| GPIO 1 | Entrada Analógica: Sensor de Temperatura (NTC) |
| GPIO 2 | Entrada Analógica: Sensor de Luminosidade (LDR) |
| GPIO 3 | Entrada Digital: Sensor de Gás (MQ — Saída Digital) |
| GPIO 45 | Saída Digital: LCD - RS (Register Select) |
| GPIO 48 | Saída Digital: LCD - EN (Enable) |
| GPIO 39 | Saída Digital: LCD - D4 (Barramento de Dados) |
| GPIO 40 | Saída Digital: LCD - D5 (Barramento de Dados) |
| GPIO 41 | Saída Digital: LCD - D6 (Barramento de Dados) |
| GPIO 42 | Saída Digital: LCD - D7 (Barramento de Dados) |

<div align="justify">

Essa é a representação visual das conexões:

<div align="center">

| **Diagrama de conexões** |
| :---: |
![Diagrama de conexões](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/ope_tel_0.png)

<div align="justify">

## 4. Conexão ao canal de comunicação com o broker Mosquitto/EMQX

Para simplificar o uso do broker emqx em conjunto ao mosquitto, foi desenvolvido este arquivo `Makefile` que possui 3 modos de operação:

1. Ao usar este comando no terminal, é realizada a exclusão do arquivo mqtt_log.csv que armazena os dados de operação recebidos do sistema:
```bash
make clean
```
2. Ao usar este comando no terminal, é criado o canal de escuta, fazendo com que os dados enviados pelo sistema sejam recebidos e armazenados no arquivo mqtt_log.csv
```bash
make listen
```
3. Ao usar este comando no terminal, é criado o canal de publicação, o que permite enviar dados ao sistema, em nosso caso essa função foi implementada para uso futuro, atualmente não há vantagem/função para dados enviados ao sistema.
```bash
make pub
```

<div align="justify">

Essa é a representação visual das conexões:

<div align="center">

| **Ação** |
| :--- |
| O estado inicial é formado por um boot em duas etapas, sendo esta a 1º, nesse momento é feita a conexão de Wi-fi, paralelamento é demonstrado o cabeçalho no display que pode ser observado na imagem abaixo.|
|![Ope_tel_1](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/ope_tel_1.png)|
| O 2º estágio do boot transiciona para uma apresentação breve do programa de estudos, enquanto que paralalemante é feita a conexão ao broker MQTT|
|![Ope_tel_2](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/ope_tel_2.png)|
| Essa já é a tela de operação em regime permanente, composta por 4 linhas, onde é possível acompanhar a temperatura atual, verificar o estado de detecção de gás, o estado das luzes autônomas e ainda, monitorar se o MQTT está operando normalmente, caso a conexão ao Wi-fi e ao MQTT estejam ativas, os dados da operação serão enviados ao broker onde serão armazenados no mqtt_log.csv|
|![Ope_tel_3](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/ope_tel_3.png)|

<div align="justify">

## 5. Arquitetura do firmware:

<div align="center">

| **Arquivo** | **Função** |
| :--- | :--- |
| [main.cpp](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/src/main.cpp)   | Responsável por processar e executar as funções primárias. |
| [wifi.cpp](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/src/wifi.cpp)   | Centraliza os comandos necessários para conexão ao wi-fi. |
| [mqtt.cpp](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/src/mqtt.cpp)  | Centraliza os comandos necessários para conexão ao broker. |
| [ubidots.cpp](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/src/ubidots.cpp) | Adiciona o suporte ao Ubidots, permitindo a implementação de dashboards |
| [wifi_con.h](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/src/wifi_con.h) | Chamada das funções recursivas de conexão ao wi-fi. |
| [mqtt_con.h](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/src/mqtt_con.h) | Chamada das funções recursivas de conexão ao MQTT. |
| [ubidots_con.h](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/src/ubidots_con.h) | Chamada das funções recursivas de conexão ao Ubidots |

<div align="justify">

## 6. Interação ao Ubidots:

Através desse processo torna-se possível a implementação de um dashboard para montiramento das grandezas, além disso, caso haja entradas intertravadas, adiciona asism um controle remoto do sistema, em nosso ambiente foi implementado desta forma:

| **Imagens do dashboard em operação** |
| :--- |
|![dash_1](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/dash_1.png)|
|![dash_2](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/dash_2.png)|
|![dash_3](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/dash_3.png)|
|![dash_4](https://github.com/MattGrossi12/fiap_embedded_projects_2/blob/main/images/dash_4.png)|
---

