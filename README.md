# Smartwatch 2D Simulation — OpenGL Project

A fully interactive **2D smartwatch simulation** built in **C++** using **OpenGL 3.3 Core Profile**.  
The application demonstrates shader-based rendering, animations, text drawing, custom cursor usage, and synchronized multi-screen UI logic.

Developed as part of the **Computer Graphics** course at the **Faculty of Technical Sciences, University of Novi Sad (FTN)**.

---

## 🚀 About the Project

This project simulates a smartwatch interface with three functional screens:

- **Time Screen** — real-time digital clock  
- **BPM (Heart Rate) Screen** — animated EKG waveform + BPM simulation  
- **Battery Screen** — battery discharge simulation  

The application runs in fullscreen, maintains a steady **75 FPS**, and keeps all screen states synchronized.

---

## ✨ Features

### 🕒 Time Screen
- Live digital clock (`HH:MM:SS`)  
- Automatic rollover of seconds, minutes, and hours  
- Smooth text rendering via **FreeType**  
- Right arrow navigates to BPM screen  

---

### ❤️ BPM Screen
- Animated scrolling EKG waveform  
- Resting BPM fluctuates between **60–80**  
- **Running mode** (hold **D**) gradually increases BPM  
- EKG waveform becomes denser as BPM increases  
- **Critical alert** overlay when BPM exceeds 200  
- Left/Right arrows handle screen navigation  

---

### 🔋 Battery Screen
- Battery fills and drains dynamically  
- Battery drains **1% every 10 seconds**  
- Color-coded battery levels:
  - 🟩 Green: >20%
  - 🟨 Yellow: 20–10%
  - 🟥 Red: ≤10%
- Battery reaches 0% → application closes  
- Percentage displayed above the battery icon using FreeType  

---

## 🎨 Rendering & System Overview

The project uses a complete modern OpenGL rendering pipeline:

- OpenGL **3.3 Core Profile** (no fixed pipeline)  
- Custom **vertex & fragment shaders**  
- **Uniforms** for transformations, colors, textures, scaling  
- Full **FreeType text rendering system**  
- Texture loading via **stb_image**  
- Alpha blending for transparency  
- Heart-shaped **custom cursor**  
- **75 FPS frame limiter**  
- Modular screen system with persistent shared state  

---

## 🛠️ Built With

- **C++17**
- **OpenGL 3.3 Core**
- **GLFW**
- **GLEW**
- **FreeType**
- **stb_image**
- Visual Studio 2022 (recommended)

---

## 🧭 Controls

| Action | Input |
|--------|--------|
| Switch screens | Left mouse click on arrows |
| Simulate running | Hold **D** |
| Exit application | **ESC** |
| Cursor | Heart-shaped custom cursor |

---

## 👤 Author

**Katarina Zgonjanin**  
Faculty of Technical Sciences (FTN), University of Novi Sad  
Computer Graphics, 2025  


