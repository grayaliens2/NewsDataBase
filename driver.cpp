#include <iostream>
#include <string>
#include <typeinfo>
#include "Articles.hpp"
#include <string.h>
#include <fstream>
#include <cctype>
using namespace std;
string remo(string line){
  for(int i = 0, len = line.size(); i < len; i++){
      if (ispunct(line[i]))
      {
        line.erase(i--, 1);
        len = line.size();
      }
  }
  return line;
}
void display(){
  cout << "=======Main Menu=======" << endl;
  cout << "1. Print the inventory" << endl;
  cout << "2. Add an Article" << endl;
  cout << "3. Delete an Article" << endl;
  cout << "4. Print Contents of an Article" << endl;
  cout << "5. Search for an Article" << endl;
  cout << "6. Quit" << endl;
}

main(int argc, char const *argv[]){
  string a, b, c, d, e, userInput;
  int count = 500;
  cout << "A database of News Articles" << endl << "by Ross Panning" << endl;
  int i = 0;
  ifstream infile;
  ArticleTree art;
  infile.open(argv[1]);
  if(infile.fail()){
    cout << "Failed to open the file." << endl;
  }
  if(!infile.fail()){
      while(getline(infile, a, ',')){
        getline(infile, b, ',');
        getline(infile, c, ',');
        getline(infile, d, ',');
        getline(infile, e, '\n');
        art.addArticleNode(b, c, d, art.views(), e);
    }
  }
  infile.close();
  while(userInput != "6"){
    display();
    getline(cin, userInput);
    switch(stoi(userInput)){
      case 1:
        cout << "Printing all Article Titles" << endl;
        art.display();
        break;
      case 2:
      {
        ofstream outfile;
        count++;
        outfile.open(argv[2]);
        cout << "Please enter the title of the Article: ";
        getline(cin, d);
        cout << endl << "Please enter the date of the Article: ";
        getline(cin, b);
        cout << endl << "Please enter the view count of this Article: ";
        getline(cin, a);
        cout << endl << "Please enter the text of this Article: ";
        getline(cin, e);
        cout << endl << "Please now enter the URL address of this Article: ";
        getline(cin, c);
        cout << endl << "Now adding Article" << endl;
        art.addArticleNode(b, c, d, stoi(a), e);
        outfile << count << "," << b << "," << c << "," << d << "," << e << endl;
        outfile.close();
        break;
      }
      case 3:
        cout << "Please enter the title of the article you want to delete: " << endl;
        getline(cin, d);
        cout << endl;
        art.deleteArticle(d);
        break;
      case 4:
        cout << "Please enter the Article name here: ";
        getline(cin, d);
        cout << endl;
        art.printArticleContents(d);
        break;
      case 5:
        cout << "Please enter a keyword you would like to search for: ";
        getline(cin, a);
        art.searchArticle(a);
        break;
    }
  }
  cout << "Goodbye" << endl;
  return 0;
}
