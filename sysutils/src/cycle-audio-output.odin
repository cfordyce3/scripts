package sysutils

import "core:fmt"
import "core:os"

main :: proc() {
  if err := echo(); err != nil {
    os.print_error(os.stderr, err, "failed to execute echo")
  }
}

echo :: proc() -> (err: os.Error) {
  r, w := os.pipe() or_return
  defer os.close(r)

  p: os.Process; {
    defer os.close(w)

    p = os.process_start({
      command = {"echo", "hello world"},
      stdout = w,
    }) or_return
  }

  output := os.read_entire_file(r, context.temp_allocator) or_return

  _ = os.process_wait(p) or_return

  fmt.print(string(output))

  return
}

