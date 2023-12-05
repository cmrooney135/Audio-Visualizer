#include "engine.h"

const color WHITE(1, 1, 1);
const color BLACK(0, 0, 0);
const color BLUE(0, 0, 1);
const color YELLOW(1, 1, 0);
const color BUBBLE(0.7, 0.8, 0.8);

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
    // Initialize shapes
    bubble = make_unique<Circle>(shapeShader, vec2(200, 500), 4, BUBBLE);
    sun = make_unique<Circle>(shapeShader, vec2(WIDTH - 10, HEIGHT - 10), vec2(12), YELLOW);

    // Initialize eye
    eye.push_back(make_unique<Circle>(shapeShader, vec2(WIDTH/2-20,HEIGHT/2-20), 5, WHITE));
    eye.push_back(make_unique<Circle>(shapeShader, vec2(WIDTH/2-20,HEIGHT/2-20), 3, BLUE));
    eye.push_back(make_unique<Circle>(shapeShader, vec2(WIDTH/2-20,HEIGHT/2-20), 2, BLACK));

    // Initialize snow
    for (int i = 0; i < 150; ++i) {
        snow.push_back(make_unique<Circle>(shapeShader, vec2(rand() % WIDTH, HEIGHT + 10 + (rand() % HEIGHT)),
                                           (rand() % 5 / 5.0) + 1, WHITE));
    }
}

void Engine::processInput() {
    glfwPollEvents();

    // Close window if escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Mouse position saved to check for collisions
    glfwGetCursorPos(window, &mouseX, &mouseY);
    mouseY = HEIGHT - mouseY; // make sure mouse y-axis isn't flipped


    // K1: What does this code do? When?
  if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
     bubble->setColor(color(BUBBLE.red, BUBBLE.green, BUBBLE.blue, 0));   }

    // K2: What does this code do? When?
  if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) {
       bubble->setColor(color(BUBBLE.red, BUBBLE.green, BUBBLE.blue, 0.7));
   }

    // K3: What happens when the bubble gets big enough to overlap with
    //     other shapes? Which shape will be drawn on top? How do you know?
  if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        bubble->setSize(vec2(bubble->getSize().x + 0.5, bubble->getSize().y + 0.5));
  }

    // M1: What does this line do? What will it look like?
   eye[1]->setColor(color(0, mouseX / double(WIDTH), mouseY / double(HEIGHT), 1));

    bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    // M2: What does this code do? When?
    if (mousePressed) {
      for (unique_ptr<Shape> &e : eye) {
            e->setPos(vec2(mouseX, mouseY));
       }
   }

    // M3: What do these lines do? What will it look like?
  if (mouseX >= 0 && mouseX <= WIDTH && mouseY >= 0 && mouseY <= HEIGHT) {
      eye[1]->setPos(vec2(eye[0]->getPosX() + (mouseX / (double)WIDTH * 25 - 10),

                            eye[0]->getPosY() + (mouseY / (double)HEIGHT * 25 - 10)));
       eye[2]->setPos(vec2(eye[1]->getPosX() + (mouseX / (double)WIDTH * 14 - 7),
                        eye[1]->getPosY() + (mouseY / (double)HEIGHT * 14 - 7)));
    }

}

void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // T1: In what direction will the sun move? What will it look like?
  sun->move(vec2(-0.2, -0.2));

    // T2: What does this code do? What will it look like?
   color sunColor = sun->getColor4();
  sun->setColor(vec3(sunColor.red, sunColor.green - 1.0 / 3500, sunColor.blue));

    // T3: What does this loop do? What will it look like?
    for (unique_ptr<Shape> &flake : snow) {
      flake->moveY(-flake->getSize().y / 5.0);
       if (flake->getPosY() < 0) {
          flake->setPos(vec2(rand() % WIDTH, HEIGHT + flake->getSize().y));
      }
   }
}

void Engine::render() {
    glClearColor(BLACK.red, BLACK.green, BLACK.blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set shader to use for all shapes
    shapeShader.use();

    // To render each shape:
    // 1. Set uniforms
    // 2. Draw shape

    sun->setUniforms();
    sun->draw();

    for (auto &s : snow) {
        s->setUniforms();
        s->draw();
    }

    bubble->setUniforms();
    bubble->draw();

    for (auto &s : eye) {
        s->setUniforms();
        s->draw();
    }

    glfwSwapBuffers(window);
}

bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}