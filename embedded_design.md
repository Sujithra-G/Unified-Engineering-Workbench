# Embedded integration design (simulation only)

```text
                 HOST PC (Windows 11)
                 USB host / application
                         |
          +--------------+---------------+
          |              |               |
       USB/UART       USB vendor       USB-to-JTAG
          |              |               |
   Microcontroller      FPGA         JTAG adapter
   telemetry/control  accelerator    debug/programming
          |              |               |
     GPIO / ADC       FPGA I/O       TCK/TMS/TDI/TDO
```

| Interface | Power / ground | Signal domain | Connector / host | Ownership / assumptions |
|---|---|---|---|---|
| MCU | Board supply per exact board datasheet; common ground | Often 3.3 V, but verify board | USB; UART/GPIO | MCU owns its GPIO/UART pins |
| FPGA | Dedicated regulated rails per board guide; common ground | Bank-specific VCCIO, may vary by bank | USB/JTAG or vendor link | FPGA pinout and bank voltage are board-specific |
| JTAG adapter | Usually host-powered; confirm adapter | Must match target JTAG I/O voltage | USB to host; target JTAG header | Adapter drives TCK/TMS/TDI and samples TDO |

Never connect signal pins until the exact board manuals, connector pinouts and voltage domains are checked. Use a level shifter when logic levels are incompatible; do not assume 5 V tolerance. Establish a common signal reference ground where required. Current-limited power and ESD precautions are recommended.

A JTAG adapter is a debug/programming transport. It does not provide interchangeable general-purpose compute resources like an MCU or FPGA. This implementation uses mock adapters only; no physical board, firmware, FPGA bitstream or JTAG transaction is implemented.
