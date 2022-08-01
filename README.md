# simple_wav_loader
A simple wav loader in C++

Created while trying to make an OpenAL audio player for a game engine.  It was surprisingly easy to load a wav file in this way.  
A reimplementation of the version I used contained to a single header without any open AL specifics

## How to use
1. Include the header file in your project
2. Define NL_WAV_LOADER above 1 include in any cpp file, or include the provided cpp file to be compiled
3. Pass in a file name and use the returned char* data as the audio.
4. Pass in optional parameters so you can verify the channels and format.

I have used the char* data that is returned to fill a buffer in openAL
