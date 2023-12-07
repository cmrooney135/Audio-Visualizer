#include "engine.h"
#include <cmath>



enum state {choose, wave};
state screen;


Engine::Engine() {
    this->initWindow();
    this->initShaders();
    //this->initShapes();
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

    // Configure text shader and renderer
    textShader = shaderManager->loadShader("../res/shaders/text.vert", "../res/shaders/text.frag", nullptr, "text");
    fontRenderer = make_unique<FontRenderer>(shaderManager->getShader("text"), "../res/fonts/MxPlus_IBM_BIOS.ttf", 24);

    // Set uniforms
    textShader.use().setVector2f("vertex", vec4(100, 100, .5, .5));
    shapeShader.use().setMatrix4("projection", this->PROJECTION);
}

void Engine::initShapes(string songChoice) {
    int hop_size = 64;
    int samplerate = 8000;
    uint_t sample_rate = 8000;
    uint_t hopSize = 128;  // Adjust as needed
    fvec_t *buffer = new_fvec(hopSize / 2);
    cvec_t *spectrum = new_cvec(512);
    aubio_pitch_t *o = new_aubio_pitch("yinfast", 256, 128, samplerate);
    aubio_tempo_t* tempo = new_aubio_tempo("default", 256, 128, samplerate);
    fvec_t *pitchvec = new_fvec(hopSize/2);
    fvec_t *tempoVec = new_fvec(hopSize/2);
    uint_t read = 0;
    float bpm = 0;
    float timeElapsed = 0.0f;
    float pulseSpeed = bpm / 60.0f;
    float yScale =1;
    screen = choose;
    const char *choice = songChoice.c_str();
    aubio_source_t *source = new_aubio_source(choice, sample_rate, hopSize);
    if (!source) {
        std::cerr << "Error opening the audio file." << std::endl;
    }
    cout << "aubio working" << endl;
    float barWidth = 0.0f;  // Use float for positioning accuracy
    float barSpacing = 0.1f;  // Adjust spacing between bars
    do {
        fvec_t* inputBuffer = new_fvec(hopSize);
        aubio_source_do(source, buffer, &hopSize);
        aubio_pitch_do(o, buffer, pitchvec);
        aubio_tempo_do(tempo, inputBuffer, tempoVec);
        for (int i = 0; i < hopSize / 2; ++i) {
            if (i % 150 == 0) {
                float amplitude = buffer->data[i];
                float pitch = pitchvec->data[i];
                if ((amplitude != 0) && (pitch != 0)) {
                    float barHeight = fabs((amplitude * HEIGHT));
                    vec2 barSize = {5.0f, barHeight};
                    pitch = pitch / 1000;
                    float red = (pitch * 5);
                    float blue = (1.0 - pitch);
                    struct color COLOR(red, 0.5, blue);
                    soundbar.push_back(std::make_unique<Rect>(
                            shapeShader, vec2(barWidth, HEIGHT / 2), barSize, COLOR));
                    barWidth += barSize.x + barSpacing;

                }
                del_fvec(inputBuffer);
            }
        }
    } while (hopSize != 0);
    del_aubio_source(source);
    del_fvec(buffer);
    del_cvec(spectrum);
    del_aubio_pitch(o);
    del_fvec(pitchvec);
    del_fvec(tempoVec);


    del_aubio_tempo(tempo);
    // Cleanup aubio
    aubio_cleanup();
    cout << "aubio cleaned up" << endl;
}
void Engine::processInput() {
    glfwPollEvents();

    //Set keys to true if pressed, false if released
    for (int key = 0; key < 1024; ++key) {
        if (glfwGetKey(window, key) == GLFW_PRESS)
            keys[key] = true;
        else if (glfwGetKey(window, key) == GLFW_RELEASE)
            keys[key] = false;
    }

    // Close window if escape key is pressed
    if (keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, true);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (screen == choose) {
        string songChoice;


            if (keys[GLFW_KEY_1]) {
                songChoice = "/Users/carolrooney/CLionProjects/Final-Project-Cmrooney/res/music_wav/03 novacane.wav";
                initShapes(songChoice);
                screen = wave;
            }
            if (keys[GLFW_KEY_2]) {
                songChoice = "/Users/carolrooney/CLionProjects/Final-Project-Cmrooney/res/music_wav/02 strawberry swing.wav";
                initShapes(songChoice);
                screen = wave;
            }
            if (keys[GLFW_KEY_3]) {
                songChoice = "/Users/carolrooney/CLionProjects/Final-Project-Cmrooney/res/music_wav/07 - Day Away [Prod Brain Kennedy].wav";
                initShapes(songChoice);
                screen = wave;
            }
            if (keys[GLFW_KEY_4]) {
                songChoice = "/Users/carolrooney/CLionProjects/Final-Project-Cmrooney/res/music_wav/14 nature feels.wav";
                initShapes(songChoice);
                screen = wave;
            }
            if (keys[GLFW_KEY_5]) {
                songChoice = "/Users/carolrooney/CLionProjects/Final-Project-Cmrooney/res/music_wav/64 - When Im Done (Prod Midi Mafia).wav";
                initShapes(songChoice);
                screen = wave;
            }
        }
    }

void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

}
void Engine::render() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    textShader.use();
    shapeShader.use();
    switch (screen) {
        case choose: {
            string message1 = "Choose one of My Top 5 songs";
            string message2 = "to view the waveform";
            string message3 = "press the number!!!";
            this->fontRenderer->renderText(message1, 0,700, 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(message2, 0,(700 - message1.size()-12), 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(message3, 0,(700 - message1.size()-24), 1, vec3{1, 1, 1});
            string choice1 = "1. Novicane";
            string choice2 = "2. Strawberry Swing";
            string choice3 = "3. Day Away";
            string choice4 = "4. Nature Feels";
            string choice5 = "5. When I'm Done";
            this->fontRenderer->renderText(choice1, 0, 500, 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(choice2, 0, (500- choice1.size()-24), 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(choice3, 0, ((500- choice1.size()-choice2.size())-48), 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(choice4, 0, ((500- choice1.size()-choice2.size()-choice3.size())-72), 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(choice5, 0, ((500- choice1.size()-choice2.size()-choice3.size()-choice4.size())-96), 1, vec3{1, 1, 1});



            break;
        }
        case wave: {
            for (const unique_ptr<Rect> &s : soundbar) {
                s->draw();
                s->setUniforms();
            }

        }

    }

    glfwSwapBuffers(window);
    // Set shader to use for all shapes
    shapeShader.use();
    //this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/2, 1, vec3{1, 1, 1});

}

bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}