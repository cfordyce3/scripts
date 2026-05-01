#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // Current sink buffer
  char current_sink_buffer[256];

  // Run commands to get sinks and store info
  FILE *current_sink_FILE = popen("pactl get-default-sink", "r");
  FILE *active_sinks_FILE = popen("pactl list sinks short | grep -o \"alsa[A-Za-z0-9._-]*\"", "r");

  // Verify popen functions did not fail
  if (current_sink_FILE == NULL || active_sinks_FILE == NULL) {
    perror("popen failed");
    return 1;
  }

  // Get and set the current sink
  char *current_sink = fgets(current_sink_buffer, sizeof(current_sink_buffer), current_sink_FILE);

  // Remove trailing newline characters
  int i=0;
  while (current_sink[i] != '\0') {
    if (current_sink[i] == '\n') {
      // printf("at index: %d\n", i); 
      current_sink[i] = '\0';
    }
    i++;
  }

  // Array of active sinks
  char **active_sinks = malloc(sizeof(*active_sinks));

  // Getting sinks variables
  int num_active_sinks = 0, current_sink_index = 0;
  // New sinks buffer
  char active_sinks_buffer[256];

  // Iterate through all active sinks and determine which is currently active
  while (fgets(active_sinks_buffer, sizeof active_sinks_buffer, active_sinks_FILE) != NULL) {
    active_sinks = realloc(active_sinks, (num_active_sinks+2) * sizeof(*active_sinks)); // resize number of sinks
    active_sinks[num_active_sinks] = malloc(sizeof(active_sinks_buffer) * sizeof(char)); // allocate memory for new string

    strcpy(active_sinks[num_active_sinks], active_sinks_buffer); // copy buffer to newly allocated memory
    active_sinks[num_active_sinks][strcspn(active_sinks[num_active_sinks], "\n")] ='\0'; // replace \n with \0

    // if currently iterated sink is the active one
    if (strcmp(active_sinks[num_active_sinks], current_sink) == 0) {
      current_sink_index = num_active_sinks;
    }  

    num_active_sinks++;
  }

  // Determine next sink to use
  int next_sink = (current_sink_index == (num_active_sinks-1)) ? 0 : current_sink_index + 1;

  // Create command to run
  char command[] = "pactl set-default-sink "; 
  strcat(command, active_sinks[next_sink]);
  system(command); // run command

  // Close FILES
  pclose(current_sink_FILE);
  pclose(active_sinks_FILE);

  // Free memory
  free(active_sinks);

  return 0;
}
