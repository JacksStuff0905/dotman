#!/bin/bash


# Find offset of embedded binary (after __ARCHIVE_BELOW__)
ARCHIVE_LINE=$(awk '/^__ARCHIVE_BELOW__/ {print NR + 1; exit}' "$0")
tail -n +$ARCHIVE_LINE "$0" > /tmp/linker_bin$$
chmod +x /tmp/linker_bin$$

# Pass current environment to the binary
sudo -E /tmp/linker_bin$$ "$@"
STATUS=$?
rm -f /tmp/linker_bin$$
exit $STATUS

__ARCHIVE_BELOW__
