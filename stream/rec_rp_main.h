ffmpeg -i http://stream.radioparadise.com/flacm -c copy -map 0 -f segment -segment_time 3600 -segment_format flac "/media/usb01/music_01/record/main/rp_main-%04d.flac"
