import csv
from random import *
import math
import time

header = ['question title', 'author', 'assignment/topic', 'date posted', 'message content', 'number of reactions for subscribe to this', 'number of reactions for this is important', 'answer 1 content', 'answer 1 number of reactions for this is helpful']
data = ['aTitle', 'anAuthor', 'assignment1', 'date123455', 'my message here', '123', '3456', 'here is my answer', '04902393293942']

titles = ['Harry', 'Ron', 'Hermione', 'Penelope', 'Fred', 'George', 'Luna', 'Pavarti', 'Padma', 'Percy']


def load_words():
    with open('words_dictionary.json') as word_file:
        valid_words = list(word_file.read().split())
       
    return valid_words

def get_random_word(words):
    random = randint(0, 200000)
    rawWord = words[2*random + 1];
    return rawWord[1: math.floor(len(rawWord) - 2)]

def generate_phrase(words, numWords):
    s = get_random_word(words)
    for i in range(1, numWords):
        s += " " +  get_random_word(words)
    return s

def get_random_author():
    return titles[randint(0, 9)]

if __name__ == '__main__':
    with open('questions.csv', 'w', encoding='UTF8') as f:
        writer = csv.writer(f)
        
        words = load_words()

        # write the header
        writer.writerow(header)
    
        for i in range(0, 100000):
            title = generate_phrase(words, 3)  
            author = get_random_author()
            assignment = randint(1, 20)
            date = int(time.time()) + randint(-100000, 0) 
            message  = generate_phrase(words,12)
            numSubscribe     = randint(1, 50)
            numImportant     = randint(1, 50)
            answer1content   = generate_phrase(words, 12)
            numHelpful       = randint(1, 50)
            
            data = [title, author, assignment, date, message, numSubscribe, numImportant, answer1content, numHelpful]
            writer.writerow(data)
