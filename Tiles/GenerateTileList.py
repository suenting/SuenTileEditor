import os
OutputFile = open('Tiles.txt','w')

def Main():
    for f in os.listdir('.'):
        if(f.endswith('.png') or f.endswith('.jpg')):
            OutputFile.write(f)
            OutputFile.write('\n')
    OutputFile.close()
Main()
