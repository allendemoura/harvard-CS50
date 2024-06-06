while True:
    try:
        name = input("What is your name? ")
        break
    except:
        print("You goofed.")
print("Hello, " + name)