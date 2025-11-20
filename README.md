# Snake Multiplayer BLE en ESP32 + TFT

Juego Snake para dos jugadores en ESP32, controlado por Bluetooth desde smartphones y visualizado en pantalla TFT 3.5". Soporta música de fondo (DFPlayer Mini) y efectos de sonido (DAC interno).

---

## 📦 Librerías necesarias

Instala desde el Gestor de Librerías de Arduino IDE:
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) (by Bodmer)
- [DFRobotDFPlayerMini](https://github.com/DFRobot/DFRobotDFPlayerMini)
- **BLEDevice** (nativa en ESP32 Arduino Core)

---

## 🛠️ Hardware requerido

- ESP32 DevKit V1
- Módulo TFT LCD 3.5” SPI (ILI9488/ILI9486)
- DFPlayer Mini + MicroSD (con archivo `0001.mp3`)
- Altavoz 8Ω (DFPlayer, música fondo)
- Altavoz piezo o bocina pequeña (GPIO 25, efectos)
- 2 smartphones Android/iOS (control BLE)
- Protoboard y cables
- Fuente 5V/2A (o power bank USB)

**Conexiones principales:**

| Módulo       | ESP32 Pin |
|--------------|-----------|
| TFT_MOSI     | 23        |
| TFT_SCK      | 18        |
| TFT_CS       | 15        |
| TFT_DC       | 2         |
| TFT_RST      | 4         |
| DFPlayer RX  | 17        |
| DFPlayer TX  | 16        |
| DAC (SFX)    | 25        |
| Alimentación | 5V, GND   |

---

## 🚀 Instrucciones rápidas

1. **Descarga y ajusta**:  
   - Todos los archivos `.h` y el `.ino` en la misma carpeta.
   - Configura los pines SPI en `User_Setup.h` de TFT_eSPI según la tabla de arriba.

2. **Carga el código** en el ESP32 usando Arduino IDE.

3. **Copia `0001.mp3`** (música de fondo) a la raíz de tu microSD e insértala en el DFPlayer.

4. **Conecta el hardware** como en el diagrama/conexión anterior.

5. **Abre la app** de control BLE en dos smartphones (puede ser una app propia, o una app de test BLE) y conecta ambos al ESP32.

6. **Juega!**  
   - Usa los smartphones para controlar las serpientes.
   - El juego y la música comenzarán automáticamente.

---

## 📝 Notas

- Música de fondo: solo archivo `0001.mp3` en la SD.
- Efectos de sonido: generados por DAC, sin interferir con la música.
- El código es modular y fácil de adaptar para otros proyectos.

---

## Créditos

Desarrollado por Equipo de Desarrollo embebido 2025.
