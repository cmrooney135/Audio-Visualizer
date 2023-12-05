import numpy as np
from scipy.io import wavfile
import matplotlib.pyplot as plt

def perform_fft(audio_file):
    # Read the WAV file
    sample_rate, data = wavfile.read(audio_file)

    # Perform FFT
    fft_result = np.fft.fft(data)

    # Calculate the corresponding frequencies
    frequencies = np.fft.fftfreq(len(fft_result), 1/sample_rate)

    # Plot the result
    plt.figure(figsize=(10, 6))
    plt.plot(frequencies, np.abs(fft_result))
    plt.title('FFT of Audio Signal')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Amplitude')
    plt.show()

if __name__ == "__main__":
    audio_file_path = "/res/Music_wav/01 Device Control (intro).wav"  # Replace with the path to your WAV file
    perform_fft(audio_file_path)

