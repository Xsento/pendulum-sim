# Pendulum Sim
Simulates the Driven, Damped Pendulum and the Double Pendulum to illustrate their chaotic behaviour and the impact of small changes in starting parameters on the course of the simulation.

## Key Features  
- C++17 backend with object-oriented design
- Modern OpenGL core profile rendering pipeline
- Fully interactive Dear ImGui user interface
- Cross-platform build configuration via CMake (Windows and Linux)

## Physics background
This project simulates the chaotic motion of a Double Pendulum in real-time by solving its non-linear equations of motion derived via Lagrangian mechanics.
Because these coupled, second-order differential equations cannot be solved analytically, the simulation implements the Runge-Kutta 4th Order (RK4) numerical integration method to prevent numerical energy drift and ensure physical stability. Additionally, the engine supports a driven, damped single pendulum, modeling external motor force and friction to demonstrate transitions from periodic behavior to chaotic attractors.

## Controls
| Function | Key |
|---|---|
| **Exit** | `Escape` |
| **Toggle GUI** | `H` |

## Screenshots
Driven, Damped Pendulum
<img width="1919" height="1000" alt="image" src="https://github.com/user-attachments/assets/ad11ea18-a49d-4921-bc8f-884a51f18e55" />
<br>
Double Pendulum
<img width="1920" height="1009" alt="image" src="https://github.com/user-attachments/assets/488f4e37-fa16-41c3-b1ed-222ea1a41a96" />

## Dependencies
- CMake `3.28.3+`
- C++ `17`
- OpenGL `3.3+`
- GLM  `included in the repository`
- GLFW3 `pre installed for Windows`
- GLAD `included in the repository`
- ImGUI `included in the repository`

## GLFW3 Installation
Debian/Ubuntu
```bash
sudo apt update
sudo apt install libglfw3 libglfw3-dev
```

## Compilation and Execution
Linux/Windows
```
mkdir build
cmake -S . -B build 
cmake --build build
cd build
```
Linux
`./pendulum-sim` <br>
Windows
`.\pendulum-sim.exe`
