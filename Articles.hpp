#ifndef ATICLES_HPP
#define ARTICLES_HPP
#include <iostream>
#include <string>
using namespace std;
struct wordItem{
  string word;
  int count;
  wordItem* next;
};
struct articleNode{
  string date;
  string source;
  string title;
  int views;
  string text;
  articleNode *parent = NULL;
  articleNode *leftChild = NULL;
  articleNode *rightChild = NULL;
  wordItem** hashTable;
};
struct articleLL{
  string name;
  int hits;
  articleLL*  nextArticle;
};
class ArticleTree{
  public:
    ArticleTree();
    ~ArticleTree();
    articleLL* newArticleLL(string, int);
    void addArticleNode(string, string, string, int, string);
    void printArticleContents(string);
    int count(articleNode*, string);
    void deleteAllArticleLL();
    bool isInTable(articleNode*, string);
    void addArticleLL(articleLL**, articleLL*);
    void incrementCount(articleNode*, string);
    void searchArticleRecursion(articleNode*, string);
    void searchArticle(string);
    void deleteArticle(string);
    int views();
    void display();
  private:
    articleLL* head;
    articleNode* root;
    unsigned int getHash(string);
    void parseWords(articleNode*);
    void addWords(articleNode*, string);
    int hashTableSize;


};
#endif
