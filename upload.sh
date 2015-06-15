#!/bin/sh

scp rave-*.tar.gz login.hepforge.org:rave/downloads/
cp rave-*.tar.gz ~/afs/www/dist/software/
mv rave-*.tar.gz old/
