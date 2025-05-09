cvlc -vvv http://stream.radioparadise.com/rock-flacm --sout="#transcode{vcodec=none,acodec=flac,ab=320,channels=2,samplerate=48000}:http{mux=flac,dst=:8082/rock_flac}" --sout-keep --loop
