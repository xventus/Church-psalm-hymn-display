# Copyright (c) 2019 Petr Vanek, petr@fotoventus.cz
# html files downloaded (wgtet mirror) from https://kancional.cz are used as input
# and then converted to a form suitable for the display.
#

from abc import ABC
from html.parser import HTMLParser
import os

# setting file paths for conversion

hymnSource = "./../data/hymn/source/html/"
hymnDestination = "./../data/hymn/destination/"

# VGA display text grid
char_per_line = 28
count_max_lines = 8




class MyHTMLParser1(HTMLParser):

    def error(self, message):
        pass

    def __init__(self):
        super().__init__()
        self.main = False
        self.counter = 0
        self.text = ''

    def handle_starttag(self, tag, attrs):
        if self.main:
            self.counter += 1

        if tag == 'main':
            self.main = True

    def handle_data(self, data):
        xxx = data
        line = xxx.replace('\n', '')
        line = line.strip()
        line = line.replace(chr(160), " ")
        line = line.replace("–", "")

        if self.main and self.counter == 2 and self.text == '':
            self.text = line

    def get_text(self):
        return self.text


############################################

class MyHTMLParser2(HTMLParser, ABC):

    def __init__(self):
        super().__init__()
        self.main = False
        self.counter = 0
        self.text = []

    def handle_starttag(self, tag, attrs):
        if self.main:
            self.counter += 1

        if tag == 'main':
            self.main = True

    def handle_data(self, data):
        # print("Counter  :", self.counter)
        # print("Encountered some data  :", data)
        if self.main and self.counter > 3:
            xxx = data
            line = xxx.replace('\n', '')
            line = line.strip()
            line = line.replace(chr(160), " ")
            line = line.replace("–", "")
            if len(line) != 0:
                if len(line) == 1:
                    self.main = False

                if self.main:
                    if line[1] == '.':
                        line = line[:0] + line[3:]
                        line = line.strip()
                        if len(line) > 14:
                            self.text.append(line)

    def get_text(self):
        return self.text


def get_song(path):
    parser = MyHTMLParser1()
    file1 = open(path, "r")
    content = file1.read()
    file1.close()
    parser.feed(content)

    if len(parser.get_text()) != 0:
        return parser.get_text()
    else:
        parser2 = MyHTMLParser2()
        parser2.feed(content)
        return parser2.get_text()
    return []


def decode_and_split(in_text):
    return_code = ""
    max_lines = count_max_lines
    skip_first = 0
    words = in_text.split()
    line = ""
    line_counter = skip_first
    first_line = True
    for x in words:

        if (line_counter + len(x)) > char_per_line:
            line_counter = 0
            if first_line:
                first_line = False
                tmp = line.ljust(char_per_line - skip_first)[:char_per_line - skip_first]
            else:
                tmp = line.ljust(char_per_line)[:char_per_line]
            return_code += tmp
            line = x + " "
            line_counter += len(x) + 1
            max_lines -= 1
            if max_lines == 0:
                break
        else:
            line_counter += len(x) + 1
            line += x + " "

    if max_lines == 0:
        return_code = return_code.strip()
        rest_data = (char_per_line * count_max_lines) - len(return_code)
        # print('rest:', rest_data)
        # print('lastline:', len(line))
        # print('free of last line :', perline-len(line))
        N = rest_data
        if rest_data > 3:
            N = 3

        return_code = return_code.ljust(N + len(return_code), '.')
        # print('rest:', len(return_code))

    else:
        if line_counter != 0:
            return_code += line

    return_code = return_code.encode('cp1250', 'replace')
    # print(return_code)
    return return_code


try:
    os.mkdir(hymnDestination)
except OSError:
    print("Creation of the directory %s failed" % hymnDestination)

arr = os.listdir(hymnSource)
for x in arr:
    # print(x)
    # print("\n")
    rc = get_song(hymnSource + x)
    if isinstance(rc, list):
        # list of lines
        try:
            os.mkdir(hymnDestination + x)
        except OSError:
            print("Creation of the directory %s failed" % x)
        pos = 0
        for y in rc:
            pos += 1
            with open(hymnDestination + x + "/" + str(pos) + ".txt", "bw") as text_file:
                text_file.write(decode_and_split(y))
    else:
        # only one line
        try:
            os.mkdir(hymnDestination + x)
        except OSError:
            print("Creation of the directory %s failed" % x)

        with open(hymnDestination + x + "/1.txt", "bw") as text_file:

            text_file.write(decode_and_split(rc))

    if len(rc) == 0:
        os.rmdir(hymnDestination + x)
