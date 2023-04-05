import string
import random
import pprint

MAX = 300

def id_generator(size=6, chars=string.ascii_uppercase + string.ascii_lowercase):
    return ''.join(random.choice(chars) for _ in range(size))

channels_cap = ['RED', 'BLUE', 'GREEN']

channels = ['red', 'blue', 'green']

files = []
name = ''

for i in range(MAX):
    for channel in channels_cap:
        name = id_generator() + '_' + str(i) + '_' + channel + '.ome.tif'
        files.append(name)

pprint.pprint(files)