#!/bin/bash
# Get absolut path
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Navigate to piracer_py directory
if [[ "$SCRIPT_DIR" != */app/piracer_py ]]; then
    echo "Error: The script must be located in [PROJECT_DIR]/app/piracer_py"
    exit 1
fi
# set permissions
chmod 755 $SCRIPT_DIR/script/monitor_main.sh
chmod 755 $SCRIPT_DIR/script/script/restart.sh
# 
bash $SCRIPT_DIR/script/monitor_main.sh