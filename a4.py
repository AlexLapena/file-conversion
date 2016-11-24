# Alex Lapena | a4.py | 0844071
#!/usr/bin/python3


import os, sys
import time

if __name__ == "__main__":

    time.sleep(1);
    #Reads in text file
    fname = "./files/q1"
    pipe = open(fname, "rw")
    fileName, pText = pipe.read().split(' ',1)
    os.remove(fname)

    htmlFile = fileName + ".html"
    infoFile = fileName + ".info"

    if os.path.isfile(infoFile):
        # Reads in info file
        with open(infoFile) as info:
            infoLine = info.readlines()
            infoLine = [infoLine.rstrip('\n') for infoLine in open(infoFile)]

        # Searches for Strings, replaces with HTML tags
        for line in infoLine:
            if (line == ""): 
                continue;
            # Bold
            if line[0] == 'B':
                subject = line[2:len(line)]
                newText = "<B>" + subject + "</B>"
                if subject in pText:
                    pText = pText.replace(subject, newText)
            # Italics
            elif line[0] == 'I':
                subject = line[2:len(line)]
                newText = "<I>" + subject + "</I>"
                if subject in pText:
                    pText = pText.replace(subject, newText)
            # Underlines
            elif line[0] == 'U':
                subject = line[2:len(line)]
                newText = "<U>" + subject + "</U>"
                if subject in pText:
                    pText = pText.replace(subject, newText)

    # Opens file to be written to
    fname = "./files/q2"
    time.sleep(1);
    pipe = os.open(fname, os.O_WRONLY)

    os.write(pipe, "<HTML>\n\n<HEAD>\n<TITLE>"+ fileName + "</TITLE>\n</HEAD>\n\n<BODY>\n" + pText + "\n</BODY>\n\n</HTML>\n")

    os.close(pipe);

