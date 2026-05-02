#!/bin/sh

# A simple script to iterate through my audio outputs

# Get default sink (so we know which to toggle away from)
DEFAULT_SINK=$(pactl get-default-sink)

# Get all (active) sinks
sinks=$(pactl list sinks short | grep -o "alsa[A-Za-z0-9._-]*") 


# Sink looping variables
count=0
declare -a sinks_array

# Iterate through sinks to get active
for sink in $sinks
do 
  ((count++)) # increment count
  # echo $count $sink
  sinks_array[$count]=$sink
  if [ "$sink" == "$DEFAULT_SINK" ]; then
    current_sink_number=$count # get current sink number
  fi
done
# echo current: $current_sink_number
next_sink_number=$((current_sink_number+1))

if [ "$current_sink_number" == "$count" ]; then
  next_sink_number=1
  next_sink=${sinks_array[1]}
else
  next_sink=${sinks_array[$next_sink_number]}
fi

# echo next: $next_sink_number $next_sink

# Set inactive sink as active
pactl set-default-sink $next_sink
