# Copyright (c) 2019 Petr Vanek, petr@fotoventus.cz
#
# simple display for checking alignment
#


import os

hymnDestination = "./../data/hymn/destination/"
# VGA display text grid
char_per_line = 28
count_max_lines = 8

class Display:

    def __init__(self):
        self.counter = 0
        self.text = ''
        self.chapter = []

    def load(self, path):
        list_of_files = os.listdir(path)
        for x in list_of_files:
            full = path + '/'
            full += x
            with open(full, 'r', encoding='cp1250') as file:
                content = rc = file.read()
                self.chapter.append(content)

    def print(self):
        for x in reversed(self.chapter):
            print("+----------------------------+")
            chunks, chunk_size = len(x), char_per_line
            lines = [x[i:i + chunk_size] for i in range(0, chunks, chunk_size)]

            for y in lines:
                h = '|'
                h += y
                h += '|'

                print(h)
            print("+----------------------------+")


list_of_dirs = os.listdir(hymnDestination)
list_of_dirs.sort()
for x in list_of_dirs:
    print('***************************************')
    print(x)
    d = Display()
    d.load(hymnDestination + x)
    d.print()
