espeak hello -w hello.wav

ffmpeg -i file.wav -f s16be -ar 8000 -acodec pcm_s16be file.raw
ffmpeg -i hello.wav -f s16be -ar 8000 -acodec pcm_s16be hello.raw
