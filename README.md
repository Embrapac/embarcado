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

$$
R(\Omega) = \frac{V_{pp} - V_{led}}{I_{led}}
$$

Para isso será necessário utilizar um pino que tenha capacidade produzir um sinal digital, para cada tomando o cuidado de não usar pinos reservados, para o Arduino será utilizado o pino D3.

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

```bash
#include <xc.h>

int main(void)
{
    // Configura RA0 como saída
    TRISAbits.TRISA10 = 0;

    while (1)
    {
        // liga LED externo
        LATAbits.LATA0 = 1;  
        for (volatile int i = 0; i < 500000; i++);
        
        // desliga LED externo
        LATAbits.LATE0 = 0;  
        for (volatile int i = 0; i < 500000; i++);
    }
}