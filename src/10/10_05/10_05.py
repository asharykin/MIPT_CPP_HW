import matplotlib.pyplot as plt

sizes = []
data = [[] for _ in range(9)]

with open("data.txt") as f:
    for line in f:
        line = line.strip()
        if not line or not line[0].isdigit():
            continue

        parts = list(map(int, line.split()))
        sizes.append(parts[0])
        for i in range(9):
            data[i].append(parts[i+1])

names = ["RS","JS","PJW","ELF","BKDR","SDBM","DJB","DEK","AP"]

for i in range(9):
    plt.plot(sizes, data[i], label=names[i])

plt.legend()
plt.xlabel("Number of strings")
plt.ylabel("Collisions")
plt.show()