# WikiHopper
A program to find the shortest path between two Wikipedia articles.

# Usage
1. Clone the repository
2. Run `make`. <br>
On the first run, this will download and parse the Wikipedia dump. This will take a long time.
3. Run `./WikiHopper` <br>
This will first load the aforementioned file, which will take a long time.

This process will take a long time, so it is recommended to run the following install script: <br>
`git clone https://github.com/MRegirouard/WikiHopper.git && cd WikiHopper && make -j3 && ./WikiHopper` <br>
Note that only three `make` jobs need to be run, as there are only a few files to build.
<br>
<br>
The program will promt for two article names, and then will find the shortest path from the start to the end article, traversing via page links.
