#include "engine.h"
#include <cmath>


const color WHITE(1, 1, 1);


Engine::Engine() {
    this->initWindow();
    this->initShaders();
    this->initShapes();
}

Engine::~Engine() {}


unsigned int Engine::initWindow(bool debug) {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(WIDTH, HEIGHT, "engine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // OpenGL configuration
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    return 0;
}

void Engine::initShaders() {
    // load shader manager
    shaderManager = make_unique<ShaderManager>();

    // Load shader into shader manager and retrieve it
    shapeShader = this->shaderManager->loadShader("../res/shaders/shape.vert", "../res/shaders/shape.frag",  nullptr, "shape");

    // Set uniforms
    shapeShader.use();
    shapeShader.setMatrix4("projection", this->PROJECTION);
}

void Engine::initShapes() {

    int hop_size = 128;
    int samplerate = 8000;
    uint_t hopSize = 256;  // Adjust as needed
    fvec_t *buffer = new_fvec(hopSize / 2);
    aubio_source_t *source = new_aubio_source(
            "/Users/carolrooney/CLionProjects/Final-Project-Cmrooney/res/music_wav/06 songs for women.wav", samplerate,
            hop_size);
    if (!source) {
        std::cerr << "Error opening the audio file." << std::endl;
    }


    cout << "aubio working" << endl;
    float barWidth = 0.0f;  // Use float for positioning accuracy
    float barSpacing = 0.1f;  // Adjust spacing between bars
    do {
        aubio_source_do(source, buffer, &hopSize);
        for (int i = 0; i < hopSize / 2; ++i) {
            if (i % 400 == 0) {
                float amplitude = buffer->data[i];
                float pitch = buffer->data[i];
                if (amplitude !=0) {
                    float barHeight = fabs((amplitude * HEIGHT));
                    vec2 barSize = {5.0f, barHeight};
                    float color = pitch;
                    float red = pitch;
                    float blue = (1.0f - pitch);
                    struct color COLOR(red, 0, blue);
                    soundbar.push_back(std::make_unique<Rect>(
                            shapeShader, vec2(barWidth, HEIGHT / 2), barSize, COLOR));
                    barWidth += barSize.x + barSpacing;
                }
            }
        }
    } while (hopSize != 0);
    // Cleanup aubio
    aubio_cleanup();
    cout << "aubio cleaned up" << endl;


}




void Engine::processInput() {
    glfwPollEvents();

    // Set keys to true if pressed, false if released
//    for (int key = 0; key < 1024; ++key) {
//        if (glfwGetKey(window, key) == GLFW_PRESS)
//            keys[key] = true;
//        else if (glfwGetKey(window, key) == GLFW_RELEASE)
//            keys[key] = false;
//    }
//
//    // Close window if escape key is pressed
//    if (keys[GLFW_KEY_ESCAPE])
//        glfwSetWindowShouldClose(window, true);
//
//    // Calculate delta time
//    float currentFrame = glfwGetTime();
//    deltaTime = currentFrame - lastFrame;
//    lastFrame = currentFrame;
//
//    // Put this code at the end of processInput in engine.cpp
//    float speed = 200.0f * deltaTime;
//    if (keys[GLFW_KEY_UP]) soundbar[0]->moveY(speed);
//    if (keys[GLFW_KEY_DOWN]) soundbar[0]->moveY(-speed);
//    if (keys[GLFW_KEY_LEFT]) soundbar[0]->moveX(-speed);
//    if (keys[GLFW_KEY_RIGHT]) soundbar[0]->moveX(speed);
}

void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

}
void Engine::render() {
    //glClearColor(BLACK.red, BLACK.green, BLACK.blue, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);

    // Set shader to use for all shapes
    shapeShader.use();

    for (const unique_ptr<Rect> &s : soundbar) {
        s->draw();
        s->setUniforms();
    }
    glfwSwapBuffers(window);
}

bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}