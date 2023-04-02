import sys
import string
import re


# 1. Load file
# 2. Change to lower case
# 3. Remove not words
# 4. delete stop words
# 5. Count

def sanitizeFile(filename):
    file = open(filename, encoding="UTF-8")
    words = file.read().lower().split()
    file.close()

    table = str.maketrans('', '', string.punctuation)
    words = [w.translate(table) for w in words if w.isalpha() and len(w) > 2]

    return words

def removeStopWords(words, stopWordsFile = 'stop_words_english.txt'):
    file = open(stopWordsFile)
    stopWords = file.read().split()
    file.close()
    words = [w for w in words if w not in stopWords]
    return words

def countWords(words):
    words.sort()
    lastWord = words[0]
    dict = {}
    dict[lastWord] = 1
    for i in range(1,len(words)):
        if words[i] == lastWord:
            dict[lastWord] = dict[lastWord] + 1
        else:
            dict[words[i]] = 1
        lastWord = words[i]
    return dict

def saveDictionaryForWordsclouds(dict):
    # "weight";"word";"color";"url"
    sorted_dict = sorted(dict.items(), key=lambda x:x[1], reverse=True)
    out = open("out.csv", mode="w", encoding="UTF-8")
    out.write('"weight";"word";"color";"url"\n')
    wordsToOutput = 80
    for (word, count) in sorted_dict:
        out.write('"' + str(count) + '";"' + str(word) + '";"";""\n')
        wordsToOutput -= 1
        if wordsToOutput == 0: 
            break
    out.close()


def run():
    if len(sys.argv) < 2:
        print("Missing arguments")
    
    words = sanitizeFile(sys.argv[1])
    words = removeStopWords(words)
    dict = countWords(words)
    saveDictionaryForWordsclouds(dict)
    

run()