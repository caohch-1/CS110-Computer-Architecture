# created by caohch1 3/28/2021
# Please change your 2rd code in heap.S to ".import test.S"
# If you see "Fuck CA!" then you passed, or you will get python's heapq's ans and yours
import heapq
from random import randint
from sys import exit
import subprocess

# valve used to control num of -1 and -2 in input, should from 0..100
valve = 30

# Init data
input = list()
input_len = randint(1, 50)
for i in range(input_len):
    if randint(0, 100) >= valve:
        ele = randint(-2, 100)
        while (ele in input and ele != -1 and ele != -2):
            ele = randint(-2, 100)
        input.append(ele)
    else:
        input.append(randint(-2, -1))
print("Input_len: "+str(len(input)))
print("Input:\t", end=" ")
for ele in input: print(str(ele), end = " ")
print("\n"+"-"*150)

# Create test.S file
with open("test.S", "w+") as f:
    f.write(".data\n")
    f.write(".globl	input\n")
    f.write("input:\n")
    f.write("	.word	")
    for ele in input: f.write(str(ele)+" ")
    f.write("\n")

    f.write(".globl  input_len\n")
    f.write("input_len:\n")
    f.write("        .word  " + str(len(input)) + "\n")

    f.write(".globl	heap\n")
    f.write("heap:\n")
    f.write("	.word	" + "0 "*32 + "\n")

    f.write(".globl	len\n")
    f.write("len:\n")
    f.write("	.word	0\n")

# Get your Res
yourCodeRes = subprocess.run("java -jar venus-jvm-latest.jar heap.S", shell = True, stdout=subprocess.PIPE)
yourCodeRes = yourCodeRes.stdout.decode()
print(yourCodeRes, end="")
print("-"*150)

# GroundTruth Ans
truth = list()
heap = list()
for i in input:
    if i == -2:
        # print("Print:\t", end=" ")
        tempStr = str()
        for ele in heap: 
            print(str(ele), end = " ")
            tempStr += (str(ele) + " ")
        print()
        truth.append(tempStr)
    elif i == -1:
        try:
            # print("Pop:\t", end=" ")
            print(heap[0])
            truth.append(str(heap[0]))
            heapq.heappop(heap)
        except:
            print("\nExited with error code -1 (underflow)")
            truth.append("")
            truth.append("Exited with error code -1")
            truth.append("")

            print("-"*150)
            if (yourCodeRes.split("\n") != truth):
                print(yourCodeRes.split("\n"))
                print(truth)
            else:
                print("Fuck CA!")

            exit()

    else:
        if len(heap) == 32:
            print("\nExited with error code -1 (overflow)")
            truth.append("")
            truth.append("Exited with error code -1")
            truth.append("")

            print("-"*150)
            if (yourCodeRes.split("\n") != truth):
                print(yourCodeRes.split("\n"))
                print(truth)
            else:
                print("Fuck CA!")


            exit()
        heapq.heappush(heap, i)

print("\nExited with error code 0")
truth.append("")
truth.append("Exited with error code 0")
truth.append("")


print("-"*150)
if (yourCodeRes.split("\n") != truth):
    print(yourCodeRes.split("\n"))
    print(truth)
else:
    print("Fuck CA!")