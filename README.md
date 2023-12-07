# Final-Project-cmrooney

This is my C++ final project!!! 
I used a plugin called aubio to generate waveforms from some of my fav songs :) .
 The height of the rectangles that make up the soundbar are determined by the amplitude of that sample of the song 
(this means how loud it is). 
The color of each rectangle is determined by its pitch! so the frequency at that given time (this is a little bit more DSP 
than advanced programming but I figured out a way to make it work). The more red a bar is the higher the frequency, and if its
more blue, the frequency is lower. 
For the sake of simplicity I included 5 of my favorite songs for the "Menu" because I have like 2000 wav files that would 
work for this project. Also, in theory, any mp3 or wav file could work becuse i included a python script in the util folder
that converts mp3 files to .wav! 
I have many other songs im not putting on Git so I know it works for ALL mp3 files. 
I wanted to include a movement with BPM being extracted but no dice. The frequency domain is terrible to deal with in C++.
Unfortunately this is not a real time audio visualizer :( If i were to recreate this project I would use a newer, more reasonable 
and more accessable audio analysis library like portaudio. 

I didnt add any user input validation but it just wont work if you press anything besides 1-5 so. 
Also my top 5 are all frank ocean songs sorry about it its the easiest file to access in my computer. 
thanks for listening (???)
