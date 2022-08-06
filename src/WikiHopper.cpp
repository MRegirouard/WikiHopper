#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include "Article.hpp"

#define FILE_LENGTH 190717816
#define LONGEST_LINE_LENGTH 251
#define PROGRESS_BAR_WIDTH 75
#define DEFAULT_FILE "WikipediaLinkGraph.txt"

std::vector<const Article*> breadthFirstSearch(const Article* start, const Article* end);
std::unordered_map<std::string, Article> readFile(const std::string filename);

int main(int argc, char* argv[])
{
	std::unordered_map<std::string, Article> articles;

	if (argc == 2)
		articles = readFile(argv[1]);
	else
		articles = readFile(DEFAULT_FILE);

	while (true) // Continuously prompt for start and end search
	{
		std::cout << std::endl << "Enter an article name: ";
		std::string articleName;
		std::getline(std::cin, articleName);

		if (!articles.contains(articleName.c_str()))
		{
			std::cerr << "Article \"" << articleName << "\" not found." << std::endl;
			continue;
		}

		std::cout << "Enter an article to link to: ";
		std::string linkName;
		std::getline(std::cin, linkName);

		if (!articles.contains(linkName.c_str()))
		{
			std::cerr << "Article \"" << linkName << "\" not found." << std::endl;
			continue;
		}

		std::cout << "Linking " << articleName << " to " << linkName << "..." << std::endl;

		// Locate the articles in the map and start the search
		const Article* start = &articles.find(articleName)->second;
		const Article* end = &articles.find(linkName)->second;

		using namespace std::chrono;
		const int64_t startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		const std::vector<const Article*> path = breadthFirstSearch(start, end);
		const int64_t endTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

		std::cout << "Search took " << (endTime - startTime) << "ms." << std::endl;

		if (path.size() > 0)
		{
			std::cout << std::endl << "Path: " << std::endl;

			for (size_t i = 0; i < path.size(); i++)
				std::cout << i + 1 << ". " << path.at(i)->name << std::endl;
		}
		else
		{
			std::cout << "No path found." << std::endl;
		}
	}
}

/**
 * @brief Reads the given file into the map of articles, showing a progress bar as it's reading
 * @param filename The filename to read
 */
std::unordered_map<std::string, Article> readFile(const std::string filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{
		std::cerr << "Could not open file " << filename << std::endl;
		exit(1);
	}

	std::cout << "Reading file " << filename << "..." << std::endl;
	using namespace std::chrono;
	const int64_t startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

	std::unordered_map<std::string, Article> articles; // Maps article names to articles

	char* line = new char[LONGEST_LINE_LENGTH + 1];
	bool nextIsName = true;
	Article* currentArticle = NULL;
	int count = 0;
	const int printEvery = FILE_LENGTH / 1000;
	std::cout << std::setprecision(1) << std::fixed;

	while (file.getline(line, LONGEST_LINE_LENGTH + 1))
	{
		if (count % printEvery == 0)
		{
			// Progress bar credit: https://stackoverflow.com/a/14539953
			std::cout << "[";
			int pos = PROGRESS_BAR_WIDTH * (float) count / FILE_LENGTH;
			for (int i = 0; i < PROGRESS_BAR_WIDTH; i++)
			{
				if (i < pos) std::cout << "=";
				else if (i == pos) std::cout << ">";
				else std::cout << " ";
			}

			const int64_t currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
			const int mins = (currentTime - startTime) / 60000;
			const int secs = (currentTime - startTime) / 1000 % 60;
			std::cout << "] " << (float) count / FILE_LENGTH * 100 << "% " << mins << ":" <<
				std::setfill('0') <<  std::setw(2) << secs << "\r" << std::flush;
		}
		
		count++;

		if (line[0] == '\0')
		{
			nextIsName = true;
		}
		else if (nextIsName)
		{
			nextIsName = false;
			if (!articles.contains(line))
				articles.emplace(std::map<std::string, Article>::value_type(line, Article(line)));

			currentArticle = &articles.find(line)->second;
		}
		else
		{
			if (!articles.contains(line))
				articles.emplace(std::map<std::string, Article>::value_type(line, Article(line)));

			currentArticle->addLink(&articles.find(line)->second);
		}
	}

	file.close();
	std::cout << std::endl << "Done reading file." << std::endl;
	return articles;
}

/**
 * @brief Perform a breadth-first search from the start article to the end article
 * @param start The start article
 * @param end The end article, to search for
 * @return std::vector<const Article*> The path of nodes visited from start to end
 */
std::vector<const Article*> breadthFirstSearch(const Article* start, const Article* end)
{	
	std::unordered_map<const Article*, std::tuple<const Article*, const int>> visits; // node, <source, cost>
	std::queue<std::tuple<const Article*, const Article*, const int>> queue; // <node, source, cost>

	queue.push(std::make_tuple(start, nullptr, 0));
	
	// Add node links to the queue and visit them until we find the end or run out of nodes to visit
	while (!queue.empty())
	{
		std::tuple<const Article*, const Article*, int> next = queue.front();
		queue.pop();

		const Article* node = std::get<0>(next);
		const int cost = std::get<2>(next);

		for (const Article* link : node->links)
		{
			if (!visits.contains(link))
			{
				if (link == end)
				{
					visits.emplace(link, std::make_tuple(node, cost + 1));
					goto found; // Break out of the for loop and surrounding while loop
				}
				else
				{
					visits.emplace(link, std::make_tuple(node, cost + 1));
					queue.push(std::make_tuple(link, node, cost + 1));
				}
			}
		}
	}

	return std::vector<const Article*>();

	found:
	// Reconstruct path backwards
	std::vector<const Article*> path;
	const Article* current = end;

	while (current != start)
	{
		path.push_back(current);
		current = std::get<0>(visits[current]);
	}

	path.push_back(start);
	std::reverse(path.begin(), path.end());

	return path;
}
