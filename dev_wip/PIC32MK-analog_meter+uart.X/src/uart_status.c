#include "uart_status.h"

#include "defs.h"
#include "on_off_button.h"

typedef struct
{
    uint8_t b0_on_off;
    uint8_t b1_emergency;
    uint8_t b2_remote;
    uint8_t b3_reserved;
    uint8_t b4_reserved;
    uint8_t b5_reserved;
    uint8_t b6_box;
    uint8_t b7_box;
} uart_protocol_fields_t;

static volatile uint8_t g_uart_box_code = UART_BOX_CODE_NONE;

static void uart_set_box_bits_from_code(uart_protocol_fields_t *fields)
{
    /*
     * Mapeamento final confirmado em bancada:
     * - Caixa P -> 10 no campo final B6:B7 visual
     * - Caixa M -> 01 no campo final B6:B7 visual
     * - Caixa G -> 11
     * - Sem caixa -> 00
     *
     * Como o empacotamento visual coloca B6 no bit 1 e B7 no bit 0,
     * definimos aqui os campos logicos exatamente nessa ordem.
     */
    switch (g_uart_box_code)
    {
        case UART_BOX_CODE_P:
            fields->b6_box = 0u;
            fields->b7_box = 1u;
            break;

        case UART_BOX_CODE_M:
            fields->b6_box = 1u;
            fields->b7_box = 0u;
            break;

        case UART_BOX_CODE_G:
            fields->b6_box = 1u;
            fields->b7_box = 1u;
            break;

        case UART_BOX_CODE_NONE:
        default:
            fields->b6_box = 0u;
            fields->b7_box = 0u;
            break;
    }
}

static void uart_fill_protocol_fields(uart_protocol_fields_t *fields)
{
    const system_state_t state = on_off_button_get_state();

    fields->b0_on_off = (state == STATE_ON) ? 1u : 0u;
    fields->b1_emergency = (state == STATE_EMERGENCY) ? 1u : 0u;
    fields->b2_remote = 0u;
    fields->b3_reserved = 0u;
    fields->b4_reserved = 0u;
    fields->b5_reserved = 0u;

    uart_set_box_bits_from_code(fields);
}

static uint8_t uart_pack_visual_b0_to_b7(const uart_protocol_fields_t *fields)
{
    uint8_t tx_byte = 0u;

    tx_byte |= (uint8_t)(fields->b0_on_off << 7);
    tx_byte |= (uint8_t)(fields->b1_emergency << 6);
    tx_byte |= (uint8_t)(fields->b2_remote << 5);
    tx_byte |= (uint8_t)(fields->b3_reserved << 4);
    tx_byte |= (uint8_t)(fields->b4_reserved << 3);
    tx_byte |= (uint8_t)(fields->b5_reserved << 2);
    tx_byte |= (uint8_t)(fields->b6_box << 1);
    tx_byte |= (uint8_t)(fields->b7_box << 0);

    return tx_byte;
}

void uart_status_init(void)
{
    g_uart_box_code = UART_BOX_CODE_NONE;
}

void uart_status_set_box_code(uint8_t box_code)
{
    if (box_code > UART_BOX_CODE_G)
    {
        box_code = UART_BOX_CODE_NONE;
    }

    g_uart_box_code = box_code;
}

uint8_t uart_status_get_box_code(void)
{
    return g_uart_box_code;
}

void uart_status_notify_state_changed(void)
{
    /*
     * Mantida apenas para preservar a interface existente.
     * O envio UART agora volta a ser decidido no main, comparando
     * o byte atual do protocolo com o ultimo byte transmitido.
     */
}

uint8_t uart_build_status_byte(void)
{
    uart_protocol_fields_t fields;

    uart_fill_protocol_fields(&fields);

    return uart_pack_visual_b0_to_b7(&fields);
}
