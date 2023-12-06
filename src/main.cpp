
#include "framework/engine.h"
int main(int argc, char *argv[]) {
    Engine engine;

    while (!engine.shouldClose()) {
        engine.processInput();
        engine.update();
        engine.render();
    }

    glfwTerminate();
    return 0;
}

//int main() {
//    int hop_size = 128;
//    int samplerate = 48000;
//    vector <float> ampVec;
//    uint_t hopSize = 256;  // Adjust as needed
//    fvec_t* buffer = new_fvec(hopSize / 2);
//    aubio_source_t* source = new_aubio_source("../res/Music_wav/02 strawberry swing.wav", samplerate, hop_size);
//    if (!source) {
//        std::cerr << "Error opening the audio file." << std::endl;
//        return 1;
//    }
//    cout << "aubio working" << endl;
//    do {
//        aubio_source_do(source, buffer, &hopSize);
//        for (uint_t i = 0; i < hopSize / 2; ++i) {
//            float amplitude = buffer->data[i];
//            ampVec.push_back(amplitude);
//        }
//        for (int i = 0; i<ampVec.size(); i++){
//
//        }
//
//
//
//
//
//
//    } while (hopSize != 0);
//    // Cleanup aubio
//    aubio_cleanup();
//    cout << "aubio cleaned up" << endl;
//
//
//    return 0;
//}