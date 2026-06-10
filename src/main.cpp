#include "main.hpp"
#include "shader.h"
#include "pendulum.hpp"
#include "doublePendulum.hpp"

// definitions for globals declared in main.hpp
bool showGUI = true;
bool H_pressed = false;
float deltaTime = 0;

enum PENDULUM_TYPE{
    SINGLE,
    DOUBLE,
    NONE
};

PENDULUM_TYPE pendulumType = PENDULUM_TYPE::NONE;

void renderGUI(GLFWwindow*& window, Shader& shaderProgram, PENDULUM_TYPE& pendulumType, std::vector<Pendulum>& SPendulumVec, std::vector<DoublePendulum>& DPendulumVec);

glm::vec3 hsv2rgb(float h, float s, float v);

int main()
{
    // glfw: initialize and configure
    std::cout << "Initializing GLFW..." << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    std::cout << "GLFW initialized properly" << std::endl;

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Please wait...", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // set up Dear ImGui context
    std::cout << "Initializing Dear ImGui..." << std::endl;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init("#version 330 core");
    std::cout << "Dear ImGui initialized properly" << std::endl;

    // build and compile the shader program
    std::cout << "Initializing shader program..." << std::endl;
    Shader shaderProgram("./assets/shaders/vertex.vs", "./assets/shaders/fragment.fs");
    std::cout << "Shader program initialized" << std::endl;

    shaderProgram.use();

    // OpenGL matrices
    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);

    model = glm::scale(model, glm::vec3(0.5f));

    view = glm::lookAt(
        glm::vec3(0.0f, 0.f, 3.3f),
        glm::vec3(0.0f), 
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    shaderProgram.setMat4("view", view);        
    shaderProgram.setMat4("projection", projection);
    shaderProgram.setMat4("model", model);


    float lastTime = glfwGetTime();
    float lastFPSupdate = lastTime;


    std::vector<DoublePendulum> DPendulumVec;
    std::vector<Pendulum> SPendulumVec;

    DoublePendulum dp(2., 2., 0., 0., {1.f, 1.f, 1.f});
    std::vector<DoublePendulum> dps;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // delta time calculations
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if (currentTime - lastFPSupdate > 1.f){
            glfwSetWindowTitle(window, (std::string("FPS: ") + std::to_string(static_cast<int>(1/deltaTime))).c_str()); 
            lastFPSupdate = currentTime;
        }

        // input    
        processInput(window);

        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // prevent distortion by setting the aspect ratio based on the framebuffer size, not the window size
        int currentWidth = SCR_WIDTH;
        int currentHeight = SCR_HEIGHT;
        glfwGetFramebufferSize(window, &currentWidth, &currentHeight);
        projection = glm::perspective(glm::radians(45.0f), static_cast<float>(currentWidth) / static_cast<float>(currentHeight), 0.1f, 100.0f);
        shaderProgram.setMat4("projection", projection);

        // render pendulums
        for (auto& SP : SPendulumVec){
            SP.updateAndDraw(shaderProgram,currentTime,deltaTime);
        }
        for (auto& DP : DPendulumVec){
            DP.updateAndDraw(shaderProgram,currentTime,deltaTime);
        }

        if (showGUI)
            renderGUI(window, shaderProgram, pendulumType, SPendulumVec, DPendulumVec);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 
    std::cout << "Window closed. Exiting..." << std::endl;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void generateDoublePendulums(double startingAngle1, double startingAngle2, double startingVelocity1, double startingVelocity2, double amount, double delta, std::vector<DoublePendulum>& vec){
    for (int i = 0; i < amount; i++){
        float hue = (static_cast<float>(i) / amount) * 360.0f; 
        glm::vec3 color = hsv2rgb(hue, 1.0f, 1.0f);
        vec.push_back(DoublePendulum(
            startingAngle1 + (i * delta), 
            startingAngle2 + (i * delta), 
            startingVelocity1, startingVelocity2, 
            color
        ));
    }
}

void generateSinglePendulums(double startingAngle, double startingVelocity, double amount, double delta, std::vector<Pendulum>& vec){
    for (int i = 0; i < static_cast<int>(amount); i++){
        float hue = (static_cast<float>(i) / amount) * 360.0f; 
        glm::vec3 color = hsv2rgb(hue, 1.0f, 1.0f);
        vec.emplace_back(Pendulum(
            startingAngle + (i * delta), 
            startingVelocity, 
            color
        ));
    }
}

void renderGUI(GLFWwindow*& window, Shader& shaderProgram, PENDULUM_TYPE& pendulumType, std::vector<Pendulum>& SPendulumVec, std::vector<DoublePendulum>& DPendulumVec){
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize 
                            | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;    

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos({0,0}, ImGuiCond_FirstUseEver);

    if (pendulumType == PENDULUM_TYPE::NONE){
        ImGui::Begin("Main menu", &showGUI, flags);
        if (ImGui::Button("Driven/Damped Pendulum", ImVec2(300, 30))){
            pendulumType = PENDULUM_TYPE::SINGLE;
        }
        if (ImGui::Button("Double Pendulum", ImVec2(300, 30))){
            pendulumType = PENDULUM_TYPE::DOUBLE;
        } 
        ImGui::Text("Press [H] to toggle GUI");
        ImGui::Text("Press [ESC] to exit");
    }
    else if (pendulumType == PENDULUM_TYPE::SINGLE){
        ImGui::Begin("Driven/Damped Pendulum", &showGUI, flags);
        static float angle = M_PI;
        static double velocity = 0;
        static unsigned int amount = 100;

        ImGui::Text("Amount of pendulums:");
        ImGui::InputScalar("##", ImGuiDataType_U32, &amount, nullptr, nullptr, "%u", ImGuiInputTextFlags_None);
        ImGui::Text("Starting angle:");
        ImGui::SliderAngle("##2", &angle, 0, 360);
        ImGui::Text("Starting velocity:");
        ImGui::InputScalar("##1", ImGuiDataType_Double, &velocity, nullptr, nullptr, "%lf", ImGuiInputTextFlags_None);
        ImGui::Separator();

        if (!SPendulumVec.empty()){
            double minF = 0;
            double maxF = 5;
            double minO = 0;
            double maxO = 1;
            ImGui::Text("Gravity:");
            ImGui::InputScalar("##6", ImGuiDataType_Double, &g, nullptr, nullptr, "%lf", ImGuiInputTextFlags_None);
            ImGui::Text("Combined mass and air resitance factor:");
            ImGui::SliderScalar("##5", ImGuiDataType_Double, &q, &minF, &maxF, "%lf", ImGuiInputTextFlags_None);
            ImGui::Text("Driving force value:");
            ImGui::SliderScalar("##3", ImGuiDataType_Double, &F_drive, &minF, &maxF, "%lf", ImGuiInputTextFlags_None);
            ImGui::Text("Driving force frequency:");
            ImGui::SliderScalar("##4", ImGuiDataType_Double, &Omega, &minO, &maxO, "%lf", ImGuiInputTextFlags_None);
            ImGui::Separator();
            ImGui::Text("Current driving force: %lf", F_drive * sin(Omega * glfwGetTime()));
        }
        else{
            g = 9.81;     
            q = 0.5;         
            F_drive = 1.2;   
            Omega = 0.666;   
        }

        if (ImGui::Button("Start the simulation", ImVec2(-1, 30))){
            SPendulumVec.clear();
            DPendulumVec.clear();
            generateSinglePendulums(angle, velocity, amount, 0.0001, SPendulumVec);
        } 

        if (ImGui::Button("Return to main menu", ImVec2(-1, 30))){
            pendulumType = PENDULUM_TYPE::NONE;
        } 
    }
    else{
        ImGui::Begin("Double Pendulum", &showGUI, flags);
        static float angle1 = M_PI;
        static float angle2 = M_PI;
        static double velocity1 = 0;
        static double velocity2 = 0;
        static unsigned int amount = 1000;

        ImGui::Text("Amount of pendulums:");
        ImGui::InputScalar("##", ImGuiDataType_U32, &amount, nullptr, nullptr, "%u", ImGuiInputTextFlags_None);
        ImGui::Text("Starting angle 1:");
        ImGui::SliderAngle("##1", &angle1, 0, 360);
        ImGui::Text("Starting angle 2:");
        ImGui::SliderAngle("##2", &angle2, 0, 360);
        ImGui::Text("Starting velocity 1:");
        ImGui::InputScalar("##3", ImGuiDataType_Double, &velocity1, nullptr, nullptr, "%lf", ImGuiInputTextFlags_None);
        ImGui::Text("Starting velocity 2:");
        ImGui::InputScalar("##4", ImGuiDataType_Double, &velocity2, nullptr, nullptr, "%lf", ImGuiInputTextFlags_None);
        ImGui::Separator();

        if(!DPendulumVec.empty()){
            ImGui::Text("Gravity:");
            ImGui::InputScalar("##5", ImGuiDataType_Double, &g, nullptr, nullptr, "%lf", ImGuiInputTextFlags_None);
        }
        else{
            g = 9.81;
        }

        if (ImGui::Button("Start the simulation", ImVec2(-1, 30))){
            SPendulumVec.clear();
            DPendulumVec.clear();
            generateDoublePendulums(angle1, angle2, velocity1, velocity2, amount, 0.00001, DPendulumVec);
        } 

        if (ImGui::Button("Return to main menu", ImVec2(-1, 30))){
            pendulumType = PENDULUM_TYPE::NONE;
        } 
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

glm::vec3 hsv2rgb(float h, float s, float v) {
    float h_prime = h / 60.0f;
    int i = static_cast<int>(h_prime);
    float f = h_prime - i;
    
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));
    
    switch (i % 6) {
        case 0: return glm::vec3(v, t, p);
        case 1: return glm::vec3(q, v, p);
        case 2: return glm::vec3(p, v, t);
        case 3: return glm::vec3(p, q, v);
        case 4: return glm::vec3(t, p, v);
        case 5: return glm::vec3(v, p, q);
        default: return glm::vec3(0.0f);
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS){
        if (!H_pressed){
            showGUI = !showGUI;
            H_pressed = true;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_RELEASE){
        if (H_pressed){
            H_pressed = false;
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

