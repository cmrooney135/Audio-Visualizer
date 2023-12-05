from pydub import AudioSegment

from pydub.utils import mediainfo
mediainfo.FFPROBE = "/opt/homebrew/bin/ffprobe"  # Replace with the actual path
import os

input_folder = "/Users/carolrooney/CLionProjects/Final-Project-cmrooney/music"
output_folder = "/Users/carolrooney/CLionProjects/Final-Project-cmrooney/Music_wav"

os.makedirs(output_folder, exist_ok=True)

for file_name in os.listdir(input_folder):
    if file_name.endswith(".mp3"):
        mp3_path = os.path.join(input_folder, file_name)

        # Load MP3 file
        audio = AudioSegment.from_file(mp3_path, format="mp3")

        # Construct output file name (replace .mp3 with .wav)
        wav_name = os.path.splitext(file_name)[0] + ".wav"
        wav_path = os.path.join(output_folder, wav_name)

        # Export as WAV
        audio.export(wav_path, format="wav")

        print(f"Converted: {file_name} -> {wav_name}")
