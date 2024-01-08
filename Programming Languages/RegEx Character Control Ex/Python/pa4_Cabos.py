# Alexander Cabos
# PA4 - pa4_Cabos.py
# 3/22/23


import re

# Read the test data from the file
with open('control-char.txt', 'rb') as f:
    data = f.read()


#Fixes all the non-printable characters
fixed_data = data.replace(b'\x05\r\n', b'').replace(b'\x06\r\n ', b'')\
                .replace(b'\r\n kf \x17\x12\'\r\n jf', b'') \
                .replace(b'\x03', b'').replace(b' \x02', b'')


#Convert to string to use regex
fixed_string = fixed_data.decode('utf-8')

#Remove extra spaces between words
fixed_string = re.sub(r"[^\n\S]+", " ", fixed_string)

#Remove extra letters at the end of a line
fixed_string = re.sub(r"\s[A-Z]\s\n", "\n", fixed_string)

#Fix space in the middle of "line" on line 3, I couldn't figure out another way to get this done.
fixed_string = re.sub(r"[i]\s[n]", "in", fixed_string)

#Update file with corrected data
with open('control-char.txt', 'w') as f:
    f.write(fixed_string)