#include <iostream>
#include <string>

using namespace std;

string removeSpace(string);
bool isPal(string&, int, int);
string mergeSort(string);
string wordType(string);
string wordOrder(string);

int main()
{
	
	string testWord;
	string newMerge;
	string newWord;
	string word;
	int first = 0;
	int last = string::npos;

	cout << "Enter a word to tell whether or not it is a palindrome." << endl;

	getline(cin, testWord);

	word = mergeSort(testWord);
	newWord = removeSpace(testWord);

	cout << "Your entry " << (isPal(newWord, first, last) ? "is a palindrome." : "is not a palindrome.") << endl;
	cout << endl;
	cout << "The order in the word is " << word << endl;
	cout << endl;
	cout <<  wordType(word) << endl;
	cout << endl;
	cout << wordOrder(word) << endl;

	return 0;
}

string removeSpace(string testWord)
{
	string newWord;

	for (int c = 0; c < testWord.length(); c++)
	{
		if (isalnum(testWord[c]))
		{
			newWord += testWord[c];
		}

	}

	return newWord;
}



bool isPal(string& newWord, int first = 0, int last = string::npos)
{

	if (last == string::npos)
	{
		last = (newWord.length()-1);
	}
	if (newWord[first] == newWord[last])
	{
		if ((first-last) == 0)
		{
			return true;
		}
		else if (first == (last - 1))
		{
			return true;
		}
		else
		{
			return isPal(newWord, first+1, last-1);
		}
	}
	else
	{
		return false;
	}
}


string merge(string str1, string str2) {
  string final = "";
  int i = 0, j = 0;
  bool fromStr1 = false;

  while (true) {
    if (i >= (int)str1.size()) {
      break;
    }
    if (j >= (int)str2.size()) {
      fromStr1 = true; 
      break;
    }

    if (str1[i] < str2[j]) {
      final += str1[i];
      i++;
    }
    else {
      final += str2[j];
      j++;
    }
  }

  if (fromStr1) {
    for (int t = i; t < (int)str1.size(); t++) {
      final += str1[t];
    }
  }
  else {
    for(int t = j; t < (int)str2.size(); t++) {
      final += str2[t];
    }
  }

  return final;
}

string mergeSort(string str1) {
  int len = str1.size();
  if (len <= 1)
    return str1;
  else {
    string newStr1 = mergeSort(str1.substr(0, len / 2));
    string newStr2 = mergeSort(str1.substr(len / 2, len - len / 2));
    return merge(newStr1, newStr2);
  }
}

string wordType(string str)
{
	int count = 0;
	int end = (str.length()-1);
	for (int i = 0; i <= end; i++)
	{
		if (str[i] == str[i+1])
		{
			count++;
		}
	}
	
	if (count % 2 == 0 && (end % 2) != 1)
	{
		cout << "type 1 palindrome!" << endl;
	}
	else
	{
		cout << "type 2 palindrome!" << endl;
	}
}

string wordOrder(string str)
{
	int pan = 0;
	int end = (str.length()-1);
	for (int i = 0; i <= end; i++)
	{
		if (str[0] == str[i+1])
		{
			pan++;
		}
	
	}
	cout << pan+1 << endl;

	
}