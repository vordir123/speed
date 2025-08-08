const esp32Pins = [0,1,2,3,4,5]; // indexes for 6 pins
const teensyPins = [0,1,2,3];
let serialPort, reader, writer;

function createPinControls(container, pins, onToggle) {
  container.innerHTML = '';
  pins.forEach(id => {
    const btn = document.createElement('button');
    btn.textContent = `Pin ${id}: OFF`;
    btn.dataset.state = '0';
    btn.addEventListener('click', () => {
      const state = btn.dataset.state === '0' ? 1 : 0;
      onToggle(id, state, () => {
        btn.dataset.state = state.toString();
        btn.textContent = `Pin ${id}: ${state ? 'ON' : 'OFF'}`;
      });
    });
    container.appendChild(btn);
  });
}

async function toggleEsp32Pin(id, state, cb) {
  const ip = document.getElementById('esp32-ip').value || location.hostname;
  try {
    await fetch(`http://${ip}/api/pin?id=${id}&state=${state}`);
    cb();
  } catch(err) { console.error(err); }
}

function toggleTeensyPin(id, state, cb) {
  if (!writer) return;
  writer.write(`PIN,${id},${state}\n`);
  cb();
}

async function connectSerial() {
  serialPort = await navigator.serial.requestPort();
  await serialPort.open({ baudRate: 115200 });
  writer = serialPort.writable.getWriter();
  reader = serialPort.readable.getReader();
  readLoop();
}

async function readLoop() {
  const log = document.getElementById('serial-log');
  while (reader) {
    const { value, done } = await reader.read();
    if (done) break;
    log.textContent += new TextDecoder().decode(value);
    log.scrollTop = log.scrollHeight;
  }
}

function setupTabs() {
  document.querySelectorAll('nav button[data-tab]').forEach(btn => {
    btn.addEventListener('click', () => {
      document.querySelectorAll('.tab').forEach(s => s.classList.add('hidden'));
      document.getElementById(btn.dataset.tab).classList.remove('hidden');
    });
  });
}

function setup() {
  createPinControls(document.getElementById('esp32-pins'), esp32Pins, toggleEsp32Pin);
  createPinControls(document.getElementById('teensy-pins'), teensyPins, toggleTeensyPin);
  document.getElementById('serial-connect').addEventListener('click', connectSerial);
  document.getElementById('request-analysis').addEventListener('click', () => {
    if (!writer) return;
    // Example request: ANALYZE,100,0 -> id=0x100, first byte 0
    writer.write('ANALYZE,256,0\n');
  });
  setupTabs();
}

document.addEventListener('DOMContentLoaded', setup);
