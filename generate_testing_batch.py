#! /usr/bin/python

from subprocess import (
  Popen,
  PIPE,
  call
)
import json
import random
import string


def randstr(length=20):
  out = ""
  while len(out) < length:
    out += random.choice(string.ascii_lowercase)
  return out

call( ["rm sample/*"], shell=True )

data = {}
for i in range(10000):
  id = randstr();
  p = Popen( ["bash generate_captcha.sh sample/"+id+".jpg"], shell=True, stdout=PIPE )
  p.wait()
  data[id] = json.loads( p.stdout.read() )

print json.dumps(data)

