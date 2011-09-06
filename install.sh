#!/bin/bash

# get Dependency
Dependency_List="libqwt5-qt4-dev libfann-dev libfftw3-dev libksane-dev libtbb-dev"
sudo apt-get install $Dependency_List

# extract wfdb
cd etc
tar xfvz wfdb.tar.gz
cd wfdb-10.*

# install wfdb
./configure
make
sudo make install
cd ../
rm  -f -r wfdb-10.*
cd ../

# make and install
cd Binary
make
sudo make install
echo "Installation Finished"