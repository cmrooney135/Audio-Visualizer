
#include "framework/engine.h"

#include <iostream>


//int main(int argc, char *argv[]) {
//    Engine engine;
//
//    while (!engine.shouldClose()) {
//        engine.processInput();
//        engine.update();
//        engine.render();
//    }
//
//    glfwTerminate();
//    return 0;
//}
#include <iostream>
#include <aubio.h>


int main() {
    aubio_source_t* source = new_aubio_source("../res/Music_wav/02 strawberry swing.wav", 0, 0);
    if (!source) {
        std::cerr << "Error opening the audio file." << std::endl;
        return 1;
    }
    cout << "aubio working" << endl;

    // Cleanup aubio
    aubio_cleanup();
    cout << "aubio cleaned up" << endl;


    return 0;
}