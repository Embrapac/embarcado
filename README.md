# 📊 Repositório para agrupamento do projeto do sistema embarcado da Embrapac:
Parte integrante do fluxo de trabalho do projeto final da residência do CPQD.

## Dispositivo em uso: 

PIC32MK MCACURIOSITYPRODEVELOPMENTBOARD


| **Descrição** | **Imagem** |
| :--- | :--- |
| **Vista baixa da PCB** | ![Vista baixa da PCB:](https://github.com/Embrapac/embarcado/blob/main/images/1.pic_pcb_view.jpeg) |
| **Divisão interna de blocos do CI** | ![Divisão interna de blocos do CI:](https://github.com/Embrapac/emvbarcado/blob/main/images/2.representacao_de_topo.png) |
| **Interface dedicada Arduino** | ![Interface dedicada Arduino:](https://github.com/Embrapac/embarcado/blob/main/images/3.interface_arduino.png) |
| **Interface de teste** | ![Interface de teste:](https://github.com/Embrapac/embarcado/blob/main/images/4.pinos_blink_control.png) |
| **Pinos dedicados ao sistema de controle de motores 1** | ![Lista da interface de controle de motores 1](https://github.com/Embrapac/embarcado/blob/main/images/5.lista_da_interface_de_controle_de_motores_1.png) |
| **Pinos dedicados ao sistema de controle de motores 2** | ![Lista da interface de controle de motores 2](https://github.com/Embrapac/embarcado/blob/main/images/6.lista_da_interface_de_controle_de_motores_2.png) |
| **Pinos dedicados para as entradas do banco 1** | ![Pinos dedicados para as entradas do banco 1:](https://github.com/Embrapac/embarcado/blob/main/images/7.lista_de_pinos_entrada_1.png) |
| **Pinos dedicados para as entradas do banco 2** | ![Pinos dedicados para as entradas do banco 2:](https://github.com/Embrapac/embarcado/blob/main/images/8.lista_de_pinos_entrada_2.png) |


## Ferramenta necessária para desenvolvimento no PIC: 
https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide

### Passos para o desenvolvimento:

```
> 1º - Acessar a aba File, no canto superior esquerdo;
```
```
> 2º - Haverão dois campos principais: Categories e Projects, no primeiro escolha "Microchip Embedded", e no segundo selecione "Application Project(s)";
```
```
> 3º - Após isso, selecione o botão "next" no canto inferior deste sub-menu;
```
```
> 4º - Em Family, selecione a opção que for melhor para a sua aplicação, em nosso caso estamos utilizando um PIC 32 MK, portanto selecionaremos a opção "32-BIT MCUs (PIC32)";
```
```
> 5º - Em device iremos escolher o dispositivo em uso que aqui será o PIC32MK0128MCA028
```
```
> 6º - Em tool há diversas opções de feramentas, selecionaremos o simulator pois não estamos com o hardware em bancada.
```
```
> 7º - Após isso, dê um next, e selecione o compilador, que será o XC32.
```
```
> 8º - Em Project name colocaremos, "embrapac", e o caminho defini o mesmo que minha branch local, neste caso foi "/home/matheus/Área de trabalho/projeto/embarcado".
```
```
> 9º - Após a execução da estrutura de pastas internas geradas pelo programa, foi criada uma pasta com o nome de "embrapac.X" que irá armazenar os dados de projeto.
```
---
