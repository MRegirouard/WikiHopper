#include "Article.hpp"

/**
 * @brief Construct a new Article, with the given name.
 * @param name The name of the Wikipedia article.
 */
Article::Article(const std::string name) : name(name)
{}

/**
 * @brief Add a link from this article to the given article.
 * @param article The article to link to.
 */
void Article::addLink(const Article* article)
{
    links.push_back(article);
}
