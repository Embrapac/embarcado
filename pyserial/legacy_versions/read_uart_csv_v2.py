#!/usr/bin/env python3
import csv
import time
import serial

PORTA = "/dev/ttyAMA0"  # ajuste se necessário
BAUD = 115200
ARQUIVO_CSV = "uart_capture.csv"
PAYLOAD_ESPERADO = b"00011000"

with serial.Serial(
    port=PORTA,
    baudrate=BAUD,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    timeout=12,
    inter_byte_timeout=0.02,
    xonxoff=False,
    rtscts=False,
    dsrdtr=False,
) as ser, open(ARQUIVO_CSV, "w", newline="") as f_csv:

    writer = csv.writer(f_csv)
    writer.writerow([
        "timestamp_epoch",
        "seq",
        "raw_hex",
        "raw_ascii",
        "payload_ok"
    ])

    print(f"Monitorando {PORTA} @ {BAUD} bps (8N1)")
    print(f"Gravando em: {ARQUIVO_CSV}")
    print("Pressione Ctrl+C para encerrar.\n")

    seq = 0

    try:
        while True:
            raw = ser.read_until(b"\n")  # lê a mensagem completa até LF
            if not raw:
                continue

            seq += 1
            ts = time.time()

            payload = raw.strip(b"\r\n")
            ok = (payload == PAYLOAD_ESPERADO)

            writer.writerow([
                f"{ts:.6f}",
                seq,
                raw.hex(" ").upper(),
                raw.decode("ascii", errors="replace").rstrip("\n"),
                int(ok)
            ])
            f_csv.flush()

            if ok:
                print(f"[{seq:06d}] OK    RAW={raw!r}")
            else:
                print(f"[{seq:06d}] ERRO  RAW={raw!r}  PAYLOAD={payload!r}")

    except KeyboardInterrupt:
        print("\nCaptura encerrada pelo usuário.")
