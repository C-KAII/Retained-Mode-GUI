# **Retained-Mode-GUI in C++**
### **Overview**
This project is a **basic Retained Mode GUI (RMGUI) implementation** using **SDL2**. It was built to help me:
- **Debug my C++ games more effectively**  
- **Learn more about GUI development**  
- **Experiment with layout management and input handling**  

At this stage, it provides:
- **Buttons, Sliders, and Text Fields**  
- **A basic layout manager (Vertical, Horizontal, Grid)**  
- **Input handling for mouse & keyboard interactions**  

---

## **Installation & Setup**
### **Prerequisites**
Make sure you have the following installed:
- **Visual Studio 2022** (other version are fine, project setup instructions will differ)  
- **SDL2 Development Libraries**  
- **SDL2_ttf Development Libraries**  

---

## **Building in Visual Studio**
### **Step 1: Install SDL2 and SDL2_ttf**
1. Download **SDL2** and **SDL2_ttf** from:  
   - [SDL2](https://github.com/libsdl-org/SDL/releases)  
   - [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf/releases)  

2. Extract the files and place them in a known directory (e.g., `C:\Libraries\SDL2`).

3. In **Visual Studio**, go to **Project -> Properties** and configure:
   - **C/C++ -> General -> Additional Include Directories**:
     ```
     C:\Libraries\SDL2\include
     ```
   - **Linker -> General -> Additional Library Directories**:
     ```
     C:\Libraries\SDL2\lib
     ```
   - **Linker -> Input -> Additional Dependencies**:
     ```
     SDL2.lib; SDL2main.lib; SDL2_ttf.lib;
     ```

4. Copy **SDL2.dll** and **SDL2_ttf.dll** to your project's output directory (e.g., `x64/Debug` or `x64/Release`).

---

### **Step 2: Open the Project in Visual Studio**
1. Open **Visual Studio 2022**.
2. Click **File -> Open -> Project/Solution (or CMake)**.
3. Make sure the correct **x64** architecture is selected.
4. Click **Build -> Build Solution (Ctrl + Shift + B / F7)**.

---

### **Step 3: Run the Project**
1. Press **F5** to start debugging, or
2. Press **Ctrl + F5** to run without debugging.

If you see a **black window**, it means SDL2 is working correctly!

---

## **Features**
- **Immediate Mode UI (IMGUI)**
- **Dynamic Layout System** (Grid, Vertical, Horizontal)
- **Custom Widgets**:
  - Buttons (Clickable, event-driven)
  - Sliders (Adjustable values)
  - Text Fields (Keyboard input support)
- **Simple SDL2-based rendering**
- **Debugging Tools** (Color-coded elements, Console output)

---

## **Usage**
Once compiled, simply **run the executable** in Visual Studio or navigate to the output folder and run:
```sh
imgui_app.exe
```

### **Controls**
| Key / Action | Function |
|-------------|----------|
| **Mouse Click** | Press buttons, interact with sliders |
| **Arrow Keys** | Adjust sliders, move text cursor |
| **Shift / Ctrl** | Enables shift / ctrl key modifications |
| **Tab** | Tab selection through widgets |
| **Enter** | Simulate mouse click on tab selected items |
| **Escape** | Exit program |

---

## **Future Plans**
- **Improve Layout Manager (Auto-resizing, dynamic updates)**
- **Better UI event handling (Drag & Drop, Tab Navigation)**
- **Add resizable windows and panels**
- **Enhance rendering (Theming, Borders, Shadows)**

---

## **Author**
**Kobi Chambers**

Check out my website - https://www.kobichambers.com/

Feel free to fork and contribute!

---

## **License**
This project is **open-source** and available under the **MIT License**.

---