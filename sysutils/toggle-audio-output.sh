#!/bin/sh

# A simple script to toggle my audio output between my speakers and my headphones

# Get default sink (so we know which to toggle away from)
DEFAULT_SINK=$(pactl get-default-sink)

# Get all sinks
sinks=$(pactl list sinks short | grep -o "alsa[A-Za-z0-9._-]*") 

# Iterate through sinks
for sink in $sinks
do 
  if [ "$sink" == "$DEFAULT_SINK" ]; then
    current_sink=$sink
  else
    inactive_sink=$sink
  fi
done

# Set inactive sink as active
pactl set-default-sink $inactive_sink
