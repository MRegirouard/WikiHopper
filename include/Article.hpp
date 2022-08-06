#include <string>
#include <vector>

/**
 * @brief Represents a Wikipedia article, with a name and a list of links to other articles.
 */
class Article
{
public:
    Article(const std::string name);
    void addLink(const Article* article);
    std::vector<const Article*> links;
    const std::string name;
};
