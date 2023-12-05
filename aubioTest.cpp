#include <iostream>
#include "aubio/src/aubio.h"
#include "aubio/src/spectral/fft.h"
#include "aubio/src/types.h"

int main() {
    // Initialize aubio
    if (aubio_init() != 0) {
        std::cerr << "Error initializing aubio." << std::endl;
        return 1;
    }

    // Your existing code ...

    aubio_source_t* source = new_aubio_source("your_audio_file.wav", 0, 0);
    if (!source) {
        std::cerr << "Error opening the audio file." << std::endl;
        return 1;
    }

    // Get the current sample
    uint_t current_sample = aubio_source_get_sample(source);
    std::cout << "Current sample: " << current_sample << std::endl;

    // Continue with the rest of your code ...

    // Cleanup aubio
    aubio_cleanup();

    return 0;
}