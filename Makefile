CXX = g++ # C++ compiler
CXXFLAGS = -O3 -g -Wall -I include/ -std=c++20 # C++ compiler flags

TARGET = WikiHopper # Target name, output file
# Where to fetch and store the data:
DATA_SRC = https://zenodo.org/record/2539424/files/enwiki.wikilink_graph.2018-03-01.csv.gz?download=1
DATA_FILE = WikipediaLinkGraph.txt

.PHONY: all
all: $(TARGET) $(DATA_FILE) # Target to build everything, including downloading the data

$(TARGET): obj/WikiHopper.o obj/Article.o
	$(CXX) $(CXXFLAGS) obj/WikiHopper.o obj/Article.o -o $(TARGET)

obj/WikiHopper.o: src/WikiHopper.cpp include/Article.hpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c src/WikiHopper.cpp -o obj/WikiHopper.o

obj/Article.o: src/Article.cpp include/Article.hpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c src/Article.cpp -o obj/Article.o

$(DATA_FILE):
	# Download and unzip the data, then parse it with awk to minimize the file size:
	curl $(DATA_SRC) | gzip -d | \
	awk -F"\t" 'NR==1 {RS="\r\n"; next} $$2==last {print $$4; next} NR>2 {print "";} {last=$$2; print $$2 "\n" $$4}' \
	> $(DATA_FILE)

.PHONY: clean dist-clean data-clean all-clean
clean:
	$(RM) obj/*

dist-clean: clean
	$(RM) $(TARGET)

data-clean:
	$(RM) $(DATA_FILE)

all-clean: data-clean dist-clean

