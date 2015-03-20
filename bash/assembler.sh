#!/bin/bash

# STEP 0 : Script variables
INPUT_FILE=$1
TMP_FILE=prog.tmp
OUTPUT_FILE=prog.vhd

# STEP 1 : Remove comments, empty lines and commas
# Comments start with ; or # and end with EOL
function remove_comments() {
    grep -v '^;\|^#' $INPUT_FILE | sed -e '/^[ \t]*$/d' -e 's/\(.*\)[;#].*/\1/g' -e 's/,//g' > $TMP_FILE
}

# STEP 2 : Read temporary file line by line and replace instructions with
# their binary code
function generate_binary() {
    while read line
    do
        case $(echo ${line^^} | cut -d ' ' -f 1) in
            # Instructions using 3 registery
            ADD|SUB|ADDC|SUBC|AND|OR|XOR|LW|SW)
                vhdl_line=$(echo -n $line | sed -e 's/ / \& /g' \
                                                -e 's/ADD /"0000000" /Ig' \
                                                -e 's/SUB /"0000001" /Ig' \
                                                -e 's/ADDC /"0000100" /Ig' \
                                                -e 's/SUBC /"0000101" /Ig' \
                                                -e 's/AND /"0010000" /Ig' \
                                                -e 's/OR /"0010001" /Ig' \
                                                -e 's/XOR /"0010010" /Ig' \
                                                -e 's/LW /"1011010" /Ig' \
                                                -e 's/SW /"1011011" /Ig' \
                                                -e 's/[R$]0/"000"/Ig' \
                                                -e 's/[R$]1/"001"/Ig' \
                                                -e 's/[R$]2/"010"/Ig' \
                                                -e 's/[R$]3/"011"/Ig' \
                                                -e 's/[R$]4/"100"/Ig' \
                                                -e 's/[R$]5/"101"/Ig' \
                                                -e 's/[R$]6/"110"/Ig' \
                                                -e 's/[R$]7/"111"/Ig')
                echo -e "${vhdl_line}; -- ${line}"
                ;;
            # Instructions using 2 registery and then "000"
            INC|DEC|MOVA|NEGA|NOT|SHL1|SHL2|SHL3|SHL4|SHL5|SHL6|SHL7|SHL8|SHL9|SHL10|SHL11|SHL12|SHL13|SHL14|SHL15|SHR1|SHR2|SHR3|SHR4|SHR5|SHR6|SHR7|SHR8|SHR9|SHR10|SHR11|SHR12|SHR13|SHR14|SHR15)
                vhdl_line=$(echo -n $line | sed -e 's/ / \& /g' \
                                                -e 's/INC /"0000010" /Ig' \
                                                -e 's/DEC /"0000011" /Ig' \
                                                -e 's/MOVA /"0000110" /Ig' \
                                                -e 's/NEGA /"0001000" /Ig' \
                                                -e 's/NOT /"0010011" /Ig' \
                                                -e 's/SHL1 /"0100000" /Ig' \
                                                -e 's/SHL2 /"0100001" /Ig' \
                                                -e 's/SHL3 /"0100010" /Ig' \
                                                -e 's/SHL4 /"0100011" /Ig' \
                                                -e 's/SHL5 /"0100100" /Ig' \
                                                -e 's/SHL6 /"0100101" /Ig' \
                                                -e 's/SHL7 /"0100110" /Ig' \
                                                -e 's/SHL8 /"0100111" /Ig' \
                                                -e 's/SHL9 /"0101000" /Ig' \
                                                -e 's/SHL10 /"0101001" /Ig' \
                                                -e 's/SHL11 /"0101010" /Ig' \
                                                -e 's/SHL12 /"0101011" /Ig' \
                                                -e 's/SHL13 /"0101100" /Ig' \
                                                -e 's/SHL14 /"0101101" /Ig' \
                                                -e 's/SHL15 /"0101111" /Ig' \
                                                -e 's/SHR1 /"0110000" /Ig' \
                                                -e 's/SHR2 /"0110001" /Ig' \
                                                -e 's/SHR3 /"0110010" /Ig' \
                                                -e 's/SHR4 /"0110011" /Ig' \
                                                -e 's/SHR5 /"0110100" /Ig' \
                                                -e 's/SHR6 /"0110101" /Ig' \
                                                -e 's/SHR7 /"0110110" /Ig' \
                                                -e 's/SHR8 /"0110111" /Ig' \
                                                -e 's/SHR9 /"0111000" /Ig' \
                                                -e 's/SHR10 /"0111001" /Ig' \
                                                -e 's/SHR11 /"0111010" /Ig' \
                                                -e 's/SHR12 /"0111011" /Ig' \
                                                -e 's/SHR13 /"0111100" /Ig' \
                                                -e 's/SHR14 /"0111101" /Ig' \
                                                -e 's/SHR15 /"0111111" /Ig' \
                                                -e 's/[R$]0/"000"/Ig' \
                                                -e 's/[R$]1/"001"/Ig' \
                                                -e 's/[R$]2/"010"/Ig' \
                                                -e 's/[R$]3/"011"/Ig' \
                                                -e 's/[R$]4/"100"/Ig' \
                                                -e 's/[R$]5/"101"/Ig' \
                                                -e 's/[R$]6/"110"/Ig' \
                                                -e 's/[R$]7/"111"/Ig')
                echo -e "${vhdl_line} & \"000\"; -- ${line}"
                ;;
            # Instructions using 1 registery, then 000, then 1 registery
            MOVB|NEGB)
                vhdl_line=$(echo -n $line | sed -e 's/ / \& /g' \
                                                -e 's/MOVB /"0000111" /Ig' \
                                                -e 's/NEGB /"0001001" /Ig' \
                                                -e 's/[R$]0/"000"/Ig' \
                                                -e 's/[R$]1/"001"/Ig' \
                                                -e 's/[R$]2/"010"/Ig' \
                                                -e 's/[R$]3/"011"/Ig' \
                                                -e 's/[R$]4/"100"/Ig' \
                                                -e 's/[R$]5/"101"/Ig' \
                                                -e 's/[R$]6/"110"/Ig' \
                                                -e 's/[R$]7/"111"/Ig')
                echo -e "$(echo $vhdl_line | cut -d '&' -f 1)&$(echo $vhdl_line | cut -d '&' -f 2)& \"000\" &$(echo $vhdl_line | cut -d '&' -f 3); -- $line"
        esac
    done < $TMP_FILE > $OUTPUT_FILE
}

# LAUNCH SCRIPT FUNCTIONS
remove_comments $INPUT_FILE
generate_binary

