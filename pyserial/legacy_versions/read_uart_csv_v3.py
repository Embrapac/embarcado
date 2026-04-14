#!/usr/bin/env python3
import csv
import time
import serial

PORTA = "/dev/ttyAMA0"
BAUD = 115200
ARQUIVO_CSV = "uart_capture_gap.csv"

BYTESIZE = serial.EIGHTBITS
PARITY = serial.PARITY_NONE
STOPBITS = serial.STOPBITS_ONE

def calc_char_time_s(baud, bytesize, parity, stopbits):
    parity_bits = 0 if parity == serial.PARITY_NONE else 1
    stop_bits = 1.0 if stopbits == serial.STOPBITS_ONE else 2.0
    total_bits = 1.0 + bytesize + parity_bits + stop_bits   # start + data + parity + stop
    return total_bits / float(baud)

CHAR_TIME_S = calc_char_time_s(BAUD, 8, PARITY, STOPBITS)
FRAME_GAP_S = 3.5 * CHAR_TIME_S   # silêncio acima disso separa frames

with serial.Serial(
    port=PORTA,
    baudrate=BAUD,
    bytesize=BYTESIZE,
    parity=PARITY,
    stopbits=STOPBITS,
    timeout=0.05,
    inter_byte_timeout=0.01,
    xonxoff=False,
    rtscts=False,
    dsrdtr=False,
    exclusive=True,
) as ser, open(ARQUIVO_CSV, "w", newline="") as f_csv:

    writer = csv.writer(f_csv)
    writer.writerow([
        "timestamp_epoch",
        "seq",
        "frame_len",
        "frame_hex",
        "frame_ascii"
    ])

    ser.reset_input_buffer()

    print(f"Monitorando {PORTA} @ {BAUD} bps")
    print(f"char_time = {CHAR_TIME_S * 1000:.3f} ms")
    print(f"frame_gap = {FRAME_GAP_S * 1000:.3f} ms")
    print(f"Gravando em: {ARQUIVO_CSV}")
    print("Pressione Ctrl+C para encerrar.\n")

    seq = 0
    frame = bytearray()
    last_rx_time = None

    try:
        while True:
            chunk = ser.read(ser.in_waiting or 1)
            now = time.monotonic()

            if chunk:
                # Se houve silêncio suficiente antes deste chunk, fecha o frame anterior
                if frame and last_rx_time is not None and (now - last_rx_time) > FRAME_GAP_S:
                    seq += 1
                    raw = bytes(frame)

                    writer.writerow([
                        f"{time.time():.6f}",
                        seq,
                        len(raw),
                        raw.hex(" ").upper(),
                        raw.decode("utf-8", errors="replace")
                    ])
                    f_csv.flush()

                    print(
                        f"[{seq:06d}] LEN={len(raw):03d} "
                        f"HEX={raw.hex(' ').upper()} "
                        f"ASCII={raw.decode('utf-8', errors='replace')!r}"
                    )

                    frame.clear()

                frame.extend(chunk)
                last_rx_time = now
                continue

            # timeout sem novos bytes: se o gap estourou, fecha o frame
            if frame and last_rx_time is not None and (now - last_rx_time) > FRAME_GAP_S:
                seq += 1
                raw = bytes(frame)

                writer.writerow([
                    f"{time.time():.6f}",
                    seq,
                    len(raw),
                    raw.hex(" ").upper(),
                    raw.decode("utf-8", errors="replace")
                ])
                f_csv.flush()

                print(
                    f"[{seq:06d}] LEN={len(raw):03d} "
                    f"HEX={raw.hex(' ').upper()} "
                    f"ASCII={raw.decode('utf-8', errors='replace')!r}"
                )

                frame.clear()
                last_rx_time = None

    except KeyboardInterrupt:
        if frame:
            seq += 1
            raw = bytes(frame)
            writer.writerow([
                f"{time.time():.6f}",
                seq,
                len(raw),
                raw.hex(" ").upper(),
                raw.decode("utf-8", errors="replace")
            ])
            f_csv.flush()

        print("\nCaptura encerrada pelo usuário.")
