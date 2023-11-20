import filecmp
import os

test = 1
ans = 1
count = 0
while ans == 1 and count < 100:
    print(test)
    test += 1
    count += 1
    os.system("./gen.out > input.txt")
    print("a.txt")
    os.system("./wrong.out < input.txt > wrong.txt")
    print("b.txt")
    os.system("./slow.out < input.txt > slow.txt")
    file1 = "./slow.txt"
    file2 = "./wrong.txt"
    ans = filecmp.cmp(file1, file2, shallow = False)
