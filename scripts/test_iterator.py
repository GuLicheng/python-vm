vals = [1, "Hello", 2]

print(vals)

for val in vals:
    print(val)

print("===================")
d = { 1: "Hello", 2: "World" }

for val in d:
    print(val)

for val in d.items():
    print(val)

for val in d.keys():
    print(val)

for val in d.values():
    print(val)


print("==================")

print(iter(d))
print(iter(d.items()))
print(iter(d.keys()))
print(iter(d.values()))
print(iter(vals))
