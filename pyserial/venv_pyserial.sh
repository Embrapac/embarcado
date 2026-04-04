#!/usr/bin/env bash
set -euo pipefail

# Uso:
#   chmod +x venv_pyserial.sh
#   ./venv_pyserial.sh
#
# Opcional:
#   ./venv_pyserial.sh meu_venv

VENV_DIR="${1:-.venv}"

if ! command -v python3 >/dev/null 2>&1; then
    echo "Erro: python3 não encontrado no sistema."
    exit 1
fi

echo "Criando ambiente virtual em: ${VENV_DIR}"
python3 -m venv "${VENV_DIR}"

# shellcheck disable=SC1090
source "${VENV_DIR}/bin/activate"

echo "Atualizando ferramentas do pip..."
python -m pip install --upgrade pip setuptools wheel

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REQ_FILE="${SCRIPT_DIR}/requirements.txt"

if [[ ! -f "${REQ_FILE}" ]]; then
    echo "Erro: requirements.txt não encontrado em ${SCRIPT_DIR}"
    exit 1
fi

echo "Instalando dependências de ${REQ_FILE}..."
pip install -r "${REQ_FILE}"

echo
echo "Ambiente pronto."
echo "Para ativar depois, use:"
echo "  source ${VENV_DIR}/bin/activate"
echo
echo "Teste rápido:"
echo "  python -c 'import serial; print(serial.__version__)'"
