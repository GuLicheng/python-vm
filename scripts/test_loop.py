a = 1
b = 0
i = 0

while i < 10:
    print(a)
    t = a
    a = a + b
    b = t
    i = i + 1

i = 0
while i < 5:
    i = i + 1
    if i % 2 == 0:
        continue
    print(i)

i = 0
j = 0
while i < 2: 
    i = i + 1
    while j < 4:
        j = j + 1
        break
print(i)
print(j)

