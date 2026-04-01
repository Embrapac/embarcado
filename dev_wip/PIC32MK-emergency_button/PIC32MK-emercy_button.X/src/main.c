/*
 * File:   main.c
 * Author: Matheus Grossi
 * Created on 13 de Fevereiro de 2026, 14:11
 */

#define _CONFIG_BITS_SOURCE

#include "defs.h"
#include "on_off_button.h"
#include "pins.h"

int main(void)
{
    init();

    while (1) {
        operation();
    }

    return 0;
}
