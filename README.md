# WikiHopper
A program to find the shortest path between two Wikipedia articles.

![image](https://user-images.githubusercontent.com/46038297/192924634-666cc76a-08a1-498f-aae7-1991253084fc.png)

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
The program will promt for two article names, and then will find the shortest path from the start to the end article, traversing via page links.<br>
Note that some paths may not still work, as Wikipedia is often changed, and the dataset is from 2018.

## Fun Paths to Try:
### "Water" to "Computer"
1. [Water](https://en.wikipedia.org/wiki/Water)
2. [Aquarium](https://en.wikipedia.org/wiki/Aquarium)
3. [Computer](https://en.wikipedia.org/wiki/Computer)

### "Green" to "1483"
1. [Green](https://en.wikipedia.org/wiki/Green)
2. [Ancient Egypt](https://en.wikipedia.org/wiki/Ancient_Egypt)
3. [Levant](https://en.wikipedia.org/wiki/Levant)
4. [15th century](https://en.wikipedia.org/wiki/15th_century)
5. [1483](https://en.wikipedia.org/wiki/1483)

### "Carpet" to "Linux"
1. [Carpet](https://en.wikipedia.org/wiki/Carpet)
2. [Afghanistan](https://en.wikipedia.org/wiki/Afghanistan)
3. [Voice of America](https://en.wikipedia.org/wiki/Voice_of_America)
4. [Linux](https://en.wikipedia.org/wiki/Linux)

# Method
The program uses a iterative breadth-first search to traverse the article graph, and a hashmap for fast retreival. Articles are first loaded from the text file into an `Article` object, which contains the article name, and a vector of pointers to other `Article` objects that this one links to within Wikipedia. These objects are then put into an `unordered_map`, so they can be found in constant time to determine if an article exists or not. The breadth-first search then finds the shortest path between nodes, in a relatively fast manner.
