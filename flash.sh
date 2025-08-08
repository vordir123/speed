#!/usr/bin/env bash
# Automated build and flash script for ESP32 and Teensy firmware
# Usage: ./flash.sh [--install|--compile|--upload|--help]
set -e
LOGFILE="flash.log"

function log() {
  echo "[$(date '+%H:%M:%S')] $1"
}

function ensure_platformio() {
  if ! command -v platformio >/dev/null 2>&1; then
    log "Installing PlatformIO..."
    python3 -m pip install --user -U platformio
    export PATH="$HOME/.local/bin:$PATH"
  fi
}

function compile_all() {
  log "Compiling ESP32 firmware" && platformio run -d firmware/esp32
  log "Compiling Teensy firmware" && platformio run -d firmware/teensy
}

function upload_all() {
  log "Uploading ESP32" && platformio run -d firmware/esp32 -t upload
  log "Uploading Teensy" && platformio run -d firmware/teensy -t upload
}

case "$1" in
  --install)
    ensure_platformio ;;
  --compile)
    ensure_platformio
    compile_all ;;
  --upload)
    ensure_platformio
    compile_all
    upload_all ;;
  --help|*)
    echo "Usage: $0 [--install|--compile|--upload|--help]" ;;
esac
