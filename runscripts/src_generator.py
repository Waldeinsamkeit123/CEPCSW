#!/usr/bin/env python
  
import os,sys,re,traceback
from datetime import datetime
from string import Template

def generate():

    template_file = open(r'read.py','r')
    tmpl = Template(template_file.read())

    if not os.path.exists('./bashes/'):
        os.makedirs('./bashes/')

#    En_gam = [1, 2, 5, 10, 15, 20, 25, 30]
    for index in range(1,101):
        rndm = '%d'%(index)
    #    En = '%d'%(index)

        lines = []
        lines.append(tmpl.substitute(NX = rndm))

        output_file_name = './bashes/Digi-RotCrystal_%04d.py'%index
        output_file = open(output_file_name,'w')

        output_file.writelines(lines)
        output_file.close()

        print('generate job file : ',output_file_name)

if __name__ == '__main__':

    generate()
