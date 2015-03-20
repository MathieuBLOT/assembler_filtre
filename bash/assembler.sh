#!/bin/bash

# STEP 0 : Script variables
INPUT_FILE=$1
TMP_FILE=assembler.tmp

# STEP 1 : Remove comments and empty lines
# Comments start with ; or # and end with EOL
function remove_comments() {
    grep -v '^;\|^#' $INPUT_FILE | sed -e '/^[ \t]*$/d' -e 's/\(.*\)[;#].*/\1/g' > $TMP_FILE
}

# STEP 2 : Parse instructions and find opcodes


# LAUNCH SCRIPT FUNCTIONS
remove_comments $1
