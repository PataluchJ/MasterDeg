import sys
import string
import re
import math

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

def getDictionaryForFile(filepath):
    words = sanitizeFile(filepath)
    words = removeStopWords(words)
    dict = countWords(words)
    return dict

def calculateIDFs(wordCountsPerFile):
    IDFs = {}

    files = wordCountsPerFile.keys()

    for f in files:
        for w in wordCountsPerFile[f]:
            if w in IDFs.keys():
                IDFs[w] += 1.0
            else:
                IDFs[w] = 1.0

    for k in IDFs.keys():
        IDFs[k] = math.log2(len(files)/IDFs[k])

    return IDFs
    
def calculateTFs(wordCount):
    sum = 0
    for k in wordCount.keys():
        sum += wordCount[k]
    
    TFs = {}

    for k in wordCount.keys():
        TFs[k] = wordCount[k] / sum

    return TFs; 

def saveWordCloudFile(name, TF, IDF):
    file = open(name + '_tfidf.csv', mode="w", encoding="UTF-8")
    file.write('"weight";"word";"color";"url"\n')
    for w in TF.keys():
        tfidf = TF[w] * IDF[w]
        file.write('"' + str(int(tfidf*100000)) + '";"' + w + '";"";""\n')
    file.close()

def run():
    files = ["hamlet", "KingLear", "Othello", "RomeoJuliet"]
    wordCountInFiles = {}
    TFs = {}
    IDF = {}

    for file in files:
        wordCountInFiles[file] = getDictionaryForFile('Shakespear/'+file+'.txt')
        TFs[file] = calculateTFs(wordCountInFiles[file])
    

    IDF = calculateIDFs(wordCountInFiles)

    for f in files:
        saveWordCloudFile(f, TFs[f], IDF)
    
    
run()