#!/bin/bash

dir="$(dirname "$0")/.."

cat "$dir/wrapper/wrapper.sh" "$dir/bin/main" > "$dir/bin/dotman"
chmod +x "$dir/bin/dotman"
# Install:
#sudo mv "$dir/bin/dotman" /usr/local/bin
