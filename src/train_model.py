#! /usr/bin/python

# 
#  train_model.py
#    used to create a training model and store it in data.inl
#
#


BATCH_SIZE = 100


from subprocess import call
import json
import string

call( "rm train ; g++ -o train train_model.cpp -std=c++0x" , shell=True)

o = json.loads(open("../training_data.json","r").read())


params = ""
count = 0
for case in o:
  params += "../training_sample/" + case + ".jpg "
  params += o[case]["text"] + " "
  count += 1
  if count >= BATCH_SIZE: break;

call( "echo '" + params + "' | ./train > data.inl", shell=True )
#call( "./build_decaptcher", shell=True )
