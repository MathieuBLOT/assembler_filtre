#!/bin/bash

# STEP 0 : Script variables
INPUT_FILE=$1
TMP_FILE=prog.tmp
OUTPUT_FILE=prog.vhd

# STEP 0bis : Handle imm values properly
# Imm values are supposed to be given in base 10, prefixed by (-) if they are
# negative
function convert_imm() {
    decimal=$1
    udecimal=$(echo $decimal | sed 's/-//g')
    ubinary=$(echo "obase=2;$udecimal" | bc)
    # Check if the binary number is not too great
    if [[ "$(echo $ubinary | wc -m)" -gt 9 ]]; then
        echo "111111111"; # Error value
    else
        # Positive number
        if [[ "$decimal" -eq "$udecimal" ]]; then
            while [ "$(echo $ubinary | wc -m)" -le 9 ]
            do
                ubinary="0${ubinary}"
            done
        # Negative number
        else
            # One's complement
            ubinary=$(echo "0${ubinary}" | sed 's/0/X/g' | sed 's/1/0/g' | sed 's/X/1/g')
            # Two's complement
            ubinary=$(echo "ibase=2;obase=2;$ubinary+1" | bc)
            # Sign extension
            while [ "$(echo $ubinary | wc -m)" -le 9 ]
            do
                ubinary="1${ubinary}"
            done
        fi
        echo $ubinary
    fi
}

# STEP 1 : Remove comments, empty lines and commas
# Comments start with ; or # and end with EOL
function remove_comments() {
    grep -v '^;\|^#' $INPUT_FILE | sed -e '/^[ \t]*$/d' -e 's/\(.*\)[;#].*/\1/g' -e 's/, / /g' -e 's/,/ /g' > $TMP_FILE
}

# STEP 2 : Read temporary file line by line and replace instructions with
# their binary code
function generate_binary() {
    linenb=0
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
                                                -e 's/XOR /"0010010" /Ig' \
                                                -e 's/OR /"0010001" /Ig' \
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
                echo -e "${linenb} => $(echo $vhdl_line | cut -d '&' -f 1)&$(echo $vhdl_line | cut -d '&' -f 2)&$(echo $vhdl_line | cut -d '&' -f 3)&$(echo $vhdl_line | cut -d '&' -f 4),    -- $line"
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
                                                -e 's/SHL15 /"0101110" /Ig' \
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
                                                -e 's/SHR15 /"0111110" /Ig' \
                                                -e 's/[R$]0/"000"/Ig' \
                                                -e 's/[R$]1/"001"/Ig' \
                                                -e 's/[R$]2/"010"/Ig' \
                                                -e 's/[R$]3/"011"/Ig' \
                                                -e 's/[R$]4/"100"/Ig' \
                                                -e 's/[R$]5/"101"/Ig' \
                                                -e 's/[R$]6/"110"/Ig' \
                                                -e 's/[R$]7/"111"/Ig')
                echo -e "${linenb} => $(echo $vhdl_line | cut -d '&' -f 1)&$(echo $vhdl_line | cut -d '&' -f 2)&$(echo $vhdl_line | cut -d '&' -f 3) & \"000\",    -- $line"
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
                echo -e "${linenb} => $(echo $vhdl_line | cut -d '&' -f 1)&$(echo $vhdl_line | cut -d '&' -f 2)& \"000\" &$(echo $vhdl_line | cut -d '&' -f 3),    -- $line"
                ;;
            # Instructions BR* an BA*
            BREQ|BRGE|BRLE|BRUMP|BRNE|BRLT|BRGT|BAEQ|BAGE|BALE|BAUMP|BANE|BALT|BAGT)
                vhdl_line=$(echo -n $line | sed -e 's/ / \& /g' \
                                                -e 's/BREQ /"1000000" /Ig' \
                                                -e 's/BRGE /"1000001" /Ig' \
                                                -e 's/BRLE /"1000010" /Ig' \
                                                -e 's/BRUMP /"1000011" /Ig' \
                                                -e 's/BRNE /"1000100" /Ig' \
                                                -e 's/BRLT /"1000101" /Ig' \
                                                -e 's/BRGT /"1000110" /Ig' \
                                                -e 's/BAEQ /"1001000" /Ig' \
                                                -e 's/BAGE /"1001001" /Ig' \
                                                -e 's/BALE /"1001010" /Ig' \
                                                -e 's/BAUMP /"1001011" /Ig' \
                                                -e 's/BANE /"1001100" /Ig' \
                                                -e 's/BALT /"1001101" /Ig' \
                                                -e 's/BAGT /"1001110" /Ig' \
                                                -e 's/[R$]0/"000"/Ig' \
                                                -e 's/[R$]1/"001"/Ig' \
                                                -e 's/[R$]2/"010"/Ig' \
                                                -e 's/[R$]3/"011"/Ig' \
                                                -e 's/[R$]4/"100"/Ig' \
                                                -e 's/[R$]5/"101"/Ig' \
                                                -e 's/[R$]6/"110"/Ig' \
                                                -e 's/[R$]7/"111"/Ig')
                echo -e "${linenb} => $(echo $vhdl_line | cut -d '&' -f 1)& \"000\" &$(echo $vhdl_line | cut -d '&' -f 2)&$(echo $vhdl_line | cut -d '&' -f 3),    -- $line"
                ;;
            # Instructions with 1 registery
            IN|OUT)
                vhdl_line=$(echo -n $line | sed -e 's/ / \& /g' \
                                                -e 's/IN /"1011000" /Ig' \
                                                -e 's/OUT /"1011001" /Ig' \
                                                -e 's/[R$]0/"000"/Ig' \
                                                -e 's/[R$]1/"001"/Ig' \
                                                -e 's/[R$]2/"010"/Ig' \
                                                -e 's/[R$]3/"011"/Ig' \
                                                -e 's/[R$]4/"100"/Ig' \
                                                -e 's/[R$]5/"101"/Ig' \
                                                -e 's/[R$]6/"110"/Ig' \
                                                -e 's/[R$]7/"111"/Ig')
                echo -e "${linenb} => $(echo $vhdl_line | cut -d '&' -f 1)&$(echo $vhdl_line | cut -d '&' -f 2) & \"000\" & \"000\",    -- ${line}"
                ;;
            RESET)
                echo -e "${linenb} => \"1111111\" & \"111\" & \"111\" & \"111\", -- ${line}"
                ;;
            # Instructions with imm
            BRIEQ|BRIGE|BRILE|BRIUMP|BRINE|BRILT|BRIGT|LI)
                vhdl_line=$(echo -n $line | sed -e 's/LI [R$]0 /"1100000" /Ig' \
                                                -e 's/LI [R$]1 /"1100001" /Ig' \
                                                -e 's/LI [R$]2 /"1100010" /Ig' \
                                                -e 's/LI [R$]3 /"1100011" /Ig' \
                                                -e 's/LI [R$]4 /"1100100" /Ig' \
                                                -e 's/LI [R$]5 /"1100101" /Ig' \
                                                -e 's/LI [R$]6 /"1100110" /Ig' \
                                                -e 's/LI [R$]7 /"1100111" /Ig' \
                                                -e 's/ / \& /g' \
                                                -e 's/BRIEQ /"1010000" /Ig' \
                                                -e 's/BRIGE /"1010001" /Ig' \
                                                -e 's/BRILE /"1010010" /Ig' \
                                                -e 's/BRIUMP /"1010011" /Ig' \
                                                -e 's/BRINE /"1010100" /Ig' \
                                                -e 's/BRILT /"1010101" /Ig' \
                                                -e 's/BRIGT /"1010110" /Ig' \
                                                -e 's/ \([-]\?[0-9]\{1,\}\)/ "convert_imm &"/Ig')
                command=$(echo $vhdl_line | cut -d '&' -f 2 | cut -d '"' -f 2)
                rescommand=$(eval $command)
                echo -e "${linenb} => $(echo $vhdl_line | cut -d '&' -f 1)& \"${rescommand}\",    -- ${line}"
                ;;
            *)
                echo -e "-- Line skipped due to error (unrecognized instruction)"
                ;;
        esac
        linenb=$(($linenb + 1))
    done < $TMP_FILE > $OUTPUT_FILE
}

# LAUNCH SCRIPT FUNCTIONS
remove_comments $INPUT_FILE
generate_binary

# Delete temporary file
rm $TMP_FILE
