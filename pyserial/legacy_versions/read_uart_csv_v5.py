#!/usr/bin/env python3
import csv
import time
from datetime import datetime
import serial

PORTA = "/dev/ttyAMA0"
BAUD = 9600
ARQUIVO_CSV = "uart_capture_frames.csv"

BYTESIZE = serial.EIGHTBITS
PARITY = serial.PARITY_NONE
STOPBITS = serial.STOPBITS_ONE

def calc_char_time_s(baud, bytesize, parity, stopbits):
    parity_bits = 0 if parity == serial.PARITY_NONE else 1
    stop_bits = 1.0 if stopbits == serial.STOPBITS_ONE else 2.0
    total_bits = 1.0 + bytesize + parity_bits + stop_bits
    return total_bits / float(baud)

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
) as ser, open(ARQUIVO_CSV, "w", newline="") as f_csv:

    writer = csv.writer(f_csv)
    writer.writerow([
        "timestamp_epoch",
        "timestamp_data_hora",
        "seq",
        "frame_len",
        "frame_hex",
        "frame_ascii"
    ])

    print(f"Monitorando {PORTA} @ {BAUD} bps (8N1)")
    print(f"Gravando em: {ARQUIVO_CSV}")
    print(f"FRAME_GAP_S = {FRAME_GAP_S * 1000:.3f} ms")
    print("Pressione Ctrl+C para encerrar.\n")

    ser.reset_input_buffer()

    seq = 0
    frame = bytearray()
    last_rx_time = None

    try:
        while True:
            chunk = ser.read(ser.in_waiting or 1)
            now = time.monotonic()

            if chunk:
                frame.extend(chunk)
                last_rx_time = now
                continue

            if frame and last_rx_time is not None and (now - last_rx_time) >= FRAME_GAP_S:
                seq += 1
                ts_epoch = time.time()
                ts_humano = datetime.now().strftime("%d/%m/%Y %H:%M:%S")
                raw = bytes(frame)

                writer.writerow([
                    f"{ts_epoch:.6f}",
                    ts_humano,
                    seq,
                    len(raw),
                    raw.hex(" ").upper(),
                    raw.decode("utf-8", errors="replace")
                ])
                f_csv.flush()

                print(
                    f"[{seq:06d}] "
                    f"[{ts_humano}] "
                    f"LEN={len(raw)} "
                    f"HEX={raw.hex(' ').upper()} "
                    f"ASCII={raw.decode('utf-8', errors='replace')!r}"
                )

                frame.clear()
                last_rx_time = None

    except KeyboardInterrupt:
        if frame:
            seq += 1
            ts_epoch = time.time()
            ts_humano = datetime.now().strftime("%d/%m/%Y %H:%M:%S")
            raw = bytes(frame)

            writer.writerow([
                f"{ts_epoch:.6f}",
                ts_humano,
                seq,
                len(raw),
                raw.hex(" ").upper(),
                raw.decode("utf-8", errors="replace")
            ])
            f_csv.flush()

        print("\nCaptura encerrada pelo usuário.")