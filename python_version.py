import json
raw = open("cancer.json", 'r')

a = json.load(raw)
print("What to sort by?\nchoices:\n")
print("Year, Area")

sortby = input()

to_sort = []

for data in a:
    to_sort.append(data[sortby])

to_sort.sort()
out = ''
f = open("output.txt","w")
for thing in to_sort:
    out += str(thing) + '\n'

f.write(out)
