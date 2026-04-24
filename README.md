# Snake Multiplayer BLE en ESP32 + TFT

Juego Snake para dos jugadores en ESP32, controlado por Bluetooth desde smartphones y visualizado en pantalla TFT 3.5". Soporta música de fondo (DFPlayer Mini) y efectos de sonido (DAC interno).

---

## Librerías necesarias

Instala desde el Gestor de Librerías de Arduino IDE:
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) (by Bodmer) (modifica el archivo User_Setup.h)
- [DFRobotDFPlayerMini](https://github.com/DFRobot/DFRobotDFPlayerMini)
- **BLEDevice** (nativa en ESP32 Arduino Core)

---

## Hardware requerido

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
| TFT_MISO     | 19        |
| TFT_MOSI     | 23        |
| TFT_SCK      | 18        |
| TFT_CS       | 15        |
| TFT_DC       | 2         |
| TFT_RST      | 4         |
| LED          | 3.3V      |
| DFPlayer RX  | 17        |
| DFPlayer TX  | 16        |
| DAC (SFX)    | 25        |
| Alimentación | 3.3V, GND |

<img width="796" height="585" alt="esquematico" src="https://github.com/user-attachments/assets/6e2a8621-c4f2-4617-bf86-6170397fe378" />

---

## Instrucciones rápidas

1. **Descarga y ajusta**:  
   - Todos los archivos `.h` y el `.ino` en la misma carpeta.
   - Reemplaza completamente el archivo `User_Setup.h` ubicado en la librería de TFT_eSPI.
```
#define USER_SETUP_INFO "User_Setup"

#define ILI9488_DRIVER

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS 15
#define TFT_DC 2
#define TFT_RST 4

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF

#define SMOOTH_FONT

#define SPI_FREQUENCY 27000000
#define SPI_READ_FREQUENCY 20000000
#define SPI_TOUCH_FREQUENCY 2500000

```

2. **Carga el código** en el ESP32 usando Arduino IDE.

3. **Copia `0001.mp3`** (música de fondo) a la raíz de tu microSD e insértala en el DFPlayer.

4. **Conecta el hardware** como en el diagrama/conexión anterior.

5. **Abre la app** de control BLE en dos smartphones (puede ser una app propia, o una app de test BLE) y conecta ambos al ESP32.

6. **Juega!**  
   - Usa los smartphones para controlar las serpientes.
   - El juego y la música comenzarán automáticamente.

---

## Notas

- Música de fondo: solo archivo `0001.mp3` en la SD.
- Efectos de sonido: generados por DAC, sin interferir con la música.
- El código es modular y fácil de adaptar para otros proyectos.
- [![Mira el video en youtube](<img width="344" height="601" alt="IMG" src="https://github.com/user-attachments/assets/833237ec-fc28-4728-897f-eed43726412f" />
)](https://www.youtube.com/shorts/wCvCZMIIGcU)
---
Licencia MIT
