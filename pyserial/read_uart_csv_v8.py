#!/usr/bin/env python3
import csv
import os
import re
import time
from datetime import datetime
from pathlib import Path

import serial

PORTA = "/dev/ttyAMA0"
BAUD = 9600

# Arquivos no mesmo diretorio do script.
BASE_DIR = Path(__file__).resolve().parent
ARQUIVO_RX_TX_CSV = BASE_DIR / "uart_capture_rx_tx.csv"
ARQUIVO_DATA_TO_SEND = BASE_DIR / "data_to_send.csv"

BYTESIZE = serial.EIGHTBITS
PARITY = serial.PARITY_NONE
STOPBITS = serial.STOPBITS_ONE

# Se True, ignora linhas antigas ja existentes no data_to_send.csv ao iniciar
# e envia apenas as novas linhas adicionadas depois que o script comecar.
START_FROM_END = True

# Intervalo de polling do arquivo e da UART.
POLL_S = 0.02


def calc_char_time_s(baud, bytesize, parity, stopbits):
    parity_bits = 0 if parity == serial.PARITY_NONE else 1
    stop_bits = 1.0 if stopbits == serial.STOPBITS_ONE else 2.0
    total_bits = 1.0 + bytesize + parity_bits + stop_bits
    return total_bits / float(baud)


def bytes_to_bin_msb(data: bytes) -> str:
    return " ".join(f"{b:08b}" for b in data)


def ascii_printable(data: bytes) -> str:
    return "".join(chr(b) if 32 <= b <= 126 else "." for b in data)


def log_frame(writer, seq, direction, raw: bytes):
    ts_epoch = time.time()
    ts_humano = datetime.now().strftime("%d/%m/%Y %H:%M:%S")

    writer.writerow([
        f"{ts_epoch:.6f}",
        ts_humano,
        seq,
        direction,
        len(raw),
        bytes_to_bin_msb(raw),
        ascii_printable(raw)
    ])
    return ts_humano


_BIN_TOKEN_RE = re.compile(r"^[01]{8}$")


def parse_csv_line_to_byte(line: str):
    """
    Extrai 1 byte binario de uma linha CSV.
    Aceita linhas como:
      00011000
      "00011000"
      00011000,alguma_coisa
      seq,00011000

    Retorna:
      bytes([valor])  se encontrar um token binario de 8 bits
      None            se a linha estiver vazia, incompleta ou invalida
    """
    line = line.strip()
    if not line:
        return None

    try:
        row = next(csv.reader([line]))
    except Exception:
        return None

    for field in row:
        token = field.strip()
        if _BIN_TOKEN_RE.fullmatch(token):
            return bytes([int(token, 2)])

    return None


CHAR_TIME_S = calc_char_time_s(BAUD, 8, PARITY, STOPBITS)
FRAME_GAP_S = 5.0 * CHAR_TIME_S

with serial.Serial(
    port=PORTA,
    baudrate=BAUD,
    bytesize=BYTESIZE,
    parity=PARITY,
    stopbits=STOPBITS,
    timeout=0.02,
    inter_byte_timeout=0.01,
    xonxoff=False,
    rtscts=False,
    dsrdtr=False,
) as ser, open(ARQUIVO_RX_TX_CSV, "w", newline="") as f_csv:

    writer = csv.writer(f_csv)
    writer.writerow([
        "timestamp_epoch",
        "timestamp_data_hora",
        "seq",
        "direction",
        "frame_len",
        "frame_bin_msb",
        "frame_ascii_printable"
    ])

    print(f"Monitorando UART {PORTA} @ {BAUD} bps (8N1)")
    print(f"Log RX/TX: {ARQUIVO_RX_TX_CSV}")
    print(f"Arquivo de envio: {ARQUIVO_DATA_TO_SEND}")
    print(f"FRAME_GAP_S = {FRAME_GAP_S * 1000:.3f} ms")
    if START_FROM_END:
        print("Modo de envio: somente novas linhas adicionadas apos o inicio.")
    else:
        print("Modo de envio: processa tambem linhas ja existentes ao iniciar.")
    print("Pressione Ctrl+C para encerrar.\n")

    ser.reset_input_buffer()

    seq = 0

    # Estado da captura RX
    rx_frame = bytearray()
    last_rx_time = None

    # Estado do monitoramento do CSV de envio
    send_file_pos = 0
    send_partial = ""
    send_file_inode = None
    send_file_initialized = False

    try:
        while True:
            # ------------------------------------------------------
            # 1) Captura RX do MCU
            # ------------------------------------------------------
            chunk = ser.read(ser.in_waiting or 1)
            now = time.monotonic()

            if chunk:
                rx_frame.extend(chunk)
                last_rx_time = now
            elif rx_frame and last_rx_time is not None and (now - last_rx_time) >= FRAME_GAP_S:
                seq += 1
                raw = bytes(rx_frame)
                ts_humano = log_frame(writer, seq, "RX", raw)
                f_csv.flush()

                print(
                    f"[{seq:06d}] "
                    f"[{ts_humano}] "
                    f"DIR=RX "
                    f"LEN={len(raw)} "
                    f"BIN={bytes_to_bin_msb(raw)} "
                    f"TXT={ascii_printable(raw)!r}"
                )

                rx_frame.clear()
                last_rx_time = None

            # ------------------------------------------------------
            # 2) Monitora data_to_send.csv e envia novas linhas na TX
            # ------------------------------------------------------
            if ARQUIVO_DATA_TO_SEND.exists():
                stat = ARQUIVO_DATA_TO_SEND.stat()
                current_inode = getattr(stat, "st_ino", None)
                current_size = stat.st_size

                # Inicializacao ou reabertura apos recriacao do arquivo
                if not send_file_initialized or send_file_inode != current_inode:
                    send_file_inode = current_inode
                    send_partial = ""
                    send_file_pos = current_size if START_FROM_END else 0
                    send_file_initialized = True

                # Se o arquivo foi truncado, reinicia da nova posicao.
                if current_size < send_file_pos:
                    send_file_pos = 0
                    send_partial = ""

                if current_size > send_file_pos:
                    with open(ARQUIVO_DATA_TO_SEND, "r", encoding="utf-8", newline="") as f_send:
                        f_send.seek(send_file_pos)
                        new_text = f_send.read()
                        send_file_pos = f_send.tell()

                    send_partial += new_text

                    while True:
                        newline_idx = send_partial.find("\n")
                        if newline_idx < 0:
                            break

                        line = send_partial[:newline_idx]
                        send_partial = send_partial[newline_idx + 1:]

                        tx_raw = parse_csv_line_to_byte(line)
                        if tx_raw is None:
                            stripped = line.strip()
                            if stripped:
                                print(f"Ignorado em {ARQUIVO_DATA_TO_SEND.name}: {stripped!r}")
                            continue

                        ser.write(tx_raw)
                        ser.flush()

                        seq += 1
                        ts_humano = log_frame(writer, seq, "TX", tx_raw)
                        f_csv.flush()

                        print(
                            f"[{seq:06d}] "
                            f"[{ts_humano}] "
                            f"DIR=TX "
                            f"LEN={len(tx_raw)} "
                            f"BIN={bytes_to_bin_msb(tx_raw)} "
                            f"TXT={ascii_printable(tx_raw)!r}"
                        )

            time.sleep(POLL_S)

    except KeyboardInterrupt:
        pass

    finally:
        if rx_frame:
            seq += 1
            raw = bytes(rx_frame)
            log_frame(writer, seq, "RX", raw)
            f_csv.flush()

        print("\nCaptura encerrada pelo usuário.")
