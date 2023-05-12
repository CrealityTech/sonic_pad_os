#!/bin/sh

keypath="/base/audio/playback_and_capture"

capture_pcm_device=$(mjson_fetch "${keypath}/capture_pcm_device")
wav_file=$(mjson_fetch "${keypath}/wav_file")

capture_channels=$(mjson_fetch "${keypath}/capture_channels")
capture_format=$(mjson_fetch "${keypath}/capture_format")
capture_rate=$(mjson_fetch "${keypath}/capture_rate")
capture_duration_sec=$(mjson_fetch "${keypath}/capture_duration_sec")
record_file=$(mjson_fetch "${keypath}/record_file")
remove_record_file=$(mjson_fetch "${keypath}/remove_record_file")
playback_pcm_device=$(mjson_fetch "${keypath}/playback_pcm_device")

if [ ! -f "$wav_file" ]; then
    ttips "No such file: \"$wav_file\""
    exit 1
fi

ttips "Starting playing and capture"
aplay -D "$playback_pcm_device" "$wav_file" &
arecord -D "$capture_pcm_device" -c "$capture_channels" -f "$capture_format" \
    -r "$capture_rate" -d "$capture_duration_sec" "$record_file"
if [ $? -ne "0" ]; then
    ttips "Error occurred when arecord"
    exit 1
fi

ttips "Finish recording, and start playing the record wav file"
aplay -D "$playback_pcm_device" "$record_file"
if [ $? -ne "0" ]; then
    ttips "Error occurred when playing"
    exit 1
fi

ttrue "Finish playing and capture test. Can you hear the sound from speaker?"
exit_code=$?

[ "x$remove_record_file" == "xtrue" ] && rm -f "$record_file"

exit $exit_code
