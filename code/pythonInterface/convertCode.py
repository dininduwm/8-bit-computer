def convertToMCode(path):
    macineCode = {"NOP":"0000", "LDA": "0001", "ADD": "0010", "SUB": "0011", "STA": "0100", "LDI": "0101", "JMP": "0110", "OUT": "1110", "HLT": "1111"} #machine code array
    mcodeArr = [0]*16
    count = 0
    try:
        f= open(path,"r")
    except:
        return("FILE ERROR")

    lines = f.readlines() #reading the lines and put them into an array

    for line in lines:
        line = line.replace("\n", "")
        splitCmd = list(line.split())
        cmd = splitCmd[0]
        try:
            data = int(splitCmd[1])
        except:
            data = 0

        if cmd in macineCode:
            mCode = (int("0b" + macineCode[cmd], 2)<<4)+data
            mcodeArr[count] = mCode
            count += 1
        elif (cmd == "PUT"):
            mcodeArr[count] = data
            count += 1
        else:
            return("ERROR ASSEMBLY CODE")

    f.close()
    return(mcodeArr)

