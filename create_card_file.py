import os

cards = []

f = open('cards.txt', 'w')
for i in range(1,5):
    suit = 'C'
    if i == 2:
        suit = 'D'
    elif i == 3:
        suit = 'H'
    elif i == 4:
        suit = 'S'
    for j in range(1,14):
        if j == 1:
            f.write('A' + suit + '\n')
        elif(j == 11):
            f.write('J' + suit + '\n')
        elif(j == 12):
            f.write('Q' + suit + '\n')
        elif(j == 13):
            f.write('K' + suit + '\n')
        elif(j == 10):
            f.write('T' + suit + '\n')
        else:
            f.write(str(j) + suit+ '\n')

