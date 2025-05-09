cvlc -vvv http://stream.radioparadise.com/flacm --sout="#transcode{vcodec=none,acodec=flac,ab=320,channels=2,samplerate=48000}:http{mux=flac,dst=:8083/main_flac}" --sout-keep --loop
