#include "Articles.hpp"
#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <string.h>
using namespace std;
int numViews = 0;
ArticleTree::ArticleTree(){
  root = nullptr;
  head = nullptr;
  this->hashTableSize = 50; //abitrary value will expirement to find best fit that balances articles with lots of words vs small articles
}
void deleteTree(articleNode* node){
  if (node == NULL)
    return;
  else{
    deleteTree(node->leftChild);
    deleteTree(node->rightChild);
    for(int i = 0; i < 50; i++){
      node->hashTable[i] = nullptr;
    }
    delete node;
  }
}
ArticleTree::~ArticleTree(){
  deleteTree(root);
}
articleNode* searchRecursion(articleNode* n, string title, int views){
  if(n == nullptr)
    return nullptr;
  else{
    if(n->title == title)
      return n;
    else if(n->views > views)
      return searchRecursion(n->leftChild, title, views);
    else
      return searchRecursion(n->rightChild, title, views);
  }
}
unsigned int ArticleTree::getHash(string word){
  unsigned int hashValue = 5381;
  int length = word.length();
  for (int i = 0; i < length; i++){
    hashValue=((hashValue<<5)+hashValue) + word[i];
  }
  hashValue %= hashTableSize;
  return hashValue;
}
bool ArticleTree::isInTable(articleNode* a, string word){
  int index = getHash(word);
  wordItem * n = a->hashTable[index];
  bool found = false;
  while(!found && n != nullptr){
    if(n->word == word)
      found = true;
    else
      n = n->next;
  }
  return found;
}
wordItem *createNode(string word, wordItem *next){
    wordItem* nw = new wordItem;
    nw->word = word;
    nw->next = next;
    nw->count = 1;
    return nw;
}
void ArticleTree::addWords(articleNode* a, string word){
  if(!isInTable(a, word)){
    int index = getHash(word);
    wordItem * n = createNode(word, nullptr);
    if(a->hashTable[index]== nullptr){
        a->hashTable[index] = n;
      }
    else{
      wordItem * prev = a->hashTable[index];
      a->hashTable[index] = n;
      n->next = prev;
    }
  }
  else{
    incrementCount(a, word);
  }
}
void ArticleTree::parseWords(articleNode* n){
  string a, b, c, d;
  stringstream ss(n->text);
  while(getline(ss, a, ' ')){
    if(b != "")
      d = b + " " + a;
    if(getline(ss, b, ' '))
      c = a + " " + b;
    addWords(n, a);
    addWords(n, b);
    addWords(n, c);
    addWords(n, d);
  }

}
void ArticleTree::addArticleNode(string date, string source, string title, int view, string text){
  articleNode * foundNode = searchRecursion(root, title, view);
  if(foundNode != nullptr)
    cout << "Article already added to database" << endl;
  else{
    articleNode * prev = nullptr;
    articleNode * temp = root;
    articleNode * newNode = new articleNode;
    newNode->date = date;
    newNode->source = source;
    newNode->title = title;
    newNode->views = view;
    newNode->text = text;
    newNode->hashTable = new wordItem*[hashTableSize];
    for(int i = 0; i < hashTableSize; i++){
      newNode->hashTable[i] = nullptr;
    }
    parseWords(newNode);
    newNode->parent = nullptr;
    newNode->leftChild = nullptr;
    newNode->rightChild = nullptr;
    while(temp != nullptr){
      prev = temp;
      if(newNode->views < temp->views){
        temp = temp->leftChild;
      }
      else{
        temp = temp->rightChild;
      }
    }
    if(prev == nullptr){
      root = newNode;
    }
    else if(newNode->views < prev->views){
      prev->leftChild = newNode;
      newNode->parent = prev;
    }
    else{
      prev->rightChild = newNode;
      newNode->parent = prev;
    }
  }

}
int ArticleTree::count(articleNode* current, string word){
  int index = getHash(word);
  wordItem * a = current->hashTable[index];
  while(a != nullptr){
    if(a->word == word)
      return a->count;
    else
      a = a->next;
  }
  return 0;
}
articleLL* ArticleTree::newArticleLL(string title, int count){
  articleLL * newNode = new articleLL;
  newNode->name = title;
  newNode->hits = count;
  newNode->nextArticle = nullptr;
  return newNode;
}
void ArticleTree::addArticleLL(struct articleLL **start, articleLL* newNode){
  if(*start== nullptr || (*start)->hits < newNode->hits){
    newNode->nextArticle = *start;
    *start = newNode;
  }
  else{
    articleLL* current = *start;
    while(current->nextArticle != nullptr && current->nextArticle->hits >= newNode->hits){
      current = current->nextArticle;
    }
    newNode->nextArticle = current->nextArticle;
    current->nextArticle = newNode;
  }
}
void ArticleTree::searchArticleRecursion(articleNode* r, string word){
  if(r != NULL){
    searchArticleRecursion(r->leftChild, word);
    if(isInTable(r, word) == true){
      articleLL * newNode = newArticleLL(r->title, count(r, word));
      addArticleLL(&head, newNode);
    }
    searchArticleRecursion(r->rightChild, word);
  }
}
void ArticleTree::deleteAllArticleLL(){
  articleLL*current = head;
  articleLL*next = new articleLL;
  while(current != nullptr){
    next = current->nextArticle;
    free(current);
    current = next;
  }
  head = nullptr;
}
void ArticleTree::searchArticle(string word){
  cout << "Here are the Articles that contain: " << word << endl
    << "===========================================" << endl;
  searchArticleRecursion(root, word);
  articleLL*current = head;
  while(current != nullptr){
    cout << word << " found in " << current->name << " " << current->hits << " time(s)" << endl;
    current = current->nextArticle;
  }
  deleteAllArticleLL();
}
int ArticleTree::views(){
  return rand() % 500000 + 10000;
}
void displayRecursion(articleNode* node){
  if(node != nullptr){
     displayRecursion(node->rightChild);
     cout << node->title << " number of views " << node->views << endl;
     displayRecursion(node->leftChild);
   }
}
void ArticleTree::display(){
  displayRecursion(root);
}
void displayRecursionCon(articleNode* node, string title){
  if(node != nullptr){
     displayRecursionCon(node->rightChild, title);
     if(node->title == title)
        cout << node->text;
     displayRecursionCon(node->leftChild, title);
   }
}
void ArticleTree::printArticleContents(string title){
  cout << "Printing Articles contents" << endl;
  displayRecursionCon(root, title);
  cout << endl;
}
void ArticleTree::incrementCount(articleNode* current, string word){
  int index = getHash(word);
  wordItem * a = current->hashTable[index];
  while(a != nullptr){
    if(a->word == word){
      a->count++;
      break;
    }
    else
      a = a->next;
  }
}
articleNode* getMinValueNode(articleNode* currNode){

    if(currNode->leftChild == NULL){
      return currNode;
    }
    return getMinValueNode(currNode->leftChild);
}
articleNode* deleteNode(articleNode *currNode, int value)
{
    if(currNode == NULL){
    return NULL;
  }
  else if(value < currNode->views){
    currNode->leftChild = deleteNode(currNode->leftChild, value);
  }
  else if(value > currNode->views){
    currNode->rightChild = deleteNode(currNode->rightChild, value);
  }
  else{
    if(currNode->leftChild == NULL && currNode->rightChild == NULL){
      delete currNode;
      currNode = nullptr;
    }
    else if(currNode->leftChild == NULL){
      articleNode * temp = currNode;
      currNode = currNode->rightChild;
      delete temp;
    }
    else if(currNode->rightChild == NULL){
      articleNode * temp = currNode;
      currNode = currNode->leftChild;
      delete temp;
    }
    else{
      articleNode * temp = getMinValueNode(currNode->rightChild);
      currNode->views = temp->views;
      currNode->title = temp->title;
      currNode->source = temp->source;
      currNode->date = temp->date;
      currNode->text = temp->text;
      currNode->hashTable = temp->hashTable;
      currNode->rightChild = deleteNode(currNode->rightChild, temp->views);
    }
  }
  return currNode;
}
void findViews(articleNode*current, string title){
  if(current != nullptr){
    findViews(current->rightChild, title);
    if(current->title == title)
      numViews = current->views;
    findViews(current->leftChild, title);
  }
}
void ArticleTree::deleteArticle(string title){
  findViews(root, title);
  cout << numViews << endl;
  articleNode* foundNode = searchRecursion(root, title, numViews);
  if(foundNode == nullptr){
    cout << "Article: " << title << " not found, cannot delete." << endl;
    return;
  }
  else{
    root = deleteNode(root, numViews);
  }
  numViews = 0;
}
